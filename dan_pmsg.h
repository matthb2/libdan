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

#ifndef DAN_PMSG_H
#define DAN_PMSG_H

#include "dan_mpi.h"
#include "dan_aa.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DAN_PMSG_USER_TAG 0
#define DAN_PMSG_SYNC_TAG 1

typedef struct
{
    dan_aa_node node;
    dan_mpi_message message;
    bool received_from;
} dan_pmsg_peer;

#define DAN_PMSG_RECEIVER_INIT \
{ .message = DAN_MPI_MESSAGE_INIT, .received_from = false }

typedef struct
{
    dan_aa_tree peers;
    dan_mpi_message received;
    dan_mpi_ibarrier ibarrier;
    enum { dan_pmsg_sending, dan_pmsg_receiving } state;
    enum { dan_pmsg_global, dan_pmsg_local } method;
} dan_pmsg;

#define DAN_PMSG_INIT \
{ .tag = 0, .peers = DAN_AA_TREE_INIT, .received = DAN_MPI_MESSAGE_INIT, \
  .ibarrier = DAN_MPI_IBARRIER_INIT, .state = dan_pmsg_sending }

void dan_pmsg_init(dan_pmsg* b);
void dan_pmsg_start(dan_pmsg* b);
void dan_pmsg_reserve(dan_pmsg* b, int peer, size_t bytes);
bool dan_pmsg_has_peer(dan_pmsg* b, int peer);
size_t dan_pmsg_reserved(dan_pmsg* b, int peer);
void dan_pmsg_allocate(dan_pmsg* b);
void* dan_pmsg_pack(dan_pmsg* b, int peer, size_t bytes);
void dan_pmsg_send(dan_pmsg* b);
bool dan_pmsg_receive(dan_pmsg* b);
void dan_pmsg_free(dan_pmsg* b);

#ifdef __cplusplus
} //extern "C"
#endif

#endif //DAN_PMSG_H
