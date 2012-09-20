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

#ifndef DAN_VECTOR_H
#define DAN_VECTOR_H

#include "dan_scalar.h"

#define DAN_2VECTOR_ZERO { DAN_SCALAR_ZERO, DAN_SCALAR_ZERO }
#define DAN_2VECTOR_ONE  { DAN_SCALAR_ONE , DAN_SCALAR_ONE  }
#define DAN_FOR_2(i)  for(dan_natural i=0; i<2; ++i)
#define DAN_FOR_3(i)  for(dan_natural i=0; i<3; ++i)

typedef dan_scalar dan_2vector[2];
typedef dan_scalar dan_3vector[3];

void dan_copy_2vector(const dan_2vector v, dan_2vector r);
void dan_copy_3vector(const dan_3vector v, dan_3vector r);
void dan_scale_2vector(dan_scalar a, const dan_2vector v, dan_2vector r);
void dan_scale_3vector(dan_scalar a, const dan_3vector v, dan_3vector r);
void dan_add_2vectors(const dan_2vector v1, const dan_2vector v2, dan_2vector r);
void dan_add_3vectors(const dan_3vector v1, const dan_3vector v2, dan_3vector r);
dan_scalar dan_dot_2vectors(const dan_2vector v1, const dan_2vector v2);
dan_scalar dan_dot_3vectors(const dan_2vector v1, const dan_2vector v2);
void dan_cross_3vectors(const dan_3vector v1, const dan_3vector v2, dan_3vector r);
dan_scalar dan_dot_2vector(const dan_2vector v);
dan_scalar dan_dot_3vector(const dan_3vector v);
dan_scalar dan_magnitude_2vector(const dan_2vector v);
dan_scalar dan_magnitude_3vector(const dan_3vector v);
void dan_unit_2vector(const dan_2vector v, dan_2vector r);
void dan_unit_3vector(const dan_3vector v, dan_3vector r);
void dan_divide_2vectors(const dan_2vector v1, const dan_2vector v2, dan_2vector r);
void dan_divide_3vectors(const dan_2vector v1, const dan_2vector v2, dan_2vector r);
void dan_normal_2vector(const dan_2vector v, dan_2vector r);

#endif
