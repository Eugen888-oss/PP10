#include <stdio.h>
#include <math.h>
#include "point.h"

int main () {
Point p = {3.0 , 4.0};

printf("Distance is: %lf\n", sqrt(p.x*p.x+p.y*p.y));

return 0;

}
