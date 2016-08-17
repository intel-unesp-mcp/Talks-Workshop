#include <stdlib.h>
#include <omp.h>

int main() {

  int sum, i, N, teams, idteam, idthread, flag;
  N=20; 
  sum=2;

/*
  //#pragma omp target data device(3) map(i, N, teams, idteam, idthread) //map(tofrom:sum)
  #pragma omp target teams distribute parallel for num_teams(2) thread_limit(3) //reduction(+:sum) num_teams(2) thread_limit(3) 
  for (i=0; i<N; i++) {
    //sum += i;
    idthread = omp_get_thread_num();
    idteam = omp_get_team_num();
    teams = omp_get_num_teams();
    printf(" i %d n %d idteam %d idthread %d teams %d\n", i,N, idteam, idthread, teams);
  }

//  printf(" sum %d \n", sum);
*/

  #pragma omp target device(3) map(i, N, teams, idteam, idthread) //map(tofrom:sum)
  {  
  for (i=0; i<N; i++) {
    //sum += i;
    idthread = omp_get_thread_num();
    idteam = omp_get_team_num();
    teams = omp_get_num_teams();
    printf(" i %d n %d idteam %d idthread %d teams %d\n", i,N, idteam, idthread, teams);
  }
  }
//  printf(" sum %d \n", sum);

  return(0);
}	
