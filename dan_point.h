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

#ifndef DAN_POINT_H
#define DAN_POINT_H

#include "dan_vector.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef dan_2vector dan_2point;
typedef dan_3vector dan_3point;

void dan_copy_2point(const dan_2point p, dan_2point r);
void dan_subtract_2points(const dan_2point p1, const dan_2point p2, dan_2vector r);
void dan_middle_2point(const dan_2point p1, const dan_2point p2, dan_2point r);

#ifdef __cplusplus
} // extern "C"
#endif

#endif
