#include <stdio.h>
#include <unistd.h>

int main() {
    printf("Host have %d logical cores.\n", sysconf(_SC_NPROCESSORS_ONLN)); 
    fflush(0);

    #pragma offload target(mic)
    {
        printf("Co-Processor have %ld logical cores.\n",
                sysconf(_SC_NPROCESSORS_ONLN )); 
        fflush(0);
    }



}
