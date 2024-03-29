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

#ifndef DAN_MPI_H
#define DAN_MPI_H

#include "dan_memory.h"
#include <mpi.h>
#ifdef __cplusplus
extern "C" {
#else
#include <stdbool.h>
#endif

typedef struct
{
    dan_buffer buffer;
    int peer;
    MPI_Request request;
} dan_mpi_message;

#define DAN_MPI_MESSAGE_INIT \
{ .buffer = DAN_BUFFER_INIT, .peer = 0, .request = MPI_REQUEST_NULL }

typedef struct
{
    int tag;
    int phase;
    dan_mpi_message message;
} dan_mpi_ibarrier;

#define DAN_MPI_IBARRIER_INIT \
{ .tag = 0, .phase = 0, .message = DAN_MPI_MESSAGE_INIT }

int  dan_mpi_size(void);
int  dan_mpi_rank(void);
void dan_mpi_send(dan_mpi_message* m, int tag);
bool dan_mpi_done(dan_mpi_message* m);
bool dan_mpi_receive(dan_mpi_message* m, int tag);

void dan_mpi_reserve(dan_mpi_message* m, size_t bytes);
size_t dan_mpi_reserved(dan_mpi_message* m);
void dan_mpi_begin_packing(dan_mpi_message* m);
void* dan_mpi_pack(dan_mpi_message* m, size_t bytes);

void dan_mpi_begin_ibarrier(dan_mpi_ibarrier* i, int tag);
bool dan_mpi_ibarrier_done(dan_mpi_ibarrier* i);

void dan_mpi_free(dan_mpi_message* m);

typedef struct
{
    dan_buffer2 buffer;
    int peer;
    MPI_Request request;
} dan_message;
#define DAN_MESSAGE_INIT \
{ .buffer = DAN_BUFFER2_INIT, .peer = 0, .request = MPI_REQUEST_NULL }

void dan_send(dan_message* m, int tag);
bool dan_done(dan_message* m);
bool dan_receive(dan_message* m, int tag);
void dan_free_message(dan_message* m);

#ifdef __cplusplus
} //extern "C"
#endif

#endif
