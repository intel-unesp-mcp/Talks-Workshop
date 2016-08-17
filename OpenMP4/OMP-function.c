#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <unistd.h>
#include "omp-func.h"

/*
//processor(core_4th_gen_avx)
void vec1(float *a, float *b, int off, int len)
{
  int i; 
  //#pragma omp simd safelen(32) aligned(a:64, b:64)
  for(i = 0; i < len; i++)
  {
   // a[i] = (a[i] > 1.0) ? 
   // a[i] * b[i] :
   // a[i + off] * b[i];
    a[i] = (a[i] > 2.34) ? 
    a[i] * b[i] :
    a[i] + b[i];

  }
}

void arrayref(float *x, float *y, int n, int n1) {
  int k;
  //_assume(n1%8=0);

  //#pragma vector aligned 
  //#pragma omp simd aligned(y:32) //aligned(x:32)
  for (k=0; k<n; k++) {
    x[k] = sin(x[k]) + cos(y[k])*n1; // + y[k+n1] + y[k-n1];
  }

}

#pragma omp declare simd uniform(op1) linear(k) notinbranch aligned(op1:32)
float SqrtMul1(float *op1, float op2, int k)
{
  return (sqrt(op1[k]) * sqrt(op2));
}

#pragma omp declare simd 
float SqrtMul2(float op1, float op2, int k)
{
  return (sqrt(op1) * sqrt(op2));
}

float SqrtMul3(float op1, float op2, int k)
{
  return (sqrt(op1) * sqrt(op2));
}

#pragma omp declare simd simdlen(32) uniform(constValue) linear(cont) notinbranch //aligned(x)
float seqfun(float x, float constValue, int cont) {
  return((x*constValue)-cont);
}

//void callseqfun(float *restrict a, float *restrict x, int n)
void callseqfun(float * a, float * x, int n)
{ 
  float constValue;
  constValue = 123;
//
  int i;
  #pragma omp simd
  for (i=0; i<n; i++) {
    a[i] = seqfun(x[i], constValue, i);
    //printf("a[i] %f", a[i]);
  }

  #pragma omp simd safelen(32) simdlen(32) linear(i) aligned(a)  
  for (i=0; i<n; i++) {
    a[i] = constValue+i;
  }

}
*/
int main() {

  float * a; 
  float * x; 
  float * b; 
  float * c; 
  float * z; 
  int NUM, i, k;
  int rep = 2;// 10; //10;//300;
  int contrep=0;

  #pragma omp declare target
  NUM=2147483646;
      

  a = _mm_malloc((sizeof (float))*NUM, 32);  
  x = _mm_malloc((sizeof (float))*NUM, 32);  
  c = _mm_malloc((sizeof (float))*NUM, 32);  
  b = malloc((sizeof (float))*NUM);  
  z = malloc((sizeof (float))*NUM);  
  
  #pragma omp end declare target

  printf("begin ww");

  for (contrep=0; contrep<rep; contrep++) {
	  printf("begin ww");	
  srand(time(NULL));

  for (i=0; i<NUM; i++) {
    a[i]=rand();
    x[i]=rand();
    b[i]=rand();
    c[i]=rand();
    z[i]=rand();
  }  
  int off =0;
    off=rand();
	vec7(a, x, 32, NUM);
  //vec1(a, x, 32, NUM);
  //vec2(a, x, 32, NUM);
//#pragma omp target
//	{
	//	vec3(a, x, 32, NUM);
	//}
  //vec4(a, x, 32, NUM);  
  //vec5(a, x, c, off, NUM);
  }

/*
  //arrayref(a, x, NUM, 8);
//  vec1(b, z, 32, NUM);
//  vec2(b, z, 32, NUM);
//  vec3(b, z, 32, NUM);


  for (i=0; i<NUM; i++) 
    x[i]=SqrtMul1(a, a[i], i);

  for (i=0; i<NUM; i++) {
    a[i]=rand();
    x[i]=rand();
  }  

  for (i=0; i<NUM; i++) 
    x[i]=SqrtMul2(a[i], a[i], i);

  for (i=0; i<NUM; i++) {
    a[i]=rand();
    x[i]=rand();
  }  

  for (i=0; i<NUM; i++) 
    x[i]=SqrtMul3(a[i], a[i], i);
  
//safelen(32) simdlen(32) linear(i) aligned(a)  
  #pragma omp simd aligned(a)  
  for (i=0; i<NUM; i++) {
    x[i] = a[i] + sin(a[i])*cos(x[i]);
  }

  for (i=0; i<NUM; i++) {
    x[i] = a[i] + sin(a[i])*cos(x[i]);
  }
  #pragma omp simd safelen(5)
  for (k=5; k<NUM; k++) {
    a[k] = a[k-5] + x[k];
  }

  callseqfun(a, x, NUM);

  printf("end ");  
*/

  _mm_free(a);
  _mm_free(x);
  _mm_free(c);
  free(b);
  free(z);

  return(0);
}