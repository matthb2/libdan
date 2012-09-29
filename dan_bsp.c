
#include <stdlib.h>
#include <stdio.h>
#include "dan_bsp.h"

enum { sending, receiving };

void dan_bsp_init(dan_bsp* b, int tag, int ibarrier_tag)
{
    if (tag == ibarrier_tag)
    {
        fprintf(stderr,"dan_bsp_use_ibarrier: the ibarrier tag must be different from the bsp tag\n");
        exit(0);
    }
    b->tag = tag;
    b->ibarrier->tag = ibarrier_tag;
}

static void free_all_takers(dan_aa_tree& t)
{
    if (**t == &dan_aa_bottom)
        return;
    free_all_takers(&((*t)->left));
    free_all_takers(&((*t)->right));
    dan_free(*t);
    **t = &dan_aa_bottom;
}

void dan_bsp_begin_superstep(dan_bsp* b)
{
    /* ensures no one starts a new superstep while others are receiving
       in the past superstep */
    if (b->state == receiving)
        while (!dan_mpi_ibarrier_done(&(b->ibarrier)));
    free_all_takers(&(b->tree));
    b->state = sending;
}

static bool receiver_less(dan_aa_node* a, dan_aa_node* b)
{
    return ((dan_bsp_receiver*)a)->message.peer
         < ((dan_bsp_receiver*)b)->message.peer;
}

static dan_bsp_receiver* find_receiver(dan_aa_tree t, int peer)
{
    dan_bsp_receiver key;
    key->message.peer = peer;
    return (dan_bsp_receiver*) dan_aa_find((dan_aa_node*)&key,t,receiver_less);
}

void dan_bsp_reserve(dan_bsp* b, int peer, size_t bytes)
{
    dan_bsp_receiver* receiver;
    receiver = find_receiver(b->tree,peer);
    if (!receiver)
    {
        receiver = malloc(sizeof(*receiver));
        dan_aa_insert((dan_aa_node*)receiver,&(b->tree),receiver_less);
    }
    receiver->message.buffer.size += bytes;
}

static void allocate(dan_aa_tree t)
{
}

void dan_bsp_allocate(dan_bsp* b)
{
}

