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


#include <math.h>
#include "dan_vector.h"
#include "dan_matrix.h"

void dan_scale_2vector(dan_scalar a, const dan_2vector v, dan_2vector r)
{
    DAN_FOR_2(i)
        r[i] = a*v[i];
}

void dan_scale_3vector(dan_scalar a, const dan_3vector v, dan_3vector r)
{
    DAN_FOR_3(i)
        r[i] = a*v[i];
}

void dan_add_2vectors(const dan_2vector v1, const dan_2vector v2, dan_2vector r)
{
    DAN_FOR_2(i)
        r[i] = v1[i] + v2[i];
}

void dan_add_3vectors(const dan_3vector v1, const dan_3vector v2, dan_3vector r)
{
    DAN_FOR_3(i)
        r[i] = v1[i] + v2[i];
}

dan_scalar dan_dot_2vectors(const dan_2vector v1, const dan_2vector v2)
{
    dan_scalar r = 0.0;
    DAN_FOR_2(i)
        r += v1[i]*v2[i];
    return r;
}

dan_scalar dan_dot_3vectors(const dan_2vector v1, const dan_2vector v2)
{
    dan_scalar r = 0.0;
    DAN_FOR_3(i)
        r += v1[i]*v2[i];
    return r;
}

void dan_cross_3vectors(const dan_3vector v1, const dan_3vector v2, dan_3vector r)
{
    dan_3matrix v1_cross;
    dan_cross_3vector(v1,v1_cross);
    dan_multiply_3matrix_3vector(v1_cross,v2,r);
}

dan_scalar dan_dot_2vector(const dan_2vector v)
{
    return dan_dot_2vectors(v,v);
}

dan_scalar dan_dot_3vector(const dan_3vector v)
{
    return dan_dot_3vectors(v,v);
}

dan_scalar dan_magnitude_2vector(const dan_2vector v)
{
    return sqrt(dan_dot_2vector(v));
}

dan_scalar dan_magnitude_3vector(const dan_3vector v)
{
    return sqrt(dan_dot_3vector(v));
}

void dan_normalize_2vector(const dan_2vector v, dan_2vector r)
{
    dan_scale_2vector(1.0/dan_magnitude_2vector(v),v,r);
}

void dan_normalize_3vector(const dan_3vector v, dan_3vector r)
{
    dan_scale_3vector(1.0/dan_magnitude_3vector(v),v,r);
}

void dan_divide_2vectors(const dan_2vector v1, const dan_2vector v2, dan_2vector r)
{
    DAN_FOR_2(i)
        r[i] = v1[i]/v2[i];
}

void dan_divide_3vectors(const dan_3vector v1, const dan_3vector v2, dan_3vector r)
{
    DAN_FOR_3(i)
        r[i] = v1[i]/v2[i];
}

