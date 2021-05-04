#include <stdio.h>
#include "pso.h"

void moveParticle(int i)
{
    extern double c1, c2, dt;

    extern particle_t particles[], pbest[];
    extern particle_t gbest;
    extern double checkFitness(double, double);

    /* Update the position of the particle */
    particles[i].position.x += (particles[i].velocity.x * dt);
    particles[i].position.y += (particles[i].velocity.y * dt);

    /* Since we're interested only in the -2PI-2PI range, we'll
     * calculate fitness only over this range.
     */
    if ((particles[i].position.x > (-2.0 * PI)) &&
        (particles[i].position.x < (2.0 * PI)) &&
        (particles[i].position.y > (-2.0 * PI)) &&
        (particles[i].position.y < (2.0 * PI))) {

        particles[i].fitness =
            checkFitness(particles[i].position.x, particles[i].position.y);

    }

    /* Write the particle's position and fitness to the trace file */
    fprintf(particles[i].fp, "%+8.5lg,  %+8.5lg,  %+8.5lg\n",
        particles[i].position.x, particles[i].position.y,
        particles[i].fitness);

    /* Update the velocity vector of the particle */
    particles[i].velocity.x +=
        ((c1 * getSRand() * (gbest.position.x - particles[i].position.x)) +
            (c2 * getSRand() * (pbest[i].position.x - particles[i].position.x)));

    particles[i].velocity.y +=
        ((c1 * getSRand() * (gbest.position.y - particles[i].position.y)) +
            (c2 * getSRand() * (pbest[i].position.y - particles[i].position.y)));

    return;
}


void moveSwarm(void)
{
    int j;

    extern particle_t particles[], pbest[];
    extern void storePbest(int);

    /* Move each particle in the swarm */
    for (j = 0; j < MAX_PARTICLES; j++) {

        moveParticle(j);

        if (particles[j].fitness > pbest[j].fitness) storePbest(j);

    }
    return;
}
