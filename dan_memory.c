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

