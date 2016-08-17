#include <time.h>
#include <stdio.h>

int main(){
  const int n=90000;
  int i, j, randV;
  int A[n];
  int B[n];


  for (j=0; j<45000; j++) {
srand(time(NULL));
randV=rand();

  for (i=0; i<n; i++)
    A[i]=B[i]=i*randV;

  // This loop will be auto-vectorized
  for (i=0; i<n; i++)
    A[i]+=B[i]*randV;

}

//  for (i=0; i<n; i++)
  //  printf("%2d %2d %2d\n", i, A[i], B[i]);
}
