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


#include "dan_matrix.h"

void dan_multiply_2matrix_2vector(const dan_2matrix m, const dan_2vector v, dan_3vector r)
{
    DAN_FOR_2(i)
        r[i] = dan_dot_2vectors(m[i],v);
}

void dan_multiply_3matrix_3vector(const dan_3matrix m, const dan_3vector v, dan_3vector r)
{
    DAN_FOR_3(i)
        r[i] = dan_dot_3vectors(m[i],v);
}

void dan_cross_3vector(const dan_3vector v, dan_3matrix r)
{
    r[0][0] = 0.0;
    r[0][1] = -v[2];
    r[0][2] = v[1];
    r[1][0] = v[2];
    r[1][1] = 0.0;
    r[1][2] = -v[0];
    r[2][0] = -v[1];
    r[2][1] = v[0];
    r[2][2] = 0.0;
}

