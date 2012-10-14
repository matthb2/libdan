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

#define DAN_PMSG_TAG 0
#define DAN_PMSG_IBARRIER_TAG 1

typedef struct
{
    dan_aa_node node;
    dan_message message;
    bool received_from;
} dan_pmsg_peer;

#define DAN_PMSG_PEER_INIT \
{ .message = DAN_MESSAGE_INIT, .received_from = false }

typedef enum { dan_pmsg_global, dan_pmsg_local } dan_pmsg_method;

typedef struct
{
    dan_aa_tree peers;
    dan_message received;
    dan_mpi_ibarrier ibarrier;
    int state;
    dan_pmsg_method method;
} dan_pmsg;

#define DAN_PMSG_INIT \
{ .peers = DAN_AA_TREE_INIT, .received = DAN_MESSAGE_INIT, \
  .ibarrier = DAN_MPI_IBARRIER_INIT, .state = 0, .method = dan_pmsg_global }

void dan_pmsg_init(dan_pmsg* m);
void dan_pmsg_start(dan_pmsg* m, dan_pmsg_method method);
void* dan_pmsg_pack(dan_pmsg* m, int id, size_t size);
#define DAN_PMSG_PACK(m,id,object,type)\
(*((type*)dan_pmsg_pack(m,id,sizeof(object)))=object)
bool dan_pmsg_has_peer(dan_pmsg* m, int id);
size_t dan_pmsg_packed(dan_pmsg* m, int id);
void dan_pmsg_send(dan_pmsg* m);
bool dan_pmsg_receive(dan_pmsg* m);
void* dan_pmsg_unpack(dan_pmsg* m, size_t size);
#define DAN_PMSG_UNPACK(m,object,type) \
(object=*((type*)dan_pmsg_unpack(m,sizeof(object))))
int dan_pmsg_received_from(dan_pmsg* m);
size_t dan_pmsg_received_size(dan_pmsg* m);
void dan_pmsg_free(dan_pmsg* m);

#ifdef __cplusplus
} //extern "C"
#endif

#endif //DAN_PMSG_H
