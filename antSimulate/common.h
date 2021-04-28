#ifndef __COMMON_H
#define __COMMON_H

#include <math.h>
#include <stdlib.h>

#define MAX_CITIES	20

#define MAX_DISTANCE	100

#define MAX_TOUR	(MAX_CITIES * MAX_DISTANCE)  

typedef struct {
	int x;
	int y;
} cityType;


#define MAX_ANTS	20

typedef struct {
	int curCity;
	int nextCity;
	unsigned char tabu[MAX_CITIES];
	int pathIndex;
	unsigned char path[MAX_CITIES];
	double tourLength;
} antType;

#define getSRand()	((float)rand() / (float)RAND_MAX)
#define getRand(x)      (int)((float)(x)*rand()/(RAND_MAX+1.0)) // 0~(x-1)

#define ALPHA		1.0
#define BETA		1.0 
#define RHO		0.9 	/* Intensity / Evaporation */
#define QVAL		100

#define MAX_TOURS	500

#define MAX_TIME	(MAX_TOURS * MAX_CITIES)  // 500*20= 10000

#define INIT_PHEROMONE	(1.0 / MAX_CITIES)

#endif /* __COMMON_H */

