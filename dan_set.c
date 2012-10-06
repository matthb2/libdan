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

#include "dan_set.h"
#include "dan_memory.h"

dan_set* dan_make_set(size_t count)
{
    dan_set* set = dan_malloc(sizeof(dan_set)+count*sizeof(void*));
    set->count = count;
    return set;
}

dan_set* dan_set_union(dan_set* a, dan_set* b)
{
    dan_set* a_minus_b = dan_set_difference(a,b);
    dan_set* result = dan_set_sum(a_minus_b,b);
    dan_set_free(a_minus_b);
    return result;
}

dan_set* dan_set_intersection(dan_set* a, dan_set* b)
{
    size_t size=0;
    for (size_t i=0; i < a->count; ++i)
    {
        if (dan_set_has(b,a->objects[i]))
            ++size;
    }
    dan_set* result = dan_make_set(size);
    size = 0;
    for (size_t i=0; i < a->count; ++i)
    {
        if (dan_set_has(b,a->objects[i]))
        {
            result->objects[size] = a->objects[i];
            ++size;
        }
    }
    return result;
}

dan_set* dan_set_difference(dan_set* a, dan_set* b)
{
    size_t size=0;
    for (size_t i=0; i < a->count; ++i)
    {
        if (!dan_set_has(b,a->objects[i]))
            ++size;
    }
    dan_set* result = dan_make_set(size);
    size = 0;
    for (size_t i=0; i < a->count; ++i)
    {
        if (!dan_set_has(b,a->objects[i]))
        {
            result->objects[size] = a->objects[i];
            ++size;
        }
    }
    return result;
}

dan_set* dan_set_sum(dan_set* a, dan_set* b)
{
    dan_set* result = dan_make_set(a->count + b->count);
    for (size_t i=0; i < a->count; ++i)
        result->objects[i] = a->objects[i];
    for (size_t i=0; i < b->count; ++i)
        result->objects[i+a->count] = b->objects[i];
    return result;
}

bool dan_set_has(dan_set* s, void* object)
{
    for (size_t i=0; i < s->count; ++i)
    {
        if (s->objects[i] == object)
            return true;
    }
    return false;
}

void dan_set_free(dan_set* s)
{
    dan_free(s);
}

