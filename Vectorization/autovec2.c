#include <stdio.h>
#include "matrix.h"
#include <time.h>

void mult2(double** a, double** b, double** c, int msize) {
  int i,j, k;

  //#pragma omp parallel for collapse (2)
  for(i=0; i<msize; i++) {
    for(j=0; j<msize; j++) {
       //#pragma omp simd
       for(k=0; k<msize; k++) {
	  c[i][j] = c[i][j] + a[i][k] * b[k][j];
	}
    }
  } 
}

void init(double** a, double** b, double** c, int msize, double randV) {
  int i,j;
  for(i=0; i<msize; i++) {
    for(j=0; j<msize; j++) {
	a[i][j] = (i+j)-randV;
	b[i][j] = (i-j)*randV;
	c[i][j] = (i*j)+randV;
    }
  }

}

void initV2(double** a, double** b, double** c, int msize) {
  int i,j;
  for(i=0; i<msize; i++) {
    for(j=0; j<msize; j++) {
	a[i][j] = 1;
	b[i][j] = 1;
	c[i][j] = 1;
    }
  }
}

void print(double** a, double** b, double** c, int msize) {
  int i,j;

  for(i=0; i<msize; i++) {
    for(j=0; j<msize; j++) {
      //printf("a %f\n", a[i][j]);
      //printf("b %f\n", b[i][j]);
      printf("a %f\n", a[i][j]);
    }
  }
}

int main(){

  int i,j,k,msize;
  double **a;
  double **b;
  double **c;
  double randV;

  msize=4096;//16000;
  randV=0;

a = (double**) malloc(msize * sizeof(double*));
for ( i = 0; i < msize; i++) {
  a[i] = (double*) malloc(msize * sizeof(double));
}
b = (double**) malloc(msize * sizeof(double*));
for ( i = 0; i < msize; i++) {
  b[i] = (double*) malloc(msize * sizeof(double));
}
c = (double**) malloc(msize * sizeof(double*));
for ( i = 0; i < msize; i++) {
  c[i] = (double*) malloc(msize * sizeof(double));
}

srand(time(NULL));
randV=rand();
randV=randV*0.11;
printf("randV %f\n", randV);

init(a, b, c, msize, randV);

/*for(k=0; k<100; k++) {

initV2(a, b, c, msize);

  for(i=0; i<msize; i++) {
    for(j=0; j<msize; j++) {
	  a[i][j] = b[i][j] + c[i][j];
    }
  } 
}
*/

//print(a, b, c, msize);
//sum(a, b, c, msize);
//sum(c, a, b, msize);

/*
for(i=0; i<msize; i++) {
    for(j=0; j<msize; j++) {
       //#pragma omp simd
       for(k=0; k<msize; k++) {
	  c[i][j] = c[i][j] + a[i][k] * b[k][j];
	}
    }
  } 
*/

mult(a, b, c, msize);

for (i = 0; i < msize; i++) {
  free(a[i]);
}
free(a);
for (i = 0; i < msize; i++) {
  free(b[i]);
}
free(b);
for (i = 0; i < msize; i++) {
  free(c[i]);
}
free(c);

}