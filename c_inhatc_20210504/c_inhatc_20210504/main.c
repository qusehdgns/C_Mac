/*
 *  Particle Swarm Optimization main function
 */

#include <stdio.h>
#include <time.h>
#include "pso.h"

 /* Acceleration and dt constants */
double c1 = 0.5, c2 = 0.5, dt = 0.1;

int main()
{
    int iter;
    FILE* fp;
    extern void initPopulation(void);
    extern void moveSwarm(void);
    extern void closePopulation(void);

    extern particle_t gbest;

    srand((unsigned)time(NULL));

    fopen_s(&fp, "best.txt", "w");

    /* Initialize the swarm */
    initPopulation();

    /* Seed the random number generator */
   /* Perform NUM_ITERATIONS number of iterations */

    for (iter = 0; iter < NUM_ITERATIONS; iter++) {

        moveSwarm();
        printf("%lg\n", gbest.fitness);
        fprintf_s(fp, "%lg\n", gbest.fitness);
    }

    /* Cleanup the swarm */
    closePopulation();

    return 0;
}


