
#include <stdlib.h>
#include <stdio.h>
#include "dan_bsp.h"

enum { sending, receiving };

void dan_bsp_init(dan_bsp* b, int tag, int ibarrier_tag)
{
    if (tag == ibarrier_tag)
    {
        fprintf(stderr,"dan_bsp_use_ibarrier: the ibarrier tag must be different from the bsp tag\n");
        exit(EXIT_FAILURE);
    }
    b->tag = tag;
    b->ibarrier.tag = ibarrier_tag;
}

static void free_all_receivers(dan_aa_tree* t)
{
    if (*t == &dan_aa_bottom)
        return;
    free_all_receivers(&((*t)->left));
    free_all_receivers(&((*t)->right));
    dan_bsp_receiver* receiver;
    receiver = (dan_bsp_receiver*) *t;
    dan_mpi_free(&(receiver->message));
    dan_free(receiver);
    *t = &dan_aa_bottom;
}

void dan_bsp_begin_superstep(dan_bsp* b)
{
    dan_bsp_free(b);
}

static bool receiver_less(dan_aa_node* a, dan_aa_node* b)
{
    return ((dan_bsp_receiver*)a)->message.peer
         < ((dan_bsp_receiver*)b)->message.peer;
}

static dan_bsp_receiver* find_receiver(dan_aa_tree t, int peer)
{
    dan_bsp_receiver key;
    key.message.peer = peer;
    return (dan_bsp_receiver*) dan_aa_find((dan_aa_node*)&key,t,receiver_less);
}

void dan_bsp_reserve(dan_bsp* b, int peer, size_t bytes)
{
    dan_bsp_receiver* receiver;
    receiver = find_receiver(b->tree,peer);
    dan_bsp_receiver temp = DAN_BSP_RECEIVER_INIT;
    if (!receiver)
    {
        receiver = dan_malloc(sizeof(*receiver));
        *receiver = temp;
        receiver->message.peer = peer;
        dan_aa_insert((dan_aa_node*)receiver,&(b->tree),receiver_less);
    }
    receiver->message.buffer.size += bytes;
}

static void allocate(dan_aa_tree t)
{
    if (t == &dan_aa_bottom)
        return;
    allocate(t->left);
    dan_buffer* buffer = &(((dan_bsp_receiver*)t)->message.buffer);
    size_t size = buffer->size;
    buffer->size = 0;
    dan_buffer_realloc(buffer,size);
    allocate(t->right);
}

void dan_bsp_allocate(dan_bsp* b)
{
    allocate(b->tree);
}

static void begin_packing(dan_aa_tree t)
{
    if (t == &dan_aa_bottom)
        return;
    begin_packing(t->left);
    ((dan_bsp_receiver*)t)->message.buffer.size = 0;
    begin_packing(t->right);
}

void dan_bsp_begin_packing(dan_bsp* b)
{
    begin_packing(b->tree);
}

void* dan_bsp_pack(dan_bsp* b, int peer, size_t bytes)
{
    dan_bsp_receiver* receiver;
    receiver = find_receiver(b->tree,peer);
    if (!receiver)
    {
        fprintf(stderr,"dan_bsp_pack: there is no receiver for this peer id. use dan_bsp_reserve to allocate a receiver with enough space first.\n");
        exit(EXIT_FAILURE);
    }
    void* at = receiver->message.buffer.data;
    at = dan_pointer_add(at,receiver->message.buffer.size);
    receiver->message.buffer.size += bytes;
    return at;
}

static void send(dan_aa_tree t, int tag)
{
    if (t == &dan_aa_bottom)
        return;
    send(t->left,tag);
    dan_bsp_receiver* receiver;
    receiver = (dan_bsp_receiver*)t;
    dan_mpi_send(&(receiver->message),tag);
    send(t->right,tag);
}

void dan_bsp_send(dan_bsp* b)
{
    send(b->tree,b->tag);
}

static bool done_sending(dan_aa_tree t)
{
    if (t == &dan_aa_bottom)
        return true;
    dan_bsp_receiver* receiver;
    receiver = (dan_bsp_receiver*)t;
    return dan_mpi_done(&(receiver->message))
        && done_sending(t->left) && done_sending(t->right);
}

bool dan_bsp_receive(dan_bsp* b)
{
    while (!dan_mpi_receive(&(b->received),b->tag))
    {
        if (b->state == dan_bsp_sending)
        {
            if (done_sending(b->tree))
            {
                dan_mpi_begin_ibarrier(&(b->ibarrier),b->ibarrier.tag);
                b->state = dan_bsp_receiving;
            }
        }
        else if (dan_mpi_ibarrier_done(&(b->ibarrier)))
            return false;
    }
    return true;
}

void dan_bsp_free(dan_bsp* b)
{
    /* ensures no one starts a new superstep while others are receiving
       in the past superstep */
    if (b->state == dan_bsp_receiving)
        while (!dan_mpi_ibarrier_done(&(b->ibarrier)));
    free_all_receivers(&(b->tree));
    dan_mpi_free(&(b->received));
    b->state = sending;
}

