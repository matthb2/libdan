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

#ifndef DAN_LINE_H
#define DAN_LINE_H

#include "dan_point.h"
#include "dan_vector.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    dan_scalar  radius;
    dan_2vector normal;
} dan_2line;

typedef struct
{
    dan_2point start;
    dan_2point end;
} dan_2line_segment;

void dan_2line_from_2points(const dan_2point p1, const dan_2point p2, dan_2line* r);
void dan_equidistant_2line(const dan_2point p1, const dan_2point p2, dan_2line* r);
void dan_2line_intersection(const dan_2line* l1, const dan_2line* l2, dan_2point r);

#ifdef __cplusplus
} //extern "C"
#endif

#endif
