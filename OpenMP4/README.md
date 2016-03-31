# OpenMP 4.0

# 1. OpenMP Sample Program

Build the application for Xeon
```
icc OMP-hello.c -o OMP-hello -fopenmp
```

Build the application for Coprocessor
```
icc OMP-hello.c -o OMP-hello.mic -mmic -fopenmp
```

Launch the application on Xeon
```
./OMP-hello
```

Launch the application on the Coprocessor from host
```
micnativeloadex ./OMP-hello.mic -e "LD_LIBRARY_PATH=/opt/intel/lib/mic/"
```

# 2. Vectorization

# simd (Matrix Multiplication)
```
export OMP_NUM_THREADS=36 
export KMP_AFFINITY=scatter
time ./matrix.icc
```

Threads #: 36 OpenMP threads
Matrix size: 10240
Using multiply kernel: multiply3
Freq = 2.347527 GHz
Execution time = 46.375 seconds

real    0m56.772s
user    28m19.769s
sys     0m3.596s

Without #pragma omp simd

Threads #: 36 OpenMP threads
Matrix size: 10240
Using multiply kernel: multiply3
Freq = 1.998753 GHz
Execution time = 64.138 seconds

real    1m5.096s
user    33m24.241s
sys     0m4.421s

# Simd function (interpolation)

```
make clean
make OMP=no
time ./main
```
Test completed in 1053563 nsec

real    0m21.074s
user    0m21.059s
sys     0m0.004s

```
make clean
make OMP=yes
time ./main
```

Test completed in 342512 nsec

real    0m6.853s
user    0m6.849s
sys     0m0.001s

# Thread and Simd

```
icc matrixompforsimd.c -o matrixompforsimd -fopenmp
time ./matrixompforsimd
```

# 3. Offload

# 4. Transfer block

# 5. Update Data

# 6. Thread League

