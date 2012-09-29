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

#endif
