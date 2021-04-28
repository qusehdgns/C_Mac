/*
 *  Ant Algorithm �ǽ����α׷�
 */


#include <stdio.h>
#include <time.h>
#include <assert.h>
#include "common.h"

//cityType cities[MAX_CITIES] = { { 1,8 },{ 8,8 },{ 8,1 },{ 5,5 },{ 1,1 } };

cityType cities[MAX_CITIES];

antType ants[MAX_ANTS];

              /*   From          To     */
double   distance[MAX_CITIES][MAX_CITIES];

              /*    From         To      */
double pheromone[MAX_CITIES][MAX_CITIES];

double  best=(double)MAX_TOUR;
int     bestIndex;


/*
 *  init()
 *  ����,���ð� �Ÿ�, ���� �������� �ʱ�ȭ��Ű�� �Լ�
 */

void init( void )
{
  int from, to, ant;


    /* �׸��忡 ���� ���� ���� */
	for (from = 0 ; from < MAX_CITIES ; from++) {

  //  /*  ���� ��ǥ �ο��Ͽ� �׸��忡 ���� ���� */
    cities[from].x = getRand( MAX_DISTANCE );
    cities[from].y = getRand( MAX_DISTANCE );
 

    for (to = 0 ; to < MAX_CITIES ; to++) {
      distance[from][to] = 0.0;
      pheromone[from][to] = INIT_PHEROMONE;
    }

  }

  /* �� ���ð��� �Ÿ��� ���  */
  for ( from = 0 ; from < MAX_CITIES ; from++) {

    for ( to = 0 ; to < MAX_CITIES ; to++) {

      if ((to != from) && (distance[from][to] == 0.0)) {
        int xd = abs(cities[from].x - cities[to].x);
        int yd = abs(cities[from].y - cities[to].y);

        distance[from][to] = sqrt( (xd * xd) + (yd * yd) );
        distance[to][from] = distance[from][to];
      }
    }
  }

    /* ���� �ʱ�ȭ */
  //to = 0;
  to = getRand(20);
  for ( ant = 0 ; ant < MAX_ANTS ; ant++ ) {

    /* ���̵��� �׸��� ���� ���ÿ� ��ġ */
    if (to == MAX_CITIES) to = 0;
    ants[ant].curCity = to++;

    for ( from = 0 ; from < MAX_CITIES ; from++ ) {
      ants[ant].tabu[from] = 0;
      ants[ant].path[from] = -1;
    }

    ants[ant].pathIndex = 1;
    ants[ant].path[0] = ants[ant].curCity;
    ants[ant].nextCity = -1;
    ants[ant].tourLength = 0.0;

    /* ���̰� �ִ� ���õ��� Ÿ�� ����Ʈ�� ���� */
    ants[ant].tabu[ants[ant].curCity] = 1;

  }
}


/*
 *  restartAnts()
 *
 *  ���� �ӹ�����(����)�� �ϵ��� ���� �������� �ٽ� �ʱ�ȭ ��Ŵ
 *  
 */

void restartAnts( void )
{
  int ant, i, to=0;

  for ( ant = 0 ; ant < MAX_ANTS ; ant++ ) {

    if (ants[ant].tourLength < best) {
      best = ants[ant].tourLength;
      bestIndex = ant;
    }

    ants[ant].nextCity = -1;
    ants[ant].tourLength = 0.0;

    for (i = 0 ; i < MAX_CITIES ; i++) {
      ants[ant].tabu[i] = 0;
      ants[ant].path[i] = -1;
    }

    if (to == MAX_CITIES) to = 0;
    ants[ant].curCity = to++;

    ants[ant].pathIndex = 1;
    ants[ant].path[0] = ants[ant].curCity;

    ants[ant].tabu[ants[ant].curCity] = 1;

  }

}


/*
 *  antProduct()
 *  ��� �߰� ���Ŀ� ���� �и� ��� 
 */

double antProduct( int from, int to )
{
  return (( pow( pheromone[from][to], ALPHA ) *
            pow( (1.0 / distance[from][to]), BETA ) ));
}


/*
 *  selectNextCity()
 *  ��� ���� �˰���� ������ �丣�� ������  ���̰� �湮�� ���� ���ø� ����
 */
int selectNextCity( int ant )
{
  int from, to;
  double denom=0.0;

  /* �湮�� ���� ����  */
  from = ants[ant].curCity;

  /* ��� �߰߿� Ȯ���Ŀ� ���� �и� ���*/
  for (to = 0 ; to < MAX_CITIES ; to++) {
	  if (ants[ant].tabu[to] == 0) {  // �湮�� ���ð� �ƴ�...
      denom += antProduct( from, to );
    }
  }

  if(denom == 0) 
  {
	  printf("0-divied occur!!");
	  exit(1);
  }
  //assert(denom != 0.0);

  do {

    double p;

    to++;   // to ���� ���� �����ϴ� ������ ���� ������ 0�� ���ú��� ����...
    if (to >= MAX_CITIES) to = 0;

    if ( ants[ant].tabu[to] == 0 ) {

      p = antProduct(from, to)/denom;   // ���� 7.1 Ȯ�� ��� 

      if (getSRand() < p ) break;  // Ȯ�� ���� ũ�� ���� �湮���÷� ���� 

    }

  } while (1);

  return to;
}


/*
 *  simulateAnts()
 *  �������� ������ ���̰� �ϴ� �ѹ��� ��� ������ ��ġ�� �ϸ�
 *  �ϴ� �ѹ� ��� ��ġ�� �ʱ�ȭ�Ǿ� �ٽ� ������� �ϴ� �Լ�
 */

// k���� ������Ʈ�� ���� ���� Ž�� , ������ ���� ��������.. ȣ���
// pathIndex �� MAX_CITIES���� ũ�� moving���� 0�� ���ϵ� 

int simulateAnts( void )   
{
  int k;
  int moving = 0;

  for (k = 0 ; k < MAX_ANTS ; k++) {

    /* ���̰� ���� �湮�� ���ð� �ִ���... */
    if (ants[k].pathIndex < MAX_CITIES) {

      ants[k].nextCity = selectNextCity( k );

      ants[k].tabu[ants[k].nextCity] = 1;

      ants[k].path[ants[k].pathIndex++] = ants[k].nextCity;

      ants[k].tourLength += distance[ants[k].curCity][ants[k].nextCity];

      /* ������ ���� ó�� (������ ���ÿ��� to ��� ���÷�) */
      if (ants[k].pathIndex == MAX_CITIES) {
        ants[k].tourLength += 
          distance[ants[k].path[MAX_CITIES-1]][ants[k].path[0]];
      }

      ants[k].curCity = ants[k].nextCity;

      moving++;

    }

  }

  return moving;
}


/*
 *  updateTrails()
 *  ���̰� �ٳ�� ����� ��� �丣�� ���� ������Ʈ(���� ����) ��Ű�� �Լ�
 */

void updateTrails( void )
{
  int from, to, i, ant;

  /* ������ ���� ó�� */
  for (from = 0 ; from < MAX_CITIES ; from++) {

    for (to = 0 ; to < MAX_CITIES ; to++) {

      if (from != to) {

        pheromone[from][to] *= (1.0 - RHO);

        if (pheromone[from][to] < 0.0) pheromone[from][to] = INIT_PHEROMONE;

      }

    }

  }

  /* ���� �ٴϴ� ��ο� ���� �丣�� ���� ó��*/
  /* �� ������ ���� ��ο� ���Ͽ�... */
  for (ant = 0 ; ant < MAX_ANTS ; ant++) {

    /* Update each leg of the tour given the tour length */
    for (i = 0 ; i < MAX_CITIES ; i++) {

      if (i < MAX_CITIES-1) {
        from = ants[ant].path[i];
        to = ants[ant].path[i+1];
      } else {
        from = ants[ant].path[i];
        to = ants[ant].path[0];
      }

      pheromone[from][to] += ((QVAL / ants[ant].tourLength) * RHO);
      pheromone[to][from] = pheromone[from][to];

    }

  }

}


/*
 *  emitDataFile()
 *  ���� ���� �ַ����� ���� ��� ���,  *
 */

void emitDataFile( int ant )
{
  int city;
  FILE *fp;

  fp = fopen("cities.dat", "w");
  for (city = 0 ; city < MAX_CITIES ; city++) {
    fprintf(fp, "%02d %02d\n", cities[city].x, cities[city].y);
  }
  fclose(fp);

  fp = fopen("solution.dat", "w");
  for (city = 0 ; city < MAX_CITIES ; city++) {
    fprintf(fp, "%02d %d\n",  
		cities[ ants[ant].path[city] ].x, 
		cities[ ants[ant].path[city] ].y );
  }  
  fprintf(fp, "%02d %d\n",  
	   cities[ ants[ant].path[0] ].x, 
	   cities[ ants[ant].path[0] ].y );

  fclose(fp);
}



/*
 *  main()
 *  ���� �Լ�
 */

int main()
{
  FILE *fp1;
  int curTime = 0;

  fp1=fopen("trace_best.dat","w");

  srand( (unsigned int) time(NULL) );

  init();

  while (curTime++ < MAX_TIME) {

    if ( simulateAnts() == 0 ) {  // ��� ������Ʈ�� ���� Ž���� ��ġ�� ���ƿ� 
      updateTrails();
      if (curTime != MAX_TIME)  restartAnts();
      printf("Time is %d (%g)\n", curTime, best);
	  fprintf(fp1,"Time is %d (%g)\n", curTime, best);
    }
 
  }

  printf("best tour : %03g\n", best);
  fprintf(fp1,"best tour : %03g\n", best);
  printf("\n\n");

  emitDataFile( bestIndex );

  fclose(fp1);

  return 0;
}

