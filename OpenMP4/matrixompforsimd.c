#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#pragma omp declare target

  #pragma omp declare simd
  float min(float a, float b) { 
    return a < b ? a : b;
  }

  #pragma omp declare simd
  float distsq(float x, float y) { 
    return (x - y) * (x - y);
  }

  //#define NUM 10240
  //#define NUM 40240
  #define NUM 2048

  typedef unsigned long long  UINT64;
  typedef double array[NUM];

  char *buf1, *buf2, *buf3, *buf4;
  char *addr1, *addr2, *addr3, *addr4;
  int Offset_Addr1, Offset_Addr2, Offset_Addr3,Offset_Addr44;

  array *a, *b, *c;
  
  double row, col, off;

#pragma omp end declare target

int main()
{
  int i,j, k, nthr, repetitions, contrep, auxrand;

  Offset_Addr1=128;
  Offset_Addr2=192;
  Offset_Addr3=0;

  row=3;
  col=-2;
  off=1;

  buf1 = _mm_malloc((sizeof (double))*NUM*NUM, 64);  
  buf2 = _mm_malloc((sizeof (double))*NUM*NUM, 64);
  buf3 = _mm_malloc((sizeof (double))*NUM*NUM, 64);

  buf1 = (char *) malloc(NUM*NUM*(sizeof (double))+1024);
  printf("Addr of buf1 = %p\n",buf1); fflush(stdout);
  addr1 = buf1 + 256 - ((UINT64)buf1%256) + (UINT64)Offset_Addr1;
  printf("Offs of buf1 = %p\n",addr1); fflush(stdout);
	
  buf2 = (char *) malloc(NUM*NUM*(sizeof (double))+1024);
  printf("Addr of buf2 = %p\n",buf2); fflush(stdout);
  addr2 = buf2 + 256 - ((UINT64)buf2%256) + (UINT64)Offset_Addr2;
  printf("Offs of buf2 = %p\n",addr2); fflush(stdout);
	
  buf3 = (char *) malloc(NUM*NUM*(sizeof (double))+1024);
  printf("Addr of buf3 = %p\n",buf3); fflush(stdout);
  addr3 = buf3 + 256 - ((UINT64)buf3%256) + (UINT64)Offset_Addr3;
  printf("Offs of buf3 = %p\n",addr3); fflush(stdout);

  a = (array *) addr1;
  b = (array *) addr2;
  c = (array *) addr3;

  srand(time(NULL));
  double rr;
  rr = rand();
  off=rr;

  nthr=60;
  nthr=36;

  omp_set_num_threads(nthr);  

  #pragma omp parallel
  for (i=0; i< NUM;i++) {
    #pragma omp simd 
    for (j=0; j<NUM;j++) {
      a[i][j] = row*i+col*j+off;
      c[i][j] = 0;
    }
  }


  row=-2;
  col=1;
  rr = rand();
  off=rr;
  #pragma omp parallel
  for (i=0; i< NUM;i++) {
    #pragma omp simd 
    for (j=0; j<NUM;j++) {
	  b[i][j] = row*i+col*j+off;
    }
  }

  printf("begin main calc");
/*
  //ex 1 - pragma omp simd

  #pragma omp parallel for collapse (2)
  for(i=0; i<NUM; i++) {
    for(k=0; k<NUM; k++) {
      #pragma omp simd 
      for(j=0; j<NUM; j++) {
        c[i][j] = c[i][j] + a[i][k] * b[k][j];
      }
    }
  }
*/

  //ex 3 - #pragma omp parallel for simd

  #pragma omp target
  {
    repetitions=180;//0000;
    contrep=0;
    auxrand=0;

    for(contrep=0; contrep<repetitions; contrep++) {
      auxrand=rand();

      i=0;
      #pragma omp parallel for simd
      for(j=0; j<NUM; j++) {
        a[i][j] = distsq(a[i][j], b[i][j])-auxrand;
        b[i][j] += min(a[i][j], b[i][j])+auxrand;
        c[i][j] = (min(distsq(a[i][j], b[i][j]), a[i][j]))/auxrand;
      }
    }
  }
}






