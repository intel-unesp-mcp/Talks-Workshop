#include <stdio.h>
#include <stdlib.h>

#pragma offload_attribute(push, target(mic))

double A, B, sum, n;
double C[10]; 

void Function1() {
  int cont;

  for (cont=0; cont<n; cont++)
    C[cont]=A + B;
}

void Function2() {
  int cont;

  for (cont=0; cont<n; cont++)
    sum += C[cont];
}

#pragma offload_attribute(pop)

int main(int argc, char * argv[])
{

  n=10;
  int cont, contRepetitions;

  A = 1;
  B = 1;

  for (cont=0; cont<n; cont++)
    C[cont]=0;

  for (cont=0; cont<n; cont++)
    printf("before offload Function1() C[cont] %f  \n", C[cont]);

  #pragma offload target(mic:0) in (A, B, n) inout (C) 
  {
    Function1();
  }

  for (cont=0; cont<n; cont++)
    printf("after offload Function1() C[cont] %f  \n", C[cont]);

  printf("before second offload SUM %f \n", sum);

  #pragma offload target(mic:1) in (n, C) out (sum) 
  {
    Function2();
  }

  printf("after second offload SUM %f \n", sum);

}