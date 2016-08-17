#include <stdio.h>
#include "matrix.h"
#include <time.h>

struct coordinate {
    float x, y, z;
} aosobj[40000];

struct coordinate2 {
    float x[40000], y[40000], z[40000];
} soaobj;

int main(){

  int i,j;
  float randV;

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
}
