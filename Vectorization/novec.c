#include <time.h>
#include <stdio.h>

int foo(int n){
int j;  
int aux=0;
  aux=cos(n);
  if (aux > 100)
    aux=+sin(n);
  else {
    for (j=0; j<50; j++) {
      aux=-sin(aux);
    }
  }

  
  if (aux > 0) aux=1/aux;

  return(aux*2);
}

int main(){
  const int n=90000;
  int i, j, randV;
  int A[n];
  int B[n];
  int C[n];

  for (i=1; i<n; i++) {
    A[i]= 1;
    B[i]= 1;
    C[i]= 1;
}

  for (j=0; j<45000; j++) {
srand(time(NULL));
randV=rand();

  for (i=1; i<n; i++)
    A[i]=A[i-1]+B[i]-i*randV;

  // This loop will be auto-vectorized
  for (i=1; i<n; i++)
    A[i]= A[i-1]+ B[i]*randV;

  for (i=1; i<n; i++)
    A[B[i]]= B[C[i]]+randV;

  for (i=1; i<n; i++)
    A[i]= foo(B[i])+randV;
}

//  for (i=0; i<n; i++)
  //  printf("%2d %2d %2d\n", i, A[i], B[i]);
}
