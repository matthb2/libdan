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

void dan_2line_from_2points(const dan_2point p1, const dan_2point p2, dan_2line* r)
{
    dan_2vector p1p2;
    dan_subtract_2points(p2,p1,p1p2);
    dan_normal_2vector(p1p2,r->normal);
    dan_unit_2vector(r->normal,r->normal);
    r->radius = dan_dot_2vectors(r->normal,p1);
}

void dan_equidistant_2line(const dan_2point p1, const dan_2point p2, dan_2line* r)
{
    dan_2line between;
    dan_2line_from_2points(p1,p2,&between);
    dan_2point midpoint;
    dan_middle_2point(p1,p2,midpoint);
    dan_normal_2vector(between.normal,r->normal);
    r->radius = dan_dot_2vectors(r->normal,midpoint);
}

void dan_2line_intersection(const dan_2line* l1, const dan_2line* l2, dan_2point r)
{
    dan_2vector ratio;
    dan_divide_2vectors(l2->normal,l1->normal,ratio);
    DAN_FOR_2(i)
    {
        r[i]  = l2->radius    - ratio[1-i]*(l1->radius);
        r[i] /= l2->normal[i] - ratio[1-i]*(l1->normal[i]);
    }
}

