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

#ifndef DAN_CIRCLE_H
#define DAN_CIRCLE_H

#include "dan_point.h"

typedef struct
{
    dan_scalar radius;
    dan_2point center;
} dan_2circle;

void dan_2circle_from_points(dan_2point points[3], dan_2circle* r);

#endif
