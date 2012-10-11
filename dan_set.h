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

#ifndef DAN_SET_H
#define DAN_SET_H

#ifdef __cplusplus
#include <cstddef>
extern "C" {
#else
#include <stddef.h>
#include <stdbool.h>
#endif

typedef struct
{
    size_t count;
    void* objects[];
} dan_set;

dan_set* dan_make_set(size_t count);
dan_set* dan_set_union(dan_set* a, dan_set* b);
dan_set* dan_set_intersection(dan_set* a, dan_set* b);
dan_set* dan_set_difference(dan_set* a, dan_set* b);
dan_set* dan_set_sum(dan_set* a, dan_set* b);
bool dan_set_has(dan_set* s, void* object);
void dan_set_free(dan_set* s);

#ifdef __cplusplus
} //extern "C"
#endif

#endif
