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
#include "dan.h"
#include "dan_mpi.h"
#include "dan_memory.h"

int  dan_mpi_size(void)
{
    int size;
    int result = MPI_Comm_size(MPI_COMM_WORLD,&size);
    DAN_FAIL_IF(result != MPI_SUCCESS,"MPI_Comm_size failed")
    return size;
}

int  dan_mpi_rank(void)
{
    int rank;
    int result = MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    DAN_FAIL_IF(result != MPI_SUCCESS,"MPI_Comm_rank failed")
    return rank;
}

void dan_mpi_send(dan_mpi_message* m, int tag)
{
    int result;
    result = MPI_Issend(
            m->buffer.data,
            m->buffer.size,
            MPI_BYTE,
            m->peer,
            tag,
            MPI_COMM_WORLD,
            &(m->request));
    DAN_FAIL_IF(result != MPI_SUCCESS,"MPI_Issend failed")
}

bool dan_mpi_done(dan_mpi_message* m)
{
    int flag;
    int result;
    result = MPI_Test(&(m->request),&flag,MPI_STATUS_IGNORE);
    DAN_FAIL_IF(result != MPI_SUCCESS,"MPI_Test failed")
    return flag;
}

bool dan_mpi_receive(dan_mpi_message* m, int tag)
{
    MPI_Status status;
    int flag;
    int result;
    result = MPI_Iprobe(m->peer,tag,MPI_COMM_WORLD,&flag,&status);
    DAN_FAIL_IF(result != MPI_SUCCESS,"MPI_Iprobe failed")
    if (!flag)
        return false;
    m->peer = status.MPI_SOURCE;
    int count;
    result = MPI_Get_count(&status,MPI_BYTE,&count);
    DAN_FAIL_IF(result != MPI_SUCCESS,"MPI_Get_count failed")
    dan_buffer_realloc(&(m->buffer),count);
    result = MPI_Recv(
            m->buffer.data,
            m->buffer.size,
            MPI_BYTE,
            m->peer,
            tag,
            MPI_COMM_WORLD,
            MPI_STATUS_IGNORE);
    DAN_FAIL_IF(result != MPI_SUCCESS,"MPI_Recv failed")
    return true;
}

void dan_mpi_reserve(dan_mpi_message* m, size_t bytes)
{
    m->buffer.size += bytes;
}

size_t dan_mpi_reserved(dan_mpi_message* m)
{
    return m->buffer.size;
}

void dan_mpi_begin_packing(dan_mpi_message* m)
{
    size_t size = m->buffer.size;
    m->buffer.size = 0;
    dan_buffer_realloc(&(m->buffer),size);
    m->buffer.size = 0;
}

void* dan_mpi_pack(dan_mpi_message* m, size_t bytes)
{
    void* at = dan_pointer_add(m->buffer.data,m->buffer.size);
    m->buffer.size += bytes;
    return at;
}

static bool ibarrier_sending(dan_mpi_ibarrier* i)
{
    int rank = dan_mpi_rank();
    int bit = i->message.peer^rank;
    int send = (bit&rank)?1:0;
    send = i->phase^send;
    return send;
}

static void start_ibarrier_step(dan_mpi_ibarrier* i)
{
    if (i->message.peer >= dan_mpi_size())
        return;
    if (ibarrier_sending(i))
        dan_mpi_send(&(i->message),i->tag);
}

void dan_mpi_begin_ibarrier(dan_mpi_ibarrier* i, int tag)
{
    i->tag = tag;
    i->phase = 0;
    i->message.buffer.data = 0;
    i->message.buffer.size = 0;
    i->message.peer = 1^dan_mpi_rank();
    start_ibarrier_step(i);
}

static bool ibarrier_step_done(dan_mpi_ibarrier* i)
{
    if (i->message.peer >= dan_mpi_size())
        return true;
    if (ibarrier_sending(i))
        return dan_mpi_done(&(i->message));
    else
        return dan_mpi_receive(&(i->message),i->tag);
}

static void ibarrier_shift(dan_mpi_ibarrier* i)
{
    int rank = dan_mpi_rank();
    int bit = i->message.peer^rank;
    if (i->phase == 0)
        bit <<= 1;
    else
        bit >>= 1;
    i->message.peer = bit^rank;
}

bool dan_mpi_ibarrier_done(dan_mpi_ibarrier* i)
{
    if (!ibarrier_step_done(i))
        return false;
    int rank = dan_mpi_rank();
    int bit = i->message.peer^rank;
    if ((i->phase == 0)&&((bit&rank)||((rank == 0)&&(i->message.peer >= dan_mpi_size()))))
        i->phase = 1;
    else
    {
        ibarrier_shift(i);
        if (i->message.peer == rank)
            return true;
    }
    start_ibarrier_step(i);
    return false;
}

void dan_mpi_free(dan_mpi_message* m)
{
    dan_buffer_realloc(&(m->buffer),0);
}

void dan_send(dan_message* m, int tag)
{
    int result;
    result = MPI_Issend(
            m->buffer.start,
            m->buffer.size,
            MPI_BYTE,
            m->peer,
            tag,
            MPI_COMM_WORLD,
            &(m->request));
    DAN_FAIL_IF(result != MPI_SUCCESS,"MPI_Issend failed")
}

bool dan_done(dan_message* m)
{
    int flag;
    int result;
    result = MPI_Test(&(m->request),&flag,MPI_STATUS_IGNORE);
    DAN_FAIL_IF(result != MPI_SUCCESS,"MPI_Test failed")
    return flag;
}

bool dan_receive(dan_message* m, int tag)
{
    MPI_Status status;
    int flag;
    int result;
    result = MPI_Iprobe(m->peer,tag,MPI_COMM_WORLD,&flag,&status);
    DAN_FAIL_IF(result != MPI_SUCCESS,"MPI_Iprobe failed")
    if (!flag)
        return false;
    m->peer = status.MPI_SOURCE;
    int count;
    result = MPI_Get_count(&status,MPI_BYTE,&count);
    DAN_FAIL_IF(result != MPI_SUCCESS,"MPI_Get_count failed")
    dan_resize_buffer2(&(m->buffer),count);
    result = MPI_Recv(
            m->buffer.start,
            m->buffer.size,
            MPI_BYTE,
            m->peer,
            tag,
            MPI_COMM_WORLD,
            MPI_STATUS_IGNORE);
    DAN_FAIL_IF(result != MPI_SUCCESS,"MPI_Recv failed")
    return true;
}

void dan_free_message(dan_message* m)
{
    dan_free_buffer2(&(m->buffer));
}

