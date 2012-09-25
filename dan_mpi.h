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

#include <stdbool.h>
#include <mpi.h>
#include "dan_buffer.h"

typedef struct
{
    int tag;
    MPI_Comm communicator;
} dan_mpi;

typedef struct
{
    dan_buffer buffer;
    int peer;
    MPI_Request request;
} dan_mpi_message;

#define DAN_MPI_MESSAGE_INIT \
{ .buffer = DAN_BUFFER_INIT, .peer = 0, .request = MPI_REQUEST_NULL }

void dan_mpi_send(dan_mpi* i, dan_mpi_message* m, int to);
bool dan_mpi_received(dan_mpi_message* m);
bool dan_mpi_try_receiving(dan_mpi* i, dan_mpi_message* m, int from);

/* returns ( global_tag++, MPI_COMM_WORLD ) */
void dan_mpi_unique(dan_mpi* i);

#endif
