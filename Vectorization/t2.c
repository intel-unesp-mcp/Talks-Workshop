#include <stdio.h>
#define RAND_MAX 100

double random(double min, double max);

double random(double min, double max) // definition
{

  double r = (double)rand()/RAND_MAX;
  r = r*(max-min)+min;
  return r;
}

void foo1(double *a, double *b, double *c, int n)
{
//__assume_aligned(a, 64);
//__assume_aligned(b, 64);
//__assume_aligned(c, 64);

  int i;
  //#pragma ivdep
  for (i=0; i<n; i++) {
    a[i] = b[i] + c[i];
    //b[i] = b[i] + c[i];
    //c[i] = b[i] - c[i];
  }
}

void foo2(double *a, double *b, double *c, int n)
{
//__assume_aligned(a, 64);
//__assume_aligned(b, 64);
//__assume_aligned(c, 64);

  int i;

  //#pragma ivdep
  for (i=0; i<n; i++) {
    a[i] *= b[i] - c[i];
    b[i] = b[i] + c[i];
    c[i] = b[i] - c[i];
  }
}

int main() {
  int nn, i, j;

//  __declspec(align(64)) double *a;
//  __declspec(align(64)) double *b;  
//  __declspec(align(64)) double *c;
   double *a;
   double *b;  
   double *c;

  nn=9000;

  //a = (double*) _mm_malloc(3*sizeof(double), 64);
  //b = (double*) _mm_malloc(3*sizeof(double), 64);
  //c = (double*) _mm_malloc(3*sizeof(double), 64);
  a = (double*) malloc(nn*sizeof(double));
  b = (double*) malloc(nn*sizeof(double));
  c = (double*) malloc(nn*sizeof(double));
  for( i=0; i<nn; i++) {
    a[i] = random(0.0, 1.0)+j;
    b[i] = random(0.5, 1.5)-j;
    c[i] = random(0.8, 1.9)+j;
  }

  for( j=0; j<100000; j++) {
    a[j] = random(0.0, 1.0)+j;
    b[j] = random(0.5, 1.5)-j;
    c[j] = random(0.8, 1.9)+j;

    foo1(a, b, c, nn);
  //foo2(a, b, c, nn);
  }
  free(a);
  free(b);
  free(c);

  return(0);
}
