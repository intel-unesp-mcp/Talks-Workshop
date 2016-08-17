#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

/*
__declspec(target(mic)) double A, B, C[100], sum;

__attribute__((target(mic))) void MyFunction() {
    sum  = A + B;
}
__declspec(target(mic)) double A, B, C[100], sum;

__declspec(target(mic)) void MyFunction() {
    sum  = A + B;
}
*/

__attribute__((target(mic))) double A, B, C[100], sum;

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

  printf("sum before offload %f \n", sum);

  #pragma offload target(mic) in (A, B) out (sum) 
  {
    MyFunction();
  }

  printf("sum after offload %f \n", sum);}
