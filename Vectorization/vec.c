#include <stdio.h>
#include "matrix.h"
#include <time.h>

double A[1000]; 
double B[1000]; 
void foo(int n){
  int gg; 
//#pragma vector nontemporal (A, B)
  for (gg=0; gg<n; gg++){
    A[gg] = 0;
    B[gg] = gg;
  } 
}

struct coordinate {
    float x, y, z;
} aosobj[40000];

struct coordinate2 {
    float x[40000], y[40000], z[40000];
} soaobj;

int main(){



  foo(1000);
  int i,j,k,msize;
  double *a;
  double *b;
  double *c;
  double randV;

  msize=20480;//3036;
  randV=0;

a = (double*) _mm_malloc(msize * msize *sizeof(double),64);
b = (double*) _mm_malloc(msize * msize *sizeof(double),64);
c = (double*) _mm_malloc(msize * msize * sizeof(double),64);

srand(time(NULL));
randV=rand();
randV=randV*0.11;
printf("randV %f\n", randV);


 for(j=0; j<10000; j++) {

 for(i=0; i<40000; i++) {
   aosobj[i].x=i + randV;    
   aosobj[i].y=i*i+ randV;  
   aosobj[i].z=i+i+ randV;
 }
 for(i=0; i<40000; i++) {
   soaobj.x[i]=i+i+ randV;
   soaobj.y[i]=i-i+ randV;
   soaobj.z[i]=i*i+ randV;
}
randV=rand();
randV=randV*0.11;

 for(i=0; i<40000; i++) {
   aosobj[i].x=    aosobj[i].y+  aosobj[i].z + randV;
 }

 for(i=0; i<40000; i++) {

   soaobj.x[i]= soaobj.y[i]+ soaobj.z[i] + randV;
 }
}

//init(a, b, c, msize, randV);

//sum(a, b, c, msize);
//sumLoopTiling(a, b, c, msize);

//multv1(a, b, c, msize);

_mm_free(a);
_mm_free(b);
_mm_free(c);

}
