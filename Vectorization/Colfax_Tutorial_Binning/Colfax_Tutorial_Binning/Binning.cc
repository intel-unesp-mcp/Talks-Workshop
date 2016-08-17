/*
   Educational example: particle binning on Intel architectures.
   (c) Colfax International, 2015

   For more information, refer to the 3-part series of publications
   "Optimization Techniques for the Intel MIC Architecture" 
   at Colfax Research:
      http://colfaxresearch.com/?p=6
      http://colfaxresearch.com/?p=709
      http://colfaxresearch.com/?p=1231

*/

#include <cstdio>
#include <cassert>
#include <omp.h>
#include <cmath>
#include <mkl_vsl.h>
#include <string>

#ifdef DOUBLE_PRECISION
#define FTYPE double
#define SIN sin
#define COS cos
#define VRNG vdRngUniform
#else
#define FTYPE float
#define SIN sinf
#define COS cosf
#define VRNG vsRngUniform
#endif

// Input data arrives as arrays of cylindrical coordinates
// of particles: r and phi. Size of each array is numDataPoints
struct InputDataType {
  int numDataPoints;
  FTYPE* r;
  FTYPE* phi;
};


// The application bins particles defined in InputDataType
// into bins in Cartesian coordinates
const int nBinsX=10;
const int nBinsY=10;
typedef int BinsType[nBinsX][nBinsY];

// We assume that the radial coordinate does not exceed this value
const FTYPE maxMagnitudeR=5.0;

// Boundaries of bins:
const FTYPE xMin = -maxMagnitudeR*1.000001;
const FTYPE xMax = +maxMagnitudeR*1.000001;
const FTYPE yMin = -maxMagnitudeR*1.000001;
const FTYPE yMax = +maxMagnitudeR*1.000001;

// Reciprocal of widths of bins:
const FTYPE binsPerUnitX = (FTYPE)nBinsX/(xMax - xMin);
const FTYPE binsPerUnitY = (FTYPE)nBinsY/(yMax - yMin);



void BinParticlesReference(const InputDataType  & inputData, BinsType & outputBins) {

  // Reference implementation: scalar, serial code without optimization

  // Loop through all particle coordinates
  for (int i = 0; i < inputData.numDataPoints; i++) { 

    // Transforming from cylindrical to Cartesian coordinates:
    const FTYPE x = inputData.r[i]*COS(inputData.phi[i]);
    const FTYPE y = inputData.r[i]*SIN(inputData.phi[i]);

    // Calculating the bin numbers for these coordinates:
    const int iX = int((x - xMin)*binsPerUnitX);
    const int iY = int((y - yMin)*binsPerUnitY);

    // Incrementing the appropriate bin in the counter:
    outputBins[iX][iY]++;
  }

}





void BinParticles_1(const InputDataType  & inputData, BinsType & outputBins) {

  // Thread-parallel implementation with atomics

  // Loop through all particle coordinates
#pragma omp parallel for
  for (int i = 0; i < inputData.numDataPoints; i++) { 

    // Transforming from cylindrical to Cartesian coordinates:
    const FTYPE x = inputData.r[i]*COS(inputData.phi[i]);
    const FTYPE y = inputData.r[i]*SIN(inputData.phi[i]);

    // Calculating the bin numbers for these coordinates:
    const int iX = int((x - xMin)*binsPerUnitX);
    const int iY = int((y - yMin)*binsPerUnitY);

    // Incrementing the appropriate bin in the counter,
    // protecting race condition with atomics
#pragma omp atomic
    outputBins[iX][iY]++;
  }

}




void BinParticles_2(const InputDataType  & inputData, BinsType & outputBins) {

  // Thread-parallel implementation
  // with reduction using thread-private containers

//#pragma omp parallel
  //{
    // Declare thread-private containers for bins
    BinsType threadPrivateBins;
    for (int i = 0; i < nBinsX; i++)
      for (int j = 0; j < nBinsY; j++)
	threadPrivateBins[i][j] = 0;
    
    // Loop through all bunches of particles
//#pragma omp for
    for (int i = 0; i < inputData.numDataPoints; i++) { 

      // Transforming from cylindrical to Cartesian coordinates:
      const FTYPE x = inputData.r[i]*COS(inputData.phi[i]);
      const FTYPE y = inputData.r[i]*SIN(inputData.phi[i]);

	// Calculating the bin numbers for these coordinates:
	const int iX = int((x - xMin)*binsPerUnitX);
	const int iY = int((y - yMin)*binsPerUnitY);

	// Incrementing the appropriate bin in the thread-private counter:
	threadPrivateBins[iX][iY]++;
    }

    // Reduction outside the parallel loop
    for(int i = 0; i < nBinsX; i++) {
      for(int j = 0; j < nBinsY; j++) {
//#pragma omp atomic
	outputBins[i][j] += threadPrivateBins[i][j];
      }
    }
  //}
}



void BinParticles_3(const InputDataType  & inputData, BinsType & outputBins) {

  // Thread-parallel, vectorized implementation 
  // with reduction using thread-private containers

  const int STRIP_WIDTH = 16;

//#pragma omp parallel
 // {
    // Declare thread-private containers for bins
    BinsType threadPrivateBins;
    for (int i = 0; i < nBinsX; i++)
      for (int j = 0; j < nBinsY; j++)
	threadPrivateBins[i][j] = 0;
    
    // Loop through all bunches of particles
//#pragma omp for
    for (int ii = 0; ii < inputData.numDataPoints; ii += STRIP_WIDTH) { 

      int iX[STRIP_WIDTH];
      int iY[STRIP_WIDTH];

      const FTYPE* r   = &(inputData.r[ii]);
      const FTYPE* phi = &(inputData.phi[ii]);

      // Vector loop
      for (int c = 0; c < STRIP_WIDTH; c++) { 
	// Transforming from cylindrical to Cartesian coordinates:
	const FTYPE x = r[c]*COS(phi[c]);
        const FTYPE y = r[c]*SIN(phi[c]);

	// Calculating the bin numbers for these coordinates:
	iX[c] = int((x - xMin)*binsPerUnitX);
	iY[c] = int((y - yMin)*binsPerUnitY);
      }

      // Scalar loop
      for (int c = 0; c < STRIP_WIDTH; c++)
      	threadPrivateBins[iX[c]][iY[c]]++;

    }

    // Reduction outside the parallel loop
    for(int i = 0; i < nBinsX; i++)
      for(int j = 0; j < nBinsY; j++) {
//#pragma omp atomic
	outputBins[i][j] += threadPrivateBins[i][j];
      }
  //}

}


void BinParticles_4(const InputDataType  & inputData, BinsType & outputBins) {

  // Thread-parallel, vectorized implementation with alignment hints
  // with reduction using thread-private containers

  const int STRIP_WIDTH = 16;

//#pragma omp parallel
//  {
    // Declare thread-private containers for bins
    BinsType threadPrivateBins;
    for (int i = 0; i < nBinsX; i++)
      for (int j = 0; j < nBinsY; j++)
	threadPrivateBins[i][j] = 0;
    
    // Loop through all bunches of particles
//#pragma omp for
    for (int ii = 0; ii < inputData.numDataPoints; ii += STRIP_WIDTH) { 

      int iX[STRIP_WIDTH] __attribute__((aligned(64)));
      int iY[STRIP_WIDTH] __attribute__((aligned(64)));

      const FTYPE* r   = &(inputData.r[ii]);
      const FTYPE* phi = &(inputData.phi[ii]);

      // Vector loop
#pragma vector aligned
      for (int c = 0; c < STRIP_WIDTH; c++) { 
	// Transforming from cylindrical to Cartesian coordinates:
	const FTYPE x = r[c]*COS(phi[c]);
        const FTYPE y = r[c]*SIN(phi[c]);

	// Calculating the bin numbers for these coordinates:
	iX[c] = int((x - xMin)*binsPerUnitX);
	iY[c] = int((y - yMin)*binsPerUnitY);
      }

      // Scalar loop
      for (int c = 0; c < STRIP_WIDTH; c++)
      	threadPrivateBins[iX[c]][iY[c]]++;

    }

    // Reduction outside the parallel loop
    for(int i = 0; i < nBinsX; i++)
      for(int j = 0; j < nBinsY; j++) {
//#pragma omp atomic
	outputBins[i][j] += threadPrivateBins[i][j];
      }
  //}

}



void BinParticles_5(const InputDataType  & inputData, BinsType & outputBins) {

  // Thread-parallel, vectorized implementation with reduction 
  // using a global container in threads-first layout 
  // (false sharing occurs)

  const int STRIP_WIDTH = 16;

  const int nThreads = omp_get_max_threads();

  // Instead of storing scalars in each bin,
  // store an array with values for each thread
  int globalBins[nBinsX][nBinsY][nThreads];

#pragma omp parallel
  {
    
    // Find this thread's own region in the global container 
    const int iThread = omp_get_thread_num();

    // Declare thread-private containers for bins
    BinsType threadPrivateBins;
    for (int i = 0; i < nBinsX; i++)
      for (int j = 0; j < nBinsY; j++)
	globalBins[i][j][iThread] = 0;
    
    // Loop through all bunches of particles
#pragma omp for
    for (int ii = 0; ii < inputData.numDataPoints; ii += STRIP_WIDTH) { 

      int iX[STRIP_WIDTH] __attribute__((aligned(64)));
      int iY[STRIP_WIDTH] __attribute__((aligned(64)));

      const FTYPE* r   = &(inputData.r[ii]);
      const FTYPE* phi = &(inputData.phi[ii]);

      // Vector loop
#pragma vector aligned
      for (int c = 0; c < STRIP_WIDTH; c++) { 
	// Transforming from cylindrical to Cartesian coordinates:
	const FTYPE x = r[c]*COS(phi[c]);
        const FTYPE y = r[c]*SIN(phi[c]);

	// Calculating the bin numbers for these coordinates:
	iX[c] = int((x - xMin)*binsPerUnitX);
	iY[c] = int((y - yMin)*binsPerUnitY);
      }

      // Scalar loop
      for (int c = 0; c < STRIP_WIDTH; c++)
      	globalBins[iX[c]][iY[c]][iThread]++;
    }
  }

  // Reduction outside of the parallel region
  for (int iThread = 0; iThread < nThreads; iThread++)
    for(int i = 0; i < nBinsX; i++)
      for(int j = 0; j < nBinsY; j++)
	outputBins[i][j] += globalBins[i][j][iThread];

}


void BinParticles_6(const InputDataType  & inputData, BinsType & outputBins) {

  // Thread-parallel, vectorized implementation with reduction 
  // using a global container in threads-last layout 
  // (false sharing occurs because there is no padding)

  const int STRIP_WIDTH = 16;

  const int nThreads = omp_get_max_threads();
  // Instead of bins with scalars,
  // store an array of bins with scalars
  int globalBins[nThreads][nBinsX][nBinsY];
#pragma omp parallel
  {
    
    // Find this thread's own region in the global container 
    const int iThread = omp_get_thread_num();

    // Declare thread-private containers for bins
    BinsType threadPrivateBins;
    for (int i = 0; i < nBinsX; i++)
      for (int j = 0; j < nBinsY; j++)
	globalBins[iThread][i][j] = 0;
    
    // Loop through all bunches of particles
#pragma omp for
    for (int ii = 0; ii < inputData.numDataPoints; ii += STRIP_WIDTH) { 

      int iX[STRIP_WIDTH] __attribute__((aligned(64)));
      int iY[STRIP_WIDTH] __attribute__((aligned(64)));

      const FTYPE* r   = &(inputData.r[ii]);
      const FTYPE* phi = &(inputData.phi[ii]);

      // Vector loop
#pragma vector aligned
      for (int c = 0; c < STRIP_WIDTH; c++) { 
	// Transforming from cylindrical to Cartesian coordinates:
	const FTYPE x = r[c]*COS(phi[c]);
        const FTYPE y = r[c]*SIN(phi[c]);

	// Calculating the bin numbers for these coordinates:
	iX[c] = int((x - xMin)*binsPerUnitX);
	iY[c] = int((y - yMin)*binsPerUnitY);
      }

      // Scalar loop
      for (int c = 0; c < STRIP_WIDTH; c++)
      	globalBins[iThread][iX[c]][iY[c]]++;
    }
  }

  // Reduction outside of the parallel region
  for (int iThread = 0; iThread < nThreads; iThread++)
    for(int i = 0; i < nBinsX; i++)
      for(int j = 0; j < nBinsY; j++)
	outputBins[i][j] += globalBins[iThread][i][j];

}




void BinParticles_7(const InputDataType  & inputData, BinsType & outputBins) {

  // Thread-parallel, vectorized implementation with reduction 
  // using a global container in threads-last layout with padding
  // (no false sharing)

  const int STRIP_WIDTH = 16;

  const int nThreads = omp_get_max_threads();
  const int containerSize = sizeof(BinsType);
#ifdef __MIC__
  // Padding value for Intel Xeon Phi coprocessors:                                                                                         
  const int paddingBytes = 64;
#else
  // Padding value for Intel Xeon processors:                                                                                               
  const int paddingBytes = 512;
#endif
  const int paddedSize = (containerSize - containerSize%64) + paddingBytes;
  typedef char PaddedBinsType[paddedSize];
  PaddedBinsType globalBins[nThreads];
  // printf("Container size: %d, padding: %d, padded container: %d bytes\n",
  // 	 containerSize, paddingBytes, paddedSize);

#pragma omp parallel
  {
    
    // Find this thread's own region in the global container 
    const int iThread = omp_get_thread_num();
    BinsType& myBins = (BinsType&)(globalBins[iThread]);

    // Declare thread-private containers for bins
    BinsType threadPrivateBins;
    for (int i = 0; i < nBinsX; i++)
      for (int j = 0; j < nBinsY; j++)
	myBins[i][j] = 0;
    
    // Loop through all bunches of particles
#pragma omp for
    for (int ii = 0; ii < inputData.numDataPoints; ii += STRIP_WIDTH) { 

      int iX[STRIP_WIDTH] __attribute__((aligned(64)));
      int iY[STRIP_WIDTH] __attribute__((aligned(64)));

      const FTYPE* r   = &(inputData.r[ii]);
      const FTYPE* phi = &(inputData.phi[ii]);

      // Vector loop
#pragma vector aligned
      for (int c = 0; c < STRIP_WIDTH; c++) { 
	// Transforming from cylindrical to Cartesian coordinates:
	const FTYPE x = r[c]*COS(phi[c]);
        const FTYPE y = r[c]*SIN(phi[c]);

	// Calculating the bin numbers for these coordinates:
	iX[c] = int((x - xMin)*binsPerUnitX);
	iY[c] = int((y - yMin)*binsPerUnitY);
      }

      // Scalar loop
      for (int c = 0; c < STRIP_WIDTH; c++)
      	myBins[iX[c]][iY[c]]++;
    }
  }

  // Reduction outside of the parallel region
  for (int iThread = 0; iThread < nThreads; iThread++)
    for(int i = 0; i < nBinsX; i++)
      for(int j = 0; j < nBinsY; j++)
  	outputBins[i][j] += ((BinsType&)(globalBins[iThread]))[i][j];

}






void ValidateResults(const BinsType & binnedData, const BinsType & binnedDataRef) {

  int valid = 1;

  // Comparing results. Some variation (1e-5) is acceptable due to rounding errors
  for (int i = 0; i < nBinsX; i++)
    for (int j = 0; j < nBinsY; j++)
      if (fabs(binnedData[i][j] - binnedDataRef[i][j]) > 1e-5*(binnedData[i][j] + binnedDataRef[i][j])) 
	valid = 0;

  if (!valid) {
    printf ("\nERROR: optimized algorithm produces incorrect results!\n");
    for (int i = 0; i < nBinsX; i++)
      {
	for (int j = 0; j < nBinsY; j++) 
	  printf("[%d/%d] ", binnedData[i][j], binnedDataRef[i][j]);
	printf("\n");
      }
    exit(1);
  }
}



int main(int argv, char* argc[]){

  double t0, t1;

  // Size of the data sample
  const int n=1<<27;
//  const int n=100;

  // Numerical factor for conversion from wall clock time to performance units
  const double HztoPerf = double(n)*1e-9;

  printf("\nParticle Binning Optimization Demo (%s)\n%s\n%s\n\n",
#ifdef DOUBLE_PRECISION
	 "double precision",
#else
	 "single precision",
#endif
	 "Additional information is available in accompanying papers at http://colfaxresearch.com/\n",
	 "(c) Colfax International, 2015.");

  // Allocating data
  t0 = omp_get_wtime(); printf("Initialization..."); fflush(stdout);
  InputDataType rawData;
  rawData.numDataPoints = n;
  rawData.r   = (FTYPE*) _mm_malloc(sizeof(FTYPE)*n, 64);
  rawData.phi = (FTYPE*) _mm_malloc(sizeof(FTYPE)*n, 64);

  // First touch from a parallel region
#pragma omp parallel for
  for (int i = 0; i < n; i++) {
    rawData.r[i] = 0.0;
    rawData.phi[i] = 0.0;
  }

  BinsType binnedDataRef, binnedData;

  // Initializing counters
  binnedDataRef[:][:] = 0;

  // Initializing arrays to some random values
  VSLStreamStatePtr rnStream;
  vslNewStream( &rnStream, VSL_BRNG_MT19937, 0 );
  VRNG(VSL_RNG_METHOD_UNIFORM_STD, rnStream, n, rawData.r,   0.0, maxMagnitudeR);
  VRNG(VSL_RNG_METHOD_UNIFORM_STD, rnStream, n, rawData.phi, 0.0, 2.0*M_PI);

  t1 = omp_get_wtime(); printf(" done in %.3f seconds.\n", t1-t0); fflush(stdout);

  // Computing the correct answer with the reference implementation to validate optimized implementations
  t0 = omp_get_wtime(); printf("Computing reference result..."); fflush(stdout);
  BinParticlesReference(rawData, binnedDataRef);
  t1 = omp_get_wtime(); printf(" done in %.3f seconds.\n", t1-t0); fflush(stdout);
  const double refTime = t1 - t0;

  const int nImplementations = 8;

  const std::string impName[nImplementations] = {
    "serial",  // reference
    "parallel, atomics for reduction",  // 1
    "parallel, thread-private containers for reduction",  // 2
    "vectorized",  // 3
    "vectorized, aligned",  // 4
    "global container for reduction in threads-first layout",  // 5
    "global container for reduction in threads-last layout",  // 6
    "global container for reduction in threads-last layout with padding"  // 7
  };

  void(*BinParticlesImplementation[nImplementations]) ( const InputDataType&, BinsType& ) = {
    BinParticlesReference,
    BinParticles_1,
    BinParticles_2,
    BinParticles_3,
    BinParticles_4,
    BinParticles_5,
    BinParticles_6,
    BinParticles_7
  };

  void(*BinParticles) ( const InputDataType&, BinsType& );

  printf("Benchmarking...\n"); fflush(stdout);

  double scalarPerf;

  //for (int implementation = 0; implementation < nImplementations; implementation++) {
  int implementation = 3;

    BinParticles = BinParticlesImplementation[implementation];

    printf("\n\n%s\n\033[1mIMPLEMENTATION %d: %s\033[0m\n\n", 
	   std::string(90,'=').c_str(), implementation, impName[implementation].c_str());

    // Computing the binning with the accelerated code:
    printf("\033[1m%5s %10s %10s %10s %s\033[0m\n", "Trial", "Time, s", "Speedup", "GP/s", "*");

    const int nTrials = 10;
    const int skipTrials = 1;
    //const int nTrials = 3;
    //const int skipTrials = 1;
    double perf = 0.0, dperf = 0.0;

    for (int t=1; t<=nTrials; t++){
    
      // Reset counters
      binnedData[:][:] = 0;

      // Compute and benchmark
      t0 = omp_get_wtime();
      BinParticles(rawData, binnedData);
      t1 = omp_get_wtime();

      // Validating results
      if (t == 1) ValidateResults(binnedData, binnedDataRef);
      
      // Translate wall clock time to performance units
      if (t > skipTrials) {
	// Ignore the first few trials: those are warm-up
	perf += HztoPerf/(t1-t0);
	dperf += HztoPerf*HztoPerf/((t1-t0)*(t1-t0));
      }
      
      if (implementation==0) {
	printf("%5d %10.3e %10s %10.2e %s\n", 
	       t, (t1-t0), "n/a", HztoPerf/(t1-t0), (t<=skipTrials?"**":""));
      } else {
	printf("%5d %10.3e %10.2f %10.2e %s\n", 
	       t, (t1-t0), HztoPerf/((t1-t0)*scalarPerf), HztoPerf/(t1-t0), (t<=skipTrials?"**":""));
      }
    
      fflush(stdout);
      
    }
    perf/=(double)(nTrials-skipTrials); 
    dperf=sqrt(dperf/(double)(nTrials-skipTrials)-perf*perf);
    if (implementation==0) scalarPerf=perf;
    printf("---------------------------------------------------------\n");
    printf("\033[1m%s%7.2f \033[42m%10.2e +- %.2e GP/s\033[0m\n",
	   "Average performance:", perf/scalarPerf, perf, dperf);
    printf("---------------------------------------------------------\n");
    printf("*  - Performance unit 1 GP/s is 10^9 particles binned per second.\n"); 
    printf("** - warm-up, not included in average\n\n"); fflush(stdout);


  //}

  _mm_free(rawData.r);
  _mm_free(rawData.phi);
}
