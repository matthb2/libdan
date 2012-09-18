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

#include "dan_point.h"

void dan_copy_2point(const dan_2point p, dan_2point r)
{
    dan_copy_2vector(p,r);
}

void dan_subtract_2points(const dan_2point p1, const dan_2point p2, dan_2vector r)
{
    dan_2vector negative_p1;
    dan_scale_2vector(-1.0,p1,negative_p1);
    dan_add_2vectors(p2,negative_p1,r);
}

void dan_middle_2point(const dan_2point p1, const dan_2point p2, dan_2point r)
{
    dan_2vector sum;
    dan_add_2vectors(p1,p2,sum);
    dan_scale_2vector(1.0/2.0,sum,r);
}

