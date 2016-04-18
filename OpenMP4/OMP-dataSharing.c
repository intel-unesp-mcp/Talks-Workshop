#include <omp.h>

int main() {

    int thid=0, i=0, J=9, K=10, L=11, M=12;
    printf("J %d -- K %d -- L %d -- M %d \n", J, K, L, M );

    #pragma omp parallel shared (K,L) private(J) 
    {
        #pragma omp for private(J) firstprivate(K) lastprivate(L) 
        for ( i = 0 ; i < 10 ; i++ ) { 
            K = K+i; 
            L = L+i; 
            M = M+i; 
            thid=omp_get_thread_num();
      	    printf(" K %d L %d M %d thid %d\n", K , L, M, thid); 	
        }
        
        thid=omp_get_thread_num();
        J = J + thid;
        printf(" J %d thid %d \n", J , thid); 	
    }	

    printf("J %d -- K %d -- L %d -- M %d \n", J, K, L, M );
}