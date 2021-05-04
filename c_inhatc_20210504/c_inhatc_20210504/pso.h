#include <stdlib.h>

typedef struct {
    double x;
    double y;
} vector_t;

typedef struct {
    FILE* fp;
    vector_t position;
    vector_t velocity;
    double fitness;
} particle_t;

#define MAX_PARTICLES    5

// 0 ~ 1 사이의 실수값
#define getSRand()    ((double)rand() / (double)RAND_MAX)

#define PI        ((double)3.1415926)

// -2PI ~ 2PI 사이의 실수값
#define getPoint()    (((4.0 * PI) * getSRand()) - (2.0 * PI))

#define NUM_ITERATIONS    100


