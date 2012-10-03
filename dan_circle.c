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

#include "dan_circle.h"
#include "dan_line.h"

void dan_2circle_from_points(dan_2point points[3], dan_2circle* r)
{
    dan_2line radials[2];
    dan_equidistant_2line(points[0],points[1],radials+0);
    dan_equidistant_2line(points[1],points[2],radials+1);
    dan_2line_intersection(radials+0,radials+1,r->center);
    dan_2vector center_to_point;
    dan_subtract_2points(points[0],r->center,center_to_point);
    r->radius = dan_magnitude_2vector(center_to_point);
}

