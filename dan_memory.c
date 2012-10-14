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

#include <stdio.h>
#include <stdlib.h>
#include "dan_memory.h"

void* dan_malloc(size_t size)
{
    if (!size)
        return 0;
    void* p = malloc(size);
    if (p == NULL)
    {
        fprintf(stderr,"libdan ran out of memory using malloc\n");
        exit(EXIT_FAILURE);
    }
    return p;
}

void* dan_realloc(void* p, size_t size)
{
    if ((p==NULL)&&(size==0))
        return NULL;
    p = realloc(p,size);
    if ((p==NULL)&&(size!=0))
    {
        fprintf(stderr,"libdan ran out of memory using realloc\n");
        exit(EXIT_FAILURE);
    }
    return p;
}

void dan_free(void* p)
{
    if (p==NULL)
    {
        fprintf(stderr,"libdan is trying to free NULL\n");
        exit(EXIT_FAILURE);
    }
    free(p);
}

void* dan_pointer_add(void* p, ptrdiff_t bytes)
{
    return ((char*)p)+bytes;
}

void dan_move_pointer(void** p, ptrdiff_t bytes)
{
    *p = dan_pointer_add(*p,bytes);
}

void dan_reserve_buffer(dan_buffer* b, size_t size)
{
    b->size += size;
}

void dan_allocate_buffer(dan_buffer* b)
{
    b->data = dan_realloc(b->data,b->size);
}

void* dan_buffer_start(dan_buffer* b)
{
    return b->data;
}

void* dan_buffer_end(dan_buffer* b)
{
    return b->data + b->size;
}

void dan_free_buffer(dan_buffer* b)
{
    b->size = 0;
    dan_allocate_buffer(b);
}

void dan_buffer_realloc(dan_buffer* b, size_t size)
{
    if (b->size == size)
    {
        return;
    }
    b->data = dan_realloc(b->data,size);
    b->size = size;
}

void dan_reserve_array(dan_array* a, size_t count, size_t element_size)
{
    dan_reserve_buffer(a,count*element_size);
}

void dan_free_buffer2(dan_buffer2* b)
{
    dan_buffer2 temp = DAN_BUFFER2_INIT;
    dan_free(b->start);
    *b = temp;
}

#define INITIAL_CAPACITY 8

void* dan_push_buffer2(dan_buffer2* b, size_t size)
{
    b->size += size;
    if (b->size > b->capacity)
    {
        b->capacity *= 2;
        if (!(b->capacity))
            b->capacity = INITIAL_CAPACITY;
        b->start = dan_realloc(b->start,b->capacity);
    }
    return b->start + b->size - size;
}

void dan_begin_buffer2(dan_buffer2* b)
{
    b->size = 0;
}

void* dan_walk_buffer2(dan_buffer2* b, size_t size)
{
    void* at = b->start + b->size;
    b->size += size;
    return at;
}

