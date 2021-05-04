/*
 *  Ant Algorithm 실습프로그램
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
 *  도시,도시간 거리, 개미 모집단을 초기화시키는 함수
 */

void init( void )
{
  int from, to, ant;


    /* 그리드에 도시 생성 과정 */
    for (from = 0 ; from < MAX_CITIES ; from++) {

  //  /*  임의 좌표 부여하여 그리드에 도시 생성 */
    cities[from].x = getRand( MAX_DISTANCE );
    cities[from].y = getRand( MAX_DISTANCE );
 

    for (to = 0 ; to < MAX_CITIES ; to++) {
      distance[from][to] = 0.0;
      pheromone[from][to] = INIT_PHEROMONE;
    }

  }

  /* 각 도시간의 거리를 계산  */
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

    /* 개미 초기화 */
  //to = 0;
  to = getRand(20);
  for ( ant = 0 ; ant < MAX_ANTS ; ant++ ) {

    /* 개미들을 그리드 상의 도시에 배치 */
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

    /* 개미가 있는 도시들을 타부 리스트에 적재 */
    ants[ant].tabu[ants[ant].curCity] = 1;

  }
}


/*
 *  restartAnts()
 *
 *  재차 임무수행(여행)을 하도록 개미 모집단을 다시 초기화 시킴
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
 *  경로 발견 수식에 사용될 분모값 계산
 */

double antProduct( int from, int to )
{
  return (( pow( pheromone[from][to], ALPHA ) *
            pow( (1.0 / distance[from][to]), BETA ) ));
}


/*
 *  selectNextCity()
 *  경로 선택 알고리즘과 현재의 페르몬 양으로  개미가 방문할 다음 도시를 선택
 */
int selectNextCity( int ant )
{
  int from, to;
  double denom=0.0;

  /* 방문할 다음 도시  */
  from = ants[ant].curCity;

  /* 경로 발견용 확률식에 사용될 분모값 계산*/
  for (to = 0 ; to < MAX_CITIES ; to++) {
      if (ants[ant].tabu[to] == 0) {  // 방문한 도시가 아님...
      denom += antProduct( from, to );
    }
  }

  if(denom == 0)
  {
      printf("0-divied occur!!\n");
      printf("\n\n");
      exit(1);
  }
  //assert(denom != 0.0);

  do {

    double p;

    to++;   // to 이후 도시 조사하다 마지막 도시 만나면 0번 도시부터 조사...
    if (to >= MAX_CITIES) to = 0;

    if ( ants[ant].tabu[to] == 0 ) {

      p = antProduct(from, to)/denom;   // 수식 7.1 확률 계산

      if (getSRand() < p ) break;  // 확률 보다 크면 다음 방문도시로 선정

    }

  } while (1);

  return to;
}


/*
 *  simulateAnts()
 *  모집단의 각각의 개미가 일단 한번씩 모두 여행을 마치게 하며
 *  일단 한번 모두 마치면 초기화되어 다시 여행토록 하는 함수
 */

// k개의 에이전트가 각각 도시 탐방 , 마지막 도시 갈때까지.. 호출됨
// pathIndex 가 MAX_CITIES보다 크면 moving값은 0로 리턴됨

int simulateAnts( void )
{
  int k;
  int moving = 0;

  for (k = 0 ; k < MAX_ANTS ; k++) {

    /* 개미가 아직 방문할 도시가 있는지... */
    if (ants[k].pathIndex < MAX_CITIES) {

      ants[k].nextCity = selectNextCity( k );

      ants[k].tabu[ants[k].nextCity] = 1;

      ants[k].path[ants[k].pathIndex++] = ants[k].nextCity;

      ants[k].tourLength += distance[ants[k].curCity][ants[k].nextCity];

      /* 마지막 도시 처리 (마지막 도시에서 to 출발 도시로) */
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
 *  개미가 다녔던 경로의 모든 페르몬 양을 업데이트(증발 포함) 시키는 함수
 */

void updateTrails( void )
{
  int from, to, i, ant;

  /* 페프몬 증발 처리 */
  for (from = 0 ; from < MAX_CITIES ; from++) {

    for (to = 0 ; to < MAX_CITIES ; to++) {

      if (from != to) {

        pheromone[from][to] *= (1.0 - RHO);

        if (pheromone[from][to] < 0.0) pheromone[from][to] = INIT_PHEROMONE;

      }

    }

  }

  /* 자주 다니는 경로에 대해 페르몬 누적 처리*/
  /* 각 개미의 여행 경로에 대하여... */
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
 *  가장 좋은 솔류션을 가진 경로 출력,  *
 */

void emitDataFile( int ant )
{
  int city;
  FILE *fp;

  fp = fopen("/Users/donghunbyun/Desktop/workspace/C/c_inhatc_20210427/c_inhatc_20210427/cities.dat", "w");
  for (city = 0 ; city < MAX_CITIES ; city++) {
    fprintf(fp, "%02d %02d\n", cities[city].x, cities[city].y);
  }
  fclose(fp);

  fp = fopen("/Users/donghunbyun/Desktop/workspace/C/c_inhatc_20210427/c_inhatc_20210427/solution.dat", "w");
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
 *  메인 함수
 */

int main()
{
  FILE *fp1;
  int curTime = 0;

  fp1=fopen("/Users/donghunbyun/Desktop/workspace/C/c_inhatc_20210427/c_inhatc_20210427/trace_best.dat","w");

  srand( (unsigned int) time(NULL) );

  init();

  while (curTime++ < MAX_TIME) {

    if ( simulateAnts() == 0 ) {  // 모든 에이전트가 도시 탐방을 마치고 돌아옴
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

