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

int  dan_mpi_size()
{
    int size;
    int result = MPI_Comm_size(MPI_COMM_WORLD,&size);
    if (result != MPI_SUCCESS)
    {
        fprintf(stderr,"libdan failed using MPI_Comm_size\n");
        exit(EXIT_SUCCESS);
    }
    return size;
}

int  dan_mpi_rank()
{
    int rank;
    int result = MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    if (result != MPI_SUCCESS)
    {
        fprintf(stderr,"libdan failed using MPI_Comm_rank\n");
        exit(EXIT_SUCCESS);
    }
    return rank;
}

void dan_mpi_send(dan_mpi_message* m, int to, int tag)
{
    int result;
    result = MPI_Issend(
            m->buffer.data,
            m->buffer.size,
            MPI_BYTE,
            to,
            tag,
            MPI_COMM_WORLD,
            &(m->request));
    if (result != MPI_SUCCESS)
    {
        fprintf(stderr,"libdan failed using MPI_Issend\n");
        exit(EXIT_FAILURE);
    }
}

bool dan_mpi_done(dan_mpi_message* m)
{
    int flag;
    int result;
    result = MPI_Test(&(m->request),&flag,MPI_STATUS_IGNORE);
    if (result != MPI_SUCCESS)
    {
        fprintf(stderr,"libdan failed using MPI_Test\n");
        exit(EXIT_FAILURE);
    }
    return flag;
}

bool dan_mpi_try_receiving(dan_mpi_message* m, int from, int tag)
{
    MPI_Status status;
    int flag;
    int result;
    result = MPI_Iprobe(from,tag,MPI_COMM_WORLD,&flag,&status);
    if (result != MPI_SUCCESS)
    {
        fprintf(stderr,"libdan failed using MPI_Iprobe\n");
        exit(EXIT_FAILURE);
    }
    if (!flag)
        return false;
    int count;
    result = MPI_Get_count(&status,MPI_BYTE,&count);
    if (result != MPI_SUCCESS)
    {
        fprintf(stderr,"libdan failed using MPI_Get_count\n");
        exit(EXIT_FAILURE);
    }
    dan_buffer_realloc(&(m->buffer),count);
    result = MPI_Recv(
            m->buffer.data,
            m->buffer.size,
            MPI_BYTE,
            from,
            tag,
            MPI_COMM_WORLD,
            MPI_STATUS_IGNORE);
    if (result != MPI_SUCCESS)
    {
        fprintf(stderr,"libdan failed using MPI_Recv\n");
        exit(EXIT_FAILURE);
    }
    return true;
}

int dan_mpi_unique_tag(void)
{
    static int global_tag = 0;
    return global_tag++;
}

void dan_mpi_begin_ibarrier(dan_mpi_ibarrier* i, int tag)
{
    static char message = 'i';
    i->tag = tag;
    i->phase = 0;
    i->message.buffer.data = &message;
    i->message.buffer.size = sizeof(message);
    i->message.peer = 1^dan_mpi_rank();
    if (i->message.peer < dan_mpi_size())
        dan_mpi_send(&(i->message),i->message.peer,tag);
}

static bool ibarrier_down(int bit, int size, dan_mpi_ibarrier* i)
{
    bit >>= 1;
    if (!bit)
        return true;
    if (i->message.peer < size)
        dan_mpi_send(&(i->message),i->message.peer,i->tag);
    return false;
}

bool dan_mpi_ibarrier_done(dan_mpi_ibarrier* i)
{
    int rank = dan_mpi_rank();
    int size = dan_mpi_size();
    int bit = i->message.peer ^ rank;
    if (!bit) //handles calls that happen after the first 'return true'
        return true;
    if (bit >= size) //rank 0 goes from phase 0 to phase 1 here (it has no 1 bits)
    {
        bit >>= 1;
        i->phase = 1;
        return !bit; //return immediately in case size==1
    }
    if (bit&rank) //turning point from phase 0 to phase 1
    {
        if (i->phase == 0)
        { //wait for the send at the end of phase 0 to be done
            if ((i->message.peer >= size)||dan_mpi_done(&(i->message)))
                i->phase = 1; //now its phase 1
        }
        else
        { //wait for the incoming message to begin phase1
            if ((i->message.peer >= size)
                ||dan_mpi_try_receiving(&(i->message),i->message.peer,i->tag))
                return ibarrier_down(bit,size,i); //start going down, sending
        }
    }
    else
    {
        if (i->phase == 0)
        { //wait for message from the sender for this bit
            if ((i->message.peer >= size)
                ||(dan_mpi_try_receiving(&(i->message),i->message.peer,i->tag)))
            {
                bit <<= 1; //shift bit up
                i->message.peer = bit^rank;
                //if we are the sender for this bit, send
                if ((i->message.peer < size)&&(bit&rank))
                    dan_mpi_send(&(i->message),i->message.peer,i->tag);
            }
        }
        else
        { //wait for one down send to be done, then move down and send again
            if ((i->message.peer >= size)||dan_mpi_done(&(i->message)))
                return ibarrier_down(bit,size,i);
        }
    }
    return false;
}

