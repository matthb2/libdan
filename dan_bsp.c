
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
    dan_aa_init(&(b->tree));
    b->message = DAN_MPI_MESSAGE_INIT;
    b->ibarrier->tag = ibarrier_tag;
    b->state = sending;
}

static void free_all_takers(dan_aa_tree& t)
{
}

void dan_bsp_begin_superstep(dan_bsp* b)
{
    /* ensures no one starts a new superstep while others are receiving
       in the past superstep */
    if (b->state == receiving)
        while (!dan_mpi_ibarrier_done(&(b->ibarrier)));
    free_all_takers(&(b->tree));
}

static int int_compare(const void* a, const void* b)
{
    return *((int*)a) - *((int*)b);
}

void dan_bsp_use_givers(dan_bsp* b, size_t count, int* givers)
{
    if (b->sync_type == dan_use_ibarrier)
    {
        dan_free(b->sync.ibarrier);
    }
    else if (b->sync_type == dan_use_givers)
    {
        dan_free(b->sync.givers);
    }
    b->sync.givers = dan_malloc(sizeof(dan_bsp_givers)+count*sizeof(int));
    b->sync.givers->count = count;
    for (size_t i=0; i < count; ++i)
    {
        b->sync.givers->givers[i] = givers[i];
    }
    qsort(b->sync.givers->givers,count,sizeof(int),int_compare);
    b->sync_type = dan_use_givers;
}

static bool taker_less(dan_aa_node* a, dan_aa_node* b)
{
    return ((dan_bsp_taker*)a)->message.peer < ((dan_bsp_taker*)b)->message.peer;
}

dan_bsp_taker* dan_bsp_find_taker(dan_bsp* b, int peer)
{
    dan_bsp_taker key;
    key->message.peer = peer;
    return (dan_bsp_taker*) dan_aa_find((dan_aa_node*)&key,b->tree,taker_less);
}

void dan_bsp_insert_taker(dan_bsp* b, int peer, dan_bsp_taker* result)
{
    result->message.peer = peer;
    dan_aa_node* inserted = dan_aa_insert((dan_aa_node*)result,b->tree,taker_less);
    if (inserted != (dan_aa_node*)result)
    {
        fprintf(stderr,"dan_bsp_insert_taker: there was already a taker with this peer id. use dan_bsp_find_taker to check for it first.\n");
        exit(0);
    }
}

void dan_bsp_add_taker(dan_bsp* b, int peer)
{
    dan_bsp_taker* taker = dan_bsp_find_taker(b,peer);
    if (taker)
        return;
    taker = dan_malloc(sizeof(*taker));
    *taker = DAN_BSP_TAKER_INIT;
    dan_bsp_insert_taker(b,peer,taker);
}

void dan_bsp_reserve(dan_bsp* b, int peer, size_t bytes)
{
    dan_bsp_taker* taker = dan_bsp_find_taker(b,peer);
    if (!taker)
    {
        fprintf(stderr,"dan_bsp_reserve: there is no taker with this peer id. use dan_bsp_add_taker to add it first.\n");
        exit(0);
    }
}

