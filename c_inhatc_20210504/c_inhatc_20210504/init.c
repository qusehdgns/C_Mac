
#include <stdio.h>
#include <string.h>
#include "pso.h"

/* Array of particles within the solution space */
particle_t particles[MAX_PARTICLES];
particle_t pbest[MAX_PARTICLES];

/* Global Best particle */
particle_t gbest;


void storePbest(int index)
{
    /* Save the particle to its personal best */
    memcpy((void*)&pbest[index], (void*)&particles[index],
        sizeof(particle_t));

    /* If this pbest is better than the gbest, save it */
    if (pbest[index].fitness > gbest.fitness) {
        gbest.position.x = pbest[index].position.x;
        gbest.position.y = pbest[index].position.y;
        gbest.fitness = pbest[index].fitness;
    }
    return;
}


#define MAX_FILENAME    80

void initPopulation(void)
{
    int i;
    char filename[MAX_FILENAME + 1];

    extern double checkFitness(double, double);

    gbest.fitness = (double)-9999.0;

    for (i = 0; i < MAX_PARTICLES; i++) {

        /* Initialize the output file pointer */
        sprintf_s(filename, MAX_FILENAME + 1, "particle%02d.txt", i);
        fopen_s(&particles[i].fp, filename, "w");

        /* Initialize a particle's starting point and fitness*/
        particles[i].position.x = getPoint();
        particles[i].position.y = getPoint();
        particles[i].fitness =
            checkFitness(particles[i].position.x, particles[i].position.y);

        /* Initialize the particle's dimensional velocity */
        particles[i].velocity.x = (getSRand() / (double)10.0);
        particles[i].velocity.y = (getSRand() / (double)10.0);

        /* Since this is the only fitness value, store this as pbest */
        storePbest(i);

        /* Store this position to the particles trace file */
        fprintf_s(particles[i].fp, "%+8.5lg, %+8.5lg, %+8.5lg\n",
            particles[i].position.x, particles[i].position.y,
            particles[i].fitness);

    }

    return;
}


void closePopulation(void)
{
    int i;

    for (i = 0; i < MAX_PARTICLES; i++) {

        fclose(particles[i].fp);

    }

    return;
}

