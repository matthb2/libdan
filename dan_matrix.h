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

#ifndef DAN_MATRIX_H
#define DAN_MATRIX_H

#include "dan_natural.h"
#include "dan_vector.h"

typedef dan_2vector dan_2matrix[2];
typedef dan_3vector dan_3matrix[3];

void dan_multiply_2matrix_2vector(const dan_2matrix m, const dan_2vector v, dan_2vector r);
void dan_multiply_3matrix_3vector(const dan_3matrix m, const dan_3vector v, dan_3vector r);
void dan_cross_3vector(const dan_3vector v, dan_3matrix r);

#endif
