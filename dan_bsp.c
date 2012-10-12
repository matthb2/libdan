/*
    Copyright 2012 Dan Ibanez
    
    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at
    
    http://www.apache.org/licenses/LICENSE-2.0
    
    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#include <stdlib.h>
#include <stdio.h>
#include "dan_bsp.h"

void dan_bsp_init(dan_bsp* b, int tag, int ibarrier_tag)
{
    dan_bsp temp = DAN_BSP_INIT;
    *b = temp;
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

dan_bsp_receiver* dan_bsp_find_receiver(dan_aa_tree t, int peer)
{
    dan_bsp_receiver key;
    key.message.peer = peer;
    return (dan_bsp_receiver*) dan_aa_find((dan_aa_node*)&key,t,receiver_less);
}

void dan_bsp_reserve(dan_bsp* b, int peer, size_t bytes)
{
    dan_bsp_receiver* receiver;
    receiver = dan_bsp_find_receiver(b->tree,peer);
    if (!receiver)
    {
        receiver = dan_malloc(sizeof(*receiver));
        dan_bsp_receiver temp = DAN_BSP_RECEIVER_INIT;
        *receiver = temp;
        receiver->message.peer = peer;
        dan_aa_insert((dan_aa_node*)receiver,&(b->tree),receiver_less);
    }
    dan_mpi_reserve(&(receiver->message),bytes);
}

bool dan_bsp_has_receiver(dan_bsp* b, int peer)
{
    return dan_bsp_find_receiver(b->tree,peer);
}

size_t dan_bsp_reserved(dan_bsp* b, int peer)
{
    dan_bsp_receiver* receiver;
    receiver = dan_bsp_find_receiver(b->tree,peer);
    if (!receiver)
    {
        fprintf(stderr,"dan_bsp_reserved: there is no receiver for this peer id. use dan_bsp_reserve to allocate a receiver with enough space first.\n");
        exit(EXIT_FAILURE);
    }
    return dan_mpi_reserved(&(receiver->message));
}

static void begin_packing(dan_aa_tree t)
{
    if (t == &dan_aa_bottom)
        return;
    begin_packing(t->left);
    dan_bsp_receiver* receiver;
    receiver = (dan_bsp_receiver*) t;
    dan_mpi_begin_packing(&(receiver->message));
    begin_packing(t->right);
}

void dan_bsp_begin_packing(dan_bsp* b)
{
    begin_packing(b->tree);
}

void* dan_bsp_pack(dan_bsp* b, int peer, size_t bytes)
{
    dan_bsp_receiver* receiver;
    receiver = dan_bsp_find_receiver(b->tree,peer);
    if (!receiver)
    {
        fprintf(stderr,"dan_bsp_pack: there is no receiver for this peer id. use dan_bsp_reserve to allocate a receiver with enough space first.\n");
        exit(EXIT_FAILURE);
    }
    return dan_mpi_pack(&(receiver->message),bytes);
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
    b->received.peer = MPI_ANY_SOURCE;
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
        {
            dan_mpi_begin_ibarrier(&(b->ibarrier),b->ibarrier.tag);
            return false;
        }
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
    b->state = dan_bsp_sending;
}

