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

#ifndef DAN_NEIGHBOR_H
#define DAN_NEIGHBOR_H

#include "dan_mpi.h"

typedef struct
{
    dan_mpi_message message;
    bool received;
} dan_neighbor;

#define DAN_NEIGHBOR_INIT \
{ .message = DAN_MPI_MESSAGE_INIT, .received = false }

typedef struct
{
    int tag;
    dan_mpi_message received;
    size_t count;
    dan_neighbor neighbors[];
} dan_neighbors;

#define DAN_NEIGHBORS_INIT \
{ .tag = 0, .received = DAN_MPI_MESSAGE_INIT, .count = 0 }

dan_neighbors* dan_make_neighbors(int tag, size_t count, int* peers);
void dan_neighbor_restart(dan_neighbors* n);
void dan_neighbor_reserve(dan_neighbors* n, int peer, size_t bytes);
void dan_neighbor_begin_packing(dan_neighbors* n);
void* dan_neighbor_pack(dan_neighbors* n, int peer, size_t bytes);
void dan_neighbor_send(dan_neighbors* n);
bool dan_neighbor_receive(dan_neighbors* n);
void dan_free_neighbors(dan_neighbors* n);

dan_neighbor* dan_find_neighbor(dan_neighbors* n, int peer);

#endif
