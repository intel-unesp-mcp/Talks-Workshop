/*
    Copyright 2005-2013 Intel Corporation.  All Rights Reserved.

    The source code contained or described herein and all documents related
    to the source code ("Material") are owned by Intel Corporation or its
    suppliers or licensors.  Title to the Material remains with Intel
    Corporation or its suppliers and licensors.  The Material is protected
    by worldwide copyright laws and treaty provisions.  No part of the
    Material may be used, copied, reproduced, modified, published, uploaded,
    posted, transmitted, distributed, or disclosed in any way without
    Intel's prior express written permission.

    No license under any patent, copyright, trade secret or other
    intellectual property right is granted to or conferred upon you by
    disclosure or delivery of the Materials, either expressly, by
    implication, inducement, estoppel or otherwise.  Any license under such
    intellectual property rights must be express and approved by Intel in
    writing.
*/

// matrix multiply routines
#include "multiply.h"

#ifdef USE_MKL
#include "mkl.h"
#endif //USE_MKL


#pragma omp declare target

  #pragma omp declare simd
  float min(float a, float b) { 
    return a < b ? a : b;
  }

  #pragma omp declare simd
  float distsq(float x, float y) { 
    return (x - y) * (x - y);
  }
#pragma omp end declare target

#ifdef USE_THR
void multiply0(int msize, int tidx, int numt, TYPE a[][NUM], TYPE b[][NUM], TYPE c[][NUM], TYPE t[][NUM])
{ 
	int i,j,k;

// Basic serial implementation
    for(i=0; i<msize; i++) {
        for(j=0; j<msize; j++) {
    	    for(k=0; k<msize; k++) {
				c[i][j] = c[i][j] + a[i][k] * b[k][j];
			}
		}
	} 
}

void multiply1(int msize, int tidx, int numt, TYPE a[][NUM], TYPE b[][NUM], TYPE c[][NUM], TYPE t[][NUM])
{
	int i,j,k;

// Naive implementation 
    for(i=tidx; i<msize; i=i+numt) {
        for(j=0; j<msize; j++) {
    	    for(k=0; k<msize; k++) {
					c[i][j] = c[i][j] + a[i][k] * b[k][j];
			}
		}
	} 
}
void multiply2(int msize, int tidx, int numt, TYPE a[][NUM], TYPE b[][NUM], TYPE c[][NUM], TYPE t[][NUM])
{
	int i,j,k;

// Step 2: Loop interchange
// Add compile option for vectorization report Windows: /Qvec-report3 Linux -vec-report3
	for(i=tidx; i<msize; i=i+numt) {
		for(k=0; k<msize; k++) {
#pragma ivdep
			for(j=0; j<msize; j++) {
				c[i][j] = c[i][j] + a[i][k] * b[k][j];
			}
		}
	} 
}
void multiply3(int msize, int tidx, int numt, TYPE a[][NUM], TYPE b[][NUM], TYPE c[][NUM], TYPE t[][NUM])
{
	int i,j,k,i0,j0,k0,ibeg,ibound,istep,mblock;

// Step3: Cache blocking
// Add current platform optimization for Windows: /QxHost Linux: -xHost
// Define the ALIGNED in the preprocessor definitions and compile option Windows: /Oa Linux: -fno-alias
    istep = msize / numt;
    ibeg = tidx * istep;
    ibound = ibeg + istep;
    mblock = MATRIX_BLOCK_SIZE;

    for (i0 = ibeg; i0 < ibound; i0 +=mblock) {
        for (k0 = 0; k0 < msize; k0 += mblock) {
            for (j0 =0; j0 < msize; j0 += mblock) {
                for (i = i0; i < i0 + mblock; i++) {
                    for (k = k0; k < k0 + mblock; k++) {
#pragma ivdep
#ifdef ALIGNED 
	#pragma vector aligned
#endif //ALIGNED
                        for (j = j0; j < j0 + mblock; j++) {
                            c[i][j]  = c[i][j] + a[i][k] * b[k][j];
						}
					}
				}
			}
		}
	}
}

void multiply4(int msize, int tidx, int numt, TYPE a[][NUM], TYPE b[][NUM], TYPE c[][NUM], TYPE t[][NUM])
{
	int i,j,k,istep,ibeg,ibound;
//transpose b
	for(i=0;i<msize;i++) {
	    for(k=0;k<msize;k++) {
		t[i][k] = b[k][i];
		}
	}

    istep = msize / numt;
    ibeg = tidx * istep;
    ibound = ibeg + istep;
/*  for(i=0;i<msize;i+=4) { // use instead for single threaded impl.*/
	for(i=ibeg;i<ibound;i+=4) {
	    for(j=0;j<msize;j+=4) {
#pragma loop count (NUM)
#pragma ivdep
			for(k=0;k<msize;k++) {
				c[i][j] = c[i][j] + a[i][k] * t[j][k];
				c[i+1][j] = c[i+1][j] + a[i+1][k] * t[j][k];
				c[i+2][j] = c[i+2][j] + a[i+2][k] * t[j][k];
				c[i+3][j] = c[i+3][j] + a[i+3][k] * t[j][k];

				c[i][j+1] = c[i][j+1] + a[i][k] * t[j+1][k];
				c[i+1][j+1] = c[i+1][j+1] + a[i+1][k] * t[j+1][k];
				c[i+2][j+1] = c[i+2][j+1] + a[i+2][k] * t[j+1][k];
				c[i+3][j+1] = c[i+3][j+1] + a[i+3][k] * t[j+1][k];

				c[i][j+2] = c[i][j+2] + a[i][k] * t[j+2][k];
				c[i+1][j+2] = c[i+1][j+2] + a[i+1][k] * t[j+2][k];
				c[i+2][j+2] = c[i+2][j+2] + a[i+2][k] * t[j+2][k];
				c[i+3][j+2] = c[i+3][j+2] + a[i+3][k] * t[j+2][k];

				c[i][j+3] = c[i][j+3] + a[i][k] * t[j+3][k];
				c[i+1][j+3] = c[i+1][j+3] + a[i+1][k] * t[j+3][k];
				c[i+2][j+3] = c[i+2][j+3] + a[i+2][k] * t[j+3][k];
				c[i+3][j+3] = c[i+3][j+3] + a[i+3][k] * t[j+3][k];
		  	}
		}
	}

 /*
	// it's the same to the loop above?
	for(i=ibeg;i<ibound;i++) {
	    for(j=0;j<msize;j++) {

#pragma ivdep
#pragma vector aligned

			for(k=0;k<msize;k++) {
				c[i][j] = c[i][j] + a[i][k] * t[j][k];}}}
*/
}
#endif // USE_THR

#ifdef USE_OMP

void multiply0(int msize, int tidx, int numt, TYPE a[][NUM], TYPE b[][NUM], TYPE c[][NUM], TYPE t[][NUM])
{ 
	int i,j,k;

// Basic serial implementation
    for(i=0; i<msize; i++) {
        for(j=0; j<msize; j++) {
    	    for(k=0; k<msize; k++) {
				c[i][j] = c[i][j] + a[i][k] * b[k][j];
			}
		}
	} 
}

void multiply1(int msize, int tidx, int numt, TYPE a[][NUM], TYPE b[][NUM], TYPE c[][NUM], TYPE t[][NUM])
{ 
	int i,j,k;

	// Basic parallel implementation
	#pragma omp parallel for
    for(i=0; i<msize; i++) {
        for(j=0; j<msize; j++) {
    	    for(k=0; k<msize; k++) {
				c[i][j] = c[i][j] + a[i][k] * b[k][j];
			}
		}
	} 
}

void multiply2(int msize, int tidx, int numt, TYPE a[][NUM], TYPE b[][NUM], TYPE c[][NUM], TYPE t[][NUM])
{
	int i,j,k;

	// Parallel with merged outer loops
	#pragma omp parallel for collapse (2)
    for(i=0; i<msize; i++) {
        for(j=0; j<msize; j++) {
    	    for(k=0; k<msize; k++) {
				c[i][j] = c[i][j] + a[i][k] * b[k][j];
			}
		}
	} 
}

void multiply3(int msize, int tidx, int numt, TYPE a[][NUM], TYPE b[][NUM], TYPE c[][NUM], TYPE t[][NUM])
{
	
//#pragma omp target device(0) map(a[0:NUM][0:NUM]) \
  map(b[0:NUM][0:NUM]) map(c[0:NUM][0:NUM])
  //{
    int i,j,k;
	#pragma omp parallel for collapse (2) //num_threads(60)
	for(i=0; i<msize; i++) {
		for(k=0; k<msize; k++) {
			for(j=0; j<msize; j++) {
				c[i][j] = c[i][j] + a[i][k] * b[k][j];
			}
		}
	}
  //}
}

void multiply4(int msize, int tidx, int numt, TYPE a[][NUM], TYPE b[][NUM], TYPE c[][NUM], TYPE t[][NUM])
{
  //loop vectorization with pragma omp simd

  int i,j,k;
  #pragma omp parallel for collapse (2) //num_threads(60)
  for(i=0; i<msize; i++) {
    for(k=0; k<msize; k++) {
      #pragma omp simd 
      for(j=0; j<msize; j++) {
        c[i][j] = c[i][j] + a[i][k] * b[k][j];
      }
    }
  }
}

void multiply5(int msize, int tidx, int numt, TYPE a[][NUM], TYPE b[][NUM], TYPE c[][NUM], TYPE t[][NUM])
{
  //offload and vectorization of loop with pragma omp simd

  #pragma omp target device(0) map(a[0:NUM][0:NUM]) map(b[0:NUM][0:NUM]) map(c[0:NUM][0:NUM])
  {
    int i,j,k;
    #pragma omp parallel for collapse (2) //num_threads(60)
    for(i=0; i<msize; i++) {
      for(k=0; k<msize; k++) {
        #pragma omp simd 
        for(j=0; j<msize; j++) {
          c[i][j] = c[i][j] + a[i][k] * b[k][j];
        }
      }
    }
  }
}

void multiply6(int msize, int tidx, int numt, TYPE a[][NUM], TYPE b[][NUM], TYPE c[][NUM], TYPE t[][NUM])
{
  //#pragma omp parallel for collapse (2) //num_threads(60)
  //offload - thread league - vectorization of loop with pragma omp simd
  int i,j,k;

  #pragma omp target data device(0) map(i , j ,k) map(a[0:NUM][0:NUM]) map(b[0:NUM][0:NUM]) map(c[0:NUM][0:NUM])
  {
    #pragma omp target teams distribute parallel for collapse (2) num_teams(2) thread_limit (30) 
    for(i=0; i<msize; i++) {
      for(k=0; k<msize; k++) {
        #pragma omp simd 
        for(j=0; j<msize; j++) {
          c[i][j] = c[i][j] + a[i][k] * b[k][j];
        }
      }
    } 
  }
}

void multiply7(int msize, int tidx, int numt, TYPE a[][NUM], TYPE b[][NUM], TYPE c[][NUM], TYPE t[][NUM])
{
  //offload - target data - target
  int i,j,k;

  #pragma omp target data map(to:a[0:NUM][0:NUM]) map(i , j ,k)  map(to:b[0:NUM][0:NUM]) map(tofrom:c[0:NUM][0:NUM])
  {
    #pragma omp target 
    {
      #pragma omp parallel for collapse (2) //num_threads(60)
      for(i=0; i<msize; i++) {
        for(k=0; k<msize; k++) {
          #pragma omp simd 
          for(j=0; j<msize; j++) {
            c[i][j] = c[i][j] + a[i][k] * b[k][j];
          }
        }
      } 
    }
    
  }
}

void multiply8(int msize, int tidx, int numt, TYPE a[][NUM], TYPE b[][NUM], TYPE c[][NUM], TYPE t[][NUM])
{
  //offload - target data - target
  int i,j,k;

  #pragma omp target data map(to:a[0:NUM][0:NUM]) map(i , j ,k)  map(to:b[0:NUM][0:NUM]) map(to:c[0:NUM][0:NUM])
  {
    #pragma omp target 
    {
      #pragma omp parallel for collapse (2) //num_threads(60)
      for(i=0; i<msize; i++) {
        for(k=0; k<msize; k++) {
          #pragma omp simd 
          for(j=0; j<msize; j++) {
            c[i][j] = c[i][j] + a[i][k] * b[k][j];
          }
        }
      } 
    }
    #pragma omp target update from(c[0:NUM][0:NUM])
  }
}

void multiply9(int msize, int tidx, int numt, TYPE a[][NUM], TYPE b[][NUM], TYPE c[][NUM], TYPE t[][NUM])
{

//  #pragma omp target device(0) map(a[0:NUM][0:NUM]) \
  //map(b[0:NUM][0:NUM]) map(c[0:NUM][0:NUM])
  //{

    int i, j, repetitions, contrep, auxrand;

    repetitions=1800000;
    contrep=0;
    auxrand=0;

    for(contrep=0; contrep<repetitions; contrep++) {
      auxrand=rand();

      //#pragma omp parallel for collapse (2) //num_threads(60)
      #pragma omp parallel for simd //collapse (2) 
      for(i=0; i<msize; i++) {
        for(j=0; j<msize; j++) {
          a[i][j] = distsq(a[i][j], b[i][j])-auxrand;
          b[i][j] += min(a[i][j], b[i][j])+auxrand;
    	  c[i][j] = (min(distsq(a[i][j], b[i][j]), a[i][j]))/auxrand;
        }
      }
    }
  //}
}


void multiply10(int msize, int tidx, int numt, TYPE a[][NUM], TYPE b[][NUM], TYPE c[][NUM], TYPE t[][NUM])
{

//  #pragma omp target device(0) map(a[0:NUM][0:NUM]) \
//  map(b[0:NUM][0:NUM]) map(c[0:NUM][0:NUM])
  //{

    int i, j, repetitions, contrep, auxrand;

    repetitions=1000000;
    contrep=0;
    auxrand=0;

    for(contrep=0; contrep<repetitions; contrep++) {
      auxrand=rand();
      j=0;
      //#pragma omp parallel for //collapse (2) //num_threads(60)
      //#pragma distribute_point
      #pragma omp parallel for simd //collapse(2)
      for(i=0; i<msize; i++) {
        //for(j=0; j<msize; j++) {
          a[i][j] = distsq(a[i][j], b[i][j])-auxrand;
          b[i][j] += min(a[i][j], b[i][j])+auxrand;
          c[i][j] = (min(distsq(a[i][j], b[i][j]), a[i][j]))/auxrand;
        //}
      }
    }
  //}
}


/*
void multiply5(int msize, int tidx, int numt, TYPE a[][NUM], TYPE b[][NUM], TYPE c[][NUM], TYPE t[][NUM])
{

	int i,j,k;

	#pragma omp parallel for collapse (2)
	for(i=0; i<msize; i++) {
		for(k=0; k<msize; k++) {
#pragma unroll(8)
#pragma ivdep
			for(j=0; j<msize; j++) {
				c[i][j] = c[i][j] + a[i][k] * b[k][j];
			}
		}
	}
}
*/
#endif // USE_OMP
/*
#ifdef USE_MKL
// DGEMM way of matrix multiply using Intel MKL
// Link with Intel MKL library: With MSFT VS and Intel Composer integration: Select build components in the Project context menu.
// For command line - check out the Intel® Math Kernel Library Link Line Advisor 
void multiply5(int msize, int tidx, int numt, TYPE a[][NUM], TYPE b[][NUM], TYPE c[][NUM], TYPE t[][NUM])
{

	double alpha = 1.0, beta = 0.;
	cblas_dgemm(CblasRowMajor,CblasNoTrans,CblasNoTrans,NUM,NUM,NUM,alpha,(const double *)b,NUM,(const double *)a,NUM,beta,(double *)c,NUM);
}
#endif //USE_MKL*/
