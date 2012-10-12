
#include <stdlib.h>
#include <stdio.h>
#include "dan_pmsg.h"

void dan_pmsg_init(dan_pmsg* b)
{
    dan_pmsg temp = DAN_PMSG_INIT;
    *b = temp;
    if (tag == ibarrier_tag)
    {
        fprintf(stderr,"dan_pmsg_use_ibarrier: the ibarrier tag must be different from the pmsg tag\n");
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
    dan_pmsg_receiver* receiver;
    receiver = (dan_pmsg_receiver*) *t;
    dan_mpi_free(&(receiver->message));
    dan_free(receiver);
    *t = &dan_aa_bottom;
}

void dan_pmsg_begin_superstep(dan_pmsg* b)
{
    dan_pmsg_free(b);
}

static bool receiver_less(dan_aa_node* a, dan_aa_node* b)
{
    return ((dan_pmsg_receiver*)a)->message.peer
         < ((dan_pmsg_receiver*)b)->message.peer;
}

dan_pmsg_receiver* dan_pmsg_find_receiver(dan_aa_tree t, int peer)
{
    dan_pmsg_receiver key;
    key.message.peer = peer;
    return (dan_pmsg_receiver*) dan_aa_find((dan_aa_node*)&key,t,receiver_less);
}

void dan_pmsg_reserve(dan_pmsg* b, int peer, size_t bytes)
{
    dan_pmsg_receiver* receiver;
    receiver = dan_pmsg_find_receiver(b->tree,peer);
    if (!receiver)
    {
        receiver = dan_malloc(sizeof(*receiver));
        dan_pmsg_receiver temp = DAN_PMSG_RECEIVER_INIT;
        *receiver = temp;
        receiver->message.peer = peer;
        dan_aa_insert((dan_aa_node*)receiver,&(b->tree),receiver_less);
    }
    dan_mpi_reserve(&(receiver->message),bytes);
}

bool dan_pmsg_has_receiver(dan_pmsg* b, int peer)
{
    return dan_pmsg_find_receiver(b->tree,peer);
}

size_t dan_pmsg_reserved(dan_pmsg* b, int peer)
{
    dan_pmsg_receiver* receiver;
    receiver = dan_pmsg_find_receiver(b->tree,peer);
    if (!receiver)
    {
        fprintf(stderr,"dan_pmsg_reserved: there is no receiver for this peer id. use dan_pmsg_reserve to allocate a receiver with enough space first.\n");
        exit(EXIT_FAILURE);
    }
    return dan_mpi_reserved(&(receiver->message));
}

static void begin_packing(dan_aa_tree t)
{
    if (t == &dan_aa_bottom)
        return;
    begin_packing(t->left);
    dan_pmsg_receiver* receiver;
    receiver = (dan_pmsg_receiver*) t;
    dan_mpi_begin_packing(&(receiver->message));
    begin_packing(t->right);
}

void dan_pmsg_begin_packing(dan_pmsg* b)
{
    begin_packing(b->tree);
}

void* dan_pmsg_pack(dan_pmsg* b, int peer, size_t bytes)
{
    dan_pmsg_receiver* receiver;
    receiver = dan_pmsg_find_receiver(b->tree,peer);
    if (!receiver)
    {
        fprintf(stderr,"dan_pmsg_pack: there is no receiver for this peer id. use dan_pmsg_reserve to allocate a receiver with enough space first.\n");
        exit(EXIT_FAILURE);
    }
    return dan_mpi_pack(&(receiver->message),bytes);
}

static void send(dan_aa_tree t, int tag)
{
    if (t == &dan_aa_bottom)
        return;
    send(t->left,tag);
    dan_pmsg_receiver* receiver;
    receiver = (dan_pmsg_receiver*)t;
    dan_mpi_send(&(receiver->message),tag);
    send(t->right,tag);
}

void dan_pmsg_send(dan_pmsg* b)
{
    send(b->tree,b->tag);
}

static bool done_sending(dan_aa_tree t)
{
    if (t == &dan_aa_bottom)
        return true;
    dan_pmsg_receiver* receiver;
    receiver = (dan_pmsg_receiver*)t;
    return dan_mpi_done(&(receiver->message))
        && done_sending(t->left) && done_sending(t->right);
}

bool dan_pmsg_receive(dan_pmsg* b)
{
    b->received.peer = MPI_ANY_SOURCE;
    while (!dan_mpi_receive(&(b->received),b->tag))
    {
        if (b->state == dan_pmsg_sending)
        {
            if (done_sending(b->tree))
            {
                dan_mpi_begin_ibarrier(&(b->ibarrier),b->ibarrier.tag);
                b->state = dan_pmsg_receiving;
            }
        }
        else if (dan_mpi_ibarrier_done(&(b->ibarrier)))
        {
            dan_mpi_begin_ibarrier(&(b->ibarrier),b->ibarrier.tag);
            return false;
        }
    }
    return true;
}

void dan_pmsg_free(dan_pmsg* b)
{
    /* ensures no one starts a new superstep while others are receiving
       in the past superstep */
    if (b->state == dan_pmsg_receiving)
        while (!dan_mpi_ibarrier_done(&(b->ibarrier)));
    free_all_receivers(&(b->tree));
    dan_mpi_free(&(b->received));
    b->state = dan_pmsg_sending;
}

