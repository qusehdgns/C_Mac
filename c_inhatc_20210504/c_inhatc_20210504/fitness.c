#include <stdio.h>
#include <math.h>
#include "pso.h"

double checkFitness(double x, double y)
{
    /* Sombrero Fitness Function */
    return ((double)(6.0 * cos(sqrt(x * x + y * y)) / (x * x + y * y + 6.0)));
}
