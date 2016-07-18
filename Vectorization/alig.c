#include <stdio.h>

//__declspec(align(64)) 
//float X[10000], X2[10000];
/*
void foo(float *a, int n, int n1, int n2) {
  int i;
  //__assume_aligned(a);

  //__assume(n1%16==0);
  //__assume(n2%16==0);

  for(i=0;i<n;i++) { // Compiler vectorizes loop with all aligned accesses
    X[i] = a[(i+2)] + a[(i+2)+n1] + a[(i+2)-n1]+ a[(i+2)+n2] + a[(i+2)-n2];
  }

  for(i=0;i<n;i++) { // Compiler vectorizes loop with all aligned accesses
    X2[i] = X[i]*a[i];
  }
}
*/

int main() {
  float a[10000], X[10000], X2[10000];

  //float * a;
  int i, n, n1, n2, j;
 
  n=10000;
  n1=1;
  n2=2;
  //a = (float*) malloc((n+10)*sizeof(float));
for( j=0; j<100000; j++) {
  for(i=0;i<n;i++) {
    a[i] =rand();
    X[i] =rand();
    X2[i] =rand();
  }

  for(i=0;i<n;i++) { // Compiler vectorizes loop with all aligned accesses
    X[i] = X[i] + a[i];
    X2[i] = X2[i] * a[i];
  }

  }

//  for( j=0; j<100000; j++) {
//    foo(a, n, n1, n2);
//  }

  return(0);
}
