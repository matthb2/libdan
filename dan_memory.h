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

#ifndef DAN_MEMORY_H
#define DAN_MEMORY_H

#include <stddef.h>

void* dan_malloc(size_t size);
void* dan_realloc(void* p, size_t size);
void dan_free(void* p);

void* dan_pointer_add(void* p, ptrdiff_t bytes);
void dan_move_pointer(void** p, ptrdiff_t bytes);

typedef struct
{
    void*  data;
    size_t size;
} dan_buffer;
#define DAN_BUFFER_INIT { .data = 0, .size = 0 }

void dan_reserve_buffer(dan_buffer* b, size_t size);
void dan_allocate_buffer(dan_buffer* b);
void* dan_buffer_start(dan_buffer* b);
void* dan_buffer_end(dan_buffer* b);
#define DAN_PACK_BUFFER(p,object,type)\
*((type*)p) = object;\
dan_move_pointer(&p,sizeof(type));
#define DAN_UNPACK_BUFFER(p,object,type)\
object = *((type*)p);\
dan_move_pointer(&p,sizeof(type));
void dan_free_buffer(dan_buffer* b);

void dan_buffer_realloc(dan_buffer* b, size_t size);

typedef dan_buffer dan_array;
void dan_reserve_array(dan_array* a, size_t count, size_t element_size);
#define DAN_RESERVE_ARRAY(a,count,type) dan_reserve_array(a,count,sizeof(type))
#define DAN_ARRAY_START(a,type) ((type*)dan_buffer_start(a))
#define DAN_ARRAY_END(a,type) ((type*)dan_buffer_end(a))

#endif
