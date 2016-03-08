#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

__declspec(target(mic))
double A, B, C[100], sum;

__attribute__((target(mic))) void MyFunction() {
    sum  = A + B;
}

int main(int argc, char * argv[] ) {
  A=5;
  B=2;
  int cont;
  int n=100;

  for (cont=0; cont<n; cont++)
    C[cont]=cont*8;

  #pragma offload target(mic) in (A, B) out (sum) 
  {
    MyFunction();
  }

  printf("sum %f \n", sum);
}
