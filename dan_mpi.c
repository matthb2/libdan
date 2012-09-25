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
#include "dan_mpi.h"

void dan_mpi_send(dan_mpi* i, dan_mpi_message* m, int to)
{
    int result;
    result = MPI_Issend(
            m->buffer.data,
            m->buffer.size,
            MPI_BYTE,
            to,
            i->tag,
            i->communicator,
            &(m->request));
    if (result != MPI_SUCCESS)
    {
        fprintf(stderr,"libdan failed to MPI_Issend\n");
        exit(EXIT_FAILURE);
    }
}

bool dan_mpi_received(dan_mpi_message* m)
{
    int flag;
    int result;
    result = MPI_Test(&(m->request),&flag,MPI_STATUS_IGNORE);
    if (result != MPI_SUCCESS)
    {
        fprintf(stderr,"libdan failed to MPI_Test\n");
        exit(EXIT_FAILURE);
    }
    return flag;
}

bool dan_mpi_try_receiving(dan_mpi* i, dan_mpi_message* m, int from)
{
    MPI_Status status;
    int flag;
    int result;
    result = MPI_Iprobe(from,i->tag,i->communicator,&flag,&status);
    if (result != MPI_SUCCESS)
    {
        fprintf(stderr,"libdan failed to MPI_Iprobe\n");
        exit(EXIT_FAILURE);
    }
    if (!flag)
        return false;
    int count;
    result = MPI_Get_count(&status,MPI_BYTE,&count);
    if (result != MPI_SUCCESS)
    {
        fprintf(stderr,"libdan failed to MPI_Get_count\n");
        exit(EXIT_FAILURE);
    }
    dan_buffer_realloc(&(m->buffer),count);
    result = MPI_Recv(
            m->buffer.data,
            m->buffer.size,
            MPI_BYTE,
            from,
            i->tag,
            i->communicator,
            MPI_STATUS_IGNORE);
    if (result != MPI_SUCCESS)
    {
        fprintf(stderr,"libdan failed to MPI_Recv\n");
        exit(EXIT_FAILURE);
    }
    return true;
}

void dan_mpi_unique(dan_mpi* i)
{
    static int global_tag = 0;
    i->tag = global_tag++;
    i->communicator = MPI_COMM_WORLD;
}

