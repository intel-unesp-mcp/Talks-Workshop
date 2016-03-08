#include <stdio.h>
#include <stdlib.h>

#pragma offload_attribute(push, target(mic))

double A, B, C[100], sum;

void Function1() {
    sum  = A + B;
}
/*
void Function2() {
  int cont;
  int n=100;

  for (cont=0; cont<n; cont++)
    C[cont] += sum;
}*/

#pragma offload_attribute(pop)

int main(int argc, char * argv[])
{
  printf("Hello World from host!\n");

  int cont;
  int n=100;

  A=5;
  B=2;
  for (cont=0; cont<n; cont++)
    C[cont]=cont;

  #pragma offload target(mic:0) in (A, B) out (sum) 
  {
    Function1();
  }

  printf("sum %f \n", sum);

/*
  #pragma offload target(mic:1) in (sum) out (C) 
  {
    Function2();
  }

  printf("C[22] %f \n", C[22]);
*/
}
