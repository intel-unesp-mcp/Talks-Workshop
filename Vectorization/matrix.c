#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void sum(double** a, double** b, double** c, int msize) {
  //__assume_aligned(a, 64);
  //__assume_aligned(b, 64);
  //__assume_aligned(c, 64);

  int i,j;

  for(i=0; i<msize; i++) {
    for(j=0; j<msize; j++) {
	  //a[i][j] = (b[j][i] + c[i][j])*(a[i][j]);
           a[i][j] = b[i][j] + c[i][j];

	  //d[i][j] = c[i][j] - c[i][j];
    }
  } 
}

void quad(int length, float * a, float * b, float * c, float * x1, float * x2)
{
  int i;

  for (i=0; i<length; i++) {
    float s = b[i]*b[i] - 4*a[i]*c[i];

    if ( s >= 0 ) {
      s = sqrt(s) ;
      x2[i] = (-b[i]+s)/(2.*a[i]);
      x1[i] = (-b[i]-s)/(2.*a[i]);
    }
    else {
      x2[i] = 0.;
      x1[i] = 0.;
    }
  }

}

void mult1(double** a, double** b, double** c, int msize) {
  int i,j, k;

  for(i=0; i<msize; i++) {
    for(j=0; j<msize; j++) {
      for(k=0; k<msize; k++) {
        c[i][j] = c[i][j] + a[i][k] * b[k][j];
      }
    }
  } 
}

void mult2(double** a, double** b, double** c, int msize) {
  int i,j, k;

  for(i=0; i<msize; i++) {
    for(k=0; k<msize; k++) {
      for(j=0; j<msize; j++) {
        c[i][j] = c[i][j] + a[i][k] * b[k][j];
      }
    }
  } 
}

void mult3(double** a, double** b, double** c, int msize) {
  int i,j, k;

  for(i=0; i<msize; i++) {
    for(k=0; k<msize; k++) {
     #pragma simd
     for(j=0; j<msize; j++) {
	c[i][j] = c[i][j] + a[i][k] * b[k][j];
      }
    }
  } 
}