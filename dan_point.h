#ifndef DAN_POINT_H
#define DAN_POINT_H

#include "dan_vector.h"

typedef dan_2vector dan_2point;
typedef dan_3vector dan_3point;

void dan_copy_2point(const dan_2point p, dan_2point r);
void dan_subtract_2points(const dan_2point p1, const dan_2point p2, dan_2vector r);
void dan_middle_2point(const dan_2point p1, const dan_2point p2, dan_2point r);

#endif
