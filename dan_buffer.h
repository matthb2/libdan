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

#ifndef DAN_BUFFER
#define DAN_BUFFER

#include <stddef.h>

typedef struct
{
    void*  data;
    size_t size;
} dan_buffer;

#define DAN_BUFFER_INIT { .data = 0, .size = 0 }

void dan_buffer_realloc(dan_buffer* b, size_t size);

#endif