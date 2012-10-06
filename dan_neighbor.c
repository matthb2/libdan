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
#include "dan_neighbor.h"

static int int_compare(const void* a, const void* b)
{
    return *((int*)a) - *((int*)b);
}

dan_neighbors* dan_make_neighbors(int tag, size_t count, int* peers)
{
    dan_neighbors* n = dan_malloc(sizeof(dan_neighbors)
                                  +count*sizeof(dan_neighbor));
    dan_neighbors temp_neighbors = DAN_NEIGHBORS_INIT;
    *n = temp_neighbors;
    n->tag = tag;
    n->count = count;
    qsort(peers,count,sizeof(int),int_compare);
    dan_neighbor temp_neighbor = DAN_NEIGHBOR_INIT;
    for (size_t i=0; i < count; ++i)
    {
        n->neighbors[i] = temp_neighbor;
        n->neighbors[i].message.peer = peers[i];
    }
    return n;
}

void dan_neighbor_restart(dan_neighbors* n)
{
    dan_free_neighbors(n);
    for (size_t i=0; i < n->count; ++i)
        n->neighbors[i].received = false;
}

static int neighbor_compare(const void* key, const void* n)
{
    return *((int*)key) - ((dan_neighbor*)n)->message.peer;
}

dan_neighbor* dan_find_neighbor(dan_neighbors* n, int peer)
{
    return bsearch(&peer,n->neighbors,n->count,
                   sizeof(dan_neighbor),neighbor_compare);
}

void dan_neighbor_reserve(dan_neighbors* n, int peer, size_t bytes)
{
    dan_neighbor* neighbor = dan_find_neighbor(n,peer);
    dan_mpi_reserve(&(neighbor->message),bytes);
}

void dan_neighbor_begin_packing(dan_neighbors* n)
{
    for (size_t i=0; i < n->count; ++i)
        dan_mpi_begin_packing(&(n->neighbors[i].message));
}

void* dan_neighbor_pack(dan_neighbors* n, int peer, size_t bytes)
{
    dan_neighbor* neighbor = dan_find_neighbor(n,peer);
    return dan_mpi_pack(&(neighbor->message),bytes);
}

void dan_neighbor_send(dan_neighbors* n)
{
    for (size_t i=0; i < n->count; ++i)
        dan_mpi_send(&(n->neighbors[i].message),n->tag);
}

static bool neighbors_done(dan_neighbors* n)
{
    for (size_t i=0; i < n->count; ++i)
    {
        dan_neighbor* neighbor = n->neighbors + i;
        if ((!dan_mpi_done(&(neighbor->message)))
          ||(!neighbor->received))
            return false;
    }
    return true;
}

bool dan_neighbor_receive(dan_neighbors* n)
{
    while (!dan_mpi_receive(&(n->received),n->tag));
    dan_neighbor* neighbor = dan_find_neighbor(n,n->received.peer);
    neighbor->received = true;
    return ! neighbors_done(n);
}

void dan_free_neighbors(dan_neighbors* n)
{
    for (size_t i=0; i < n->count; ++i)
        dan_mpi_free(&(n->neighbors[i].message));
}

