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

#include "dan_natural.h"
#include "dan_line.h"

void dan_2line_intersection(const struct dan_2line* l1, const struct dan_2line* l2, dan_2point r)
{
    dan_2vector ratio;
    dan_divide_2vectors(l2->slope,l1->slope,ratio);
    DAN_FOR_2(i)
    {
        r[i]  =        l2->c - ratio[1-i]*(l1->c);
        r[i] /= l2->slope[i] - ratio[1-i]*(l1->slope[i]);
    }
}

