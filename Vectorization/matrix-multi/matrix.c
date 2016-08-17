void init(double* a, double* b, double* c, int msize, double randV) {
  int i,j;

  for (i = 0; i < msize; i++) {
    for (j = 0; j < msize; j++) {
      a[i*msize + j] = (i+j)-randV;
      b[i*msize + j] = (i-j)*randV;
      c[i*msize + j] = (i+j)+randV;
    }
  }

}

void sum(double* a, double* b, double* c, int msize) {

  int i,j;

  for(i=0; i<msize; i++) {
    #pragma vector aligned
    for(j=0; j<msize; j++) {
      c[i*msize+j] = a[i*msize+j] + b[i*msize+j];
    }
  }

}

void sumLoopTiling(double* a, double* b, double* c, int msize) {

  int i,j, ii, jj, block_size;
  block_size=512;

  for (i=0; i< msize; i+=block_size) {
    for (j=0; j< msize; j+=block_size) {
      for (ii=i; ii<i+block_size; ii++) {
        #pragma vector aligned 
        for (jj=j; jj<j+block_size; jj++) {
          c[ii*msize+jj] = a[ii*msize+jj] + b[ii*msize+jj];
        }
      }
    }
  }


}


void multv1(double* a, double* b, double* c, int msize) {
  int i,j, k;

  for(i=0; i<msize; i++) {
      for(k=0; k<msize; k++) {
    for(j=0; j<msize; j++) {
        c[i*msize+j] += a[i*msize+k] * b[k*msize+j];
      }
    }
  } 
}

void multv2(double* a, double* b, double* c, int msize) {
  int i,j, k;

  for(i=0; i<msize; i++) {
    for(j=0; j<msize; j++) {
	  #pragma ivdep
      for(k=0; k<msize; k++) {
          c[i*msize+j] += a[i*msize+k] * b[k*msize+j];
	}
    }
  } 
}

void multv3(double* a, double* b, double* c, int msize) {
  int i,j, k;

  for(i=0; i<msize; i++) {
      for(k=0; k<msize; k++) {
	  #pragma ivdep
          #pragma vector aligned
	  for(j=0; j<msize; j++) {
          c[i*msize+j] += a[i*msize+k] * b[k*msize+j];
	}
    }
  } 
}

void multv4(double* a, double* b, double* c, int msize) {

  #define BSIZE 64
  #define bmin(x,y) ((x)<(y)?x:y) 
	
  int i,j, k, kl, jl;

  for(kl=0; kl<msize; kl+=BSIZE) {
    for(jl=0; jl<msize; jl+=BSIZE) {
      for(i=0; i<msize; i++) {
        for(k=kl; k<bmin(kl+BSIZE,msize); k++) {
          #pragma ivdep
          #pragma vector aligned
	  for(j=jl; j<bmin(jl+BSIZE,msize); j++) {
            c[i*msize+j] += a[i*msize+k] * b[k*msize+j];
  	  } 
        }
      }
    }
  } 

}

