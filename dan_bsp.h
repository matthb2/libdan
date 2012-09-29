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

#ifndef DAN_BSP_H
#define DAN_BSP_H

#include "dan_mpi.h"
#include "dan_aa.h"

typedef struct
{
    dan_aa_node node;
    dan_mpi_message message;
} dan_bsp_receiver;

#define DAN_BSP_TAKER_INIT \
{ .message = DAN_MPI_MESSAGE_INIT }

typedef struct
{
    int tag;
    dan_aa_tree tree;
    dan_mpi_message received;
    dan_mpi_ibarrier ibarrier;
    enum { dan_bsp_sending, dan_bsp_receiving } state;
} dan_bsp;

#define DAN_BSP_INIT \
{ .tag = 0, .tree = DAN_AA_TREE_INIT, .received = DAN_MPI_MESSAGE_INIT, \
  .ibarrier = DAN_MPI_IBARRIER_INIT, .state = dan_bsp_sending }

void dan_bsp_init(dan_bsp* b, int tag, int ibarrier_tag);
void dan_bsp_begin_superstep(dan_bsp* b);
void dan_bsp_reserve(dan_bsp* b, int peer, size_t bytes);
void dan_bsp_allocate(dan_bsp* b);
void dan_bsp_begin_packing(dan_bsp* b);
void* dan_bsp_pack(dan_bsp* b, int peer, size_t bytes);
void dan_bsp_send(dan_bsp* b);
bool dan_bsp_receive(dan_bsp* b);

#endif
