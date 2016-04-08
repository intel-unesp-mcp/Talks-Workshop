# OpenMP 4.0

Demonstrations <p>

* OpenMP Sample Program
* Vectorization
* Offload
** Target Data
** Target Update
* Thread League
* N-Body

# 1. OpenMP Sample Program

Build the application for Xeon (without OpenMP Option)
```
icc OMP-hello.c -o OMP-hello
```

The OpenMP pragmas are ignored by compiler:
```
OMP-hello.c(10): warning #3180: unrecognized OpenMP #pragma
      #pragma omp parallel
              ^

OMP-hello.c(19): warning #3180: unrecognized OpenMP #pragma
      #pragma omp for
              ^

OMP-hello.c(24): warning #3180: unrecognized OpenMP #pragma
      #pragma omp for
              ^
```

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

Examples using vectorization pragmas.

# SIMD (Matrix Multiplication)

Running Matrix Multiplication Example without #pragma omp simd

```
cd matrix/linux/
vim ../src/multiply.h
change line 38 to: #define MULTIPLY multiply4
make icc
export OMP_NUM_THREADS=36 
export KMP_AFFINITY=scatter
time ./matrix.icc
```

Results:

```
Threads #: 36 OpenMP threads <p>
Matrix size: 10240 <p>
Using multiply kernel: multiply3 <p>
Freq = 1.998753 GHz <p>
Execution time = 64.138 seconds <p>

real    1m5.096s 
user    33m24.241s 
sys     0m4.421s 
```

Running Matrix Multiplication Example with #pragma omp simd

```
cd matrix/linux/
vim ../src/multiply.h
change line 38 to: #define MULTIPLY multiply5
make icc
export OMP_NUM_THREADS=36 
export KMP_AFFINITY=scatter
time ./matrix.icc
```

Results:

```
Threads #: 36 OpenMP threads <p>
Matrix size: 10240 <p>
Using multiply kernel: multiply3 <p>
Freq = 2.347527 GHz <p>
Execution time = 46.375 seconds <p>
 
real    0m56.772s <p>
user    28m19.769s <p>
sys     0m3.596s <p>
```

# Simd function (Interpolation)

Running Interpolation without vectorized functions

```
make clean
make OMP=no
time ./main
```

Results:

```
Test completed in 1053563 nsec <p>

real    0m21.074s <p>
user    0m21.059s <p>
sys     0m0.004s <p>
```
Running Interpolation without vectorized functions

```
make clean
make OMP=yes
time ./main
```
Results:
```
Test completed in 342512 nsec <p>

real    0m6.853s <p>
user    0m6.849s <p>
sys     0m0.001s <p>
```

# Thread and Simd

```
icc matrixompforsimd.c -o matrixompforsimd -fopenmp
time ./matrixompforsimd
```

# 3. Offload

Examples using offload pragmas.

## target

multiply5 - execute matrix multiplication on Xeon Card.

```
export MIC_OMP_NUM_THREADS=120
export MIC_KMP_AFFINITY=scatter
export OFFLOAD_REPORT=2
./matrix.icc
```

target

```
export MIC_OMP_NUM_THREADS=120
export MIC_KMP_AFFINITY=scatter
export OFFLOAD_REPORT=2
./matrix.icc
```
Offload Report and Results:

```
Matrix size: 10240 <p>
Using multiply kernel: multiply5 <p>
[Offload] [MIC 0] [File]                    ../src/multiply.c 
[Offload] [MIC 0] [Line]                    243 
[Offload] [MIC 0] [Tag]                     Tag 0 
[Offload] [HOST]  [Tag 0] [CPU Time]        37.847054(seconds) 
[Offload] [MIC 0] [Tag 0] [CPU->MIC Data]   2516582428 (bytes) 
[Offload] [MIC 0] [Tag 0] [MIC Time]        46.479576(seconds) 
[Offload] [MIC 0] [Tag 0] [MIC->CPU Data]   2516582404 (bytes) 

Freq = 3.588089 GHz 
Execution time = 37.973 seconds 

real    1m9.208s 
user    0m51.274s
sys     0m1.915s 
```

# 4. Target Data

```
time ./matrix.icc
```
Offload Report and Results:

```
Using multiply kernel: multiply7 <p>
[Offload] [MIC 0] [File]                    ../src/multiply.c <p>
[Offload] [MIC 0] [Line]                    284 <p>
[Offload] [MIC 0] [Tag]                     Tag 0 <p>
[Offload] [HOST]  [Tag 0] [CPU Time]        5.487947(seconds) <p>
[Offload] [MIC 0] [Tag 0] [CPU->MIC Data]   2516582436 (bytes) <p>
[Offload] [MIC 0] [Tag 0] [MIC Time]        0.000127(seconds) <p>
[Offload] [MIC 0] [Tag 0] [MIC->CPU Data]   0 (bytes) <p>

[Offload] [MIC 0] [File]                    ../src/multiply.c <p>
[Offload] [MIC 0] [Line]                    286 <p>
[Offload] [MIC 0] [Tag]                     Tag 1 <p>
[Offload] [HOST]  [Tag 1] [CPU Time]        32.173884(seconds) <p>
[Offload] [MIC 0] [Tag 1] [CPU->MIC Data]   36 (bytes) <p>
[Offload] [MIC 0] [Tag 1] [MIC Time]        46.540515(seconds) <p>
[Offload] [MIC 0] [Tag 1] [MIC->CPU Data]   4 (bytes) <p>

[Offload] [MIC 0] [File]                    ../src/multiply.c <p>
[Offload] [MIC 0] [Line]                    299 <p>
[Offload] [MIC 0] [Tag]                     Tag 2 <p>
[Offload] [HOST]  [Tag 2] [CPU Time]        0.082984(seconds) <p>
[Offload] [MIC 0] [Tag 2] [CPU->MIC Data]   56 (bytes) <p>
[Offload] [MIC 0] [Tag 2] [MIC Time]        0.000060(seconds) <p>
[Offload] [MIC 0] [Tag 2] [MIC->CPU Data]   838860812 (bytes) <p>

Freq = 3.582878 GHz <p>
Execution time = 37.925 seconds <p>

real    1m8.707s <p>
user    0m50.756s <p>
sys     0m1.591s <p>
```

# 5. Target Update

target update

```
time ./matrix.icc
```
Offload Report and Results:
```
Matrix size: 10240 <p>
Using multiply kernel: multiply8 <p>
[Offload] [MIC 0] [File]                    ../src/multiply.c <p>
[Offload] [MIC 0] [Line]                    307 <p>
[Offload] [MIC 0] [Tag]                     Tag 0 <p>
[Offload] [HOST]  [Tag 0] [CPU Time]        5.417288(seconds) <p>
[Offload] [MIC 0] [Tag 0] [CPU->MIC Data]   2516582436 (bytes) <p>
[Offload] [MIC 0] [Tag 0] [MIC Time]        0.000132(seconds) <p>
[Offload] [MIC 0] [Tag 0] [MIC->CPU Data]   0 (bytes) <p>

[Offload] [MIC 0] [File]                    ../src/multiply.c <p>
[Offload] [MIC 0] [Line]                    309 <p>
[Offload] [MIC 0] [Tag]                     Tag 1 <p>
[Offload] [HOST]  [Tag 1] [CPU Time]        30.656746(seconds) <p>
[Offload] [MIC 0] [Tag 1] [CPU->MIC Data]   36 (bytes) <p>
[Offload] [MIC 0] [Tag 1] [MIC Time]        44.345895(seconds) <p>
[Offload] [MIC 0] [Tag 1] [MIC->CPU Data]   4 (bytes) <p>

[Offload] [MIC 0] [File]                    ../src/multiply.c <p>
[Offload] [MIC 0] [Line]                    321 <p>
[Offload] [MIC 0] [Tag]                     Tag 2 <p>
[Offload] [HOST]  [Tag 2] [CPU Time]        0.080083(seconds) <p>
[Offload] [MIC 0] [Tag 2] [CPU->MIC Data]   0 (bytes) <p>
[Offload] [MIC 0] [Tag 2] [MIC Time]        0.000000(seconds) <p>
[Offload] [MIC 0] [Tag 2] [MIC->CPU Data]   838860800 (bytes) <p>

[Offload] [MIC 0] [File]                    ../src/multiply.c <p>
[Offload] [MIC 0] [Line]                    322 <p>
[Offload] [MIC 0] [Tag]                     Tag 3 <p>
[Offload] [HOST]  [Tag 3] [CPU Time]        0.088541(seconds) <p>
[Offload] [MIC 0] [Tag 3] [CPU->MIC Data]   56 (bytes) <p>
[Offload] [MIC 0] [Tag 3] [MIC Time]        0.000084(seconds) <p>
[Offload] [MIC 0] [Tag 3] [MIC->CPU Data]   838860812 (bytes) <p>

Freq = 3.594468 GHz <p>
Execution time = 36.299 seconds <p>

real    1m6.286s <p>
user    0m48.385s <p>
sys     0m1.665s <p>
```

# 6. Thread League

```
time ./matrix.icc
```

Offload Report and Results:
```
Using multiply kernel: multiply6 <p>
[Offload] [MIC 0] [File]                    ../src/multiply.c <p>
[Offload] [MIC 0] [Line]                    264 <p>
[Offload] [MIC 0] [Tag]                     Tag 0 <p>
[Offload] [HOST]  [Tag 0] [CPU Time]        5.436198(seconds) <p>
[Offload] [MIC 0] [Tag 0] [CPU->MIC Data]   2516582436 (bytes) <p>
[Offload] [MIC 0] [Tag 0] [MIC Time]        0.000143(seconds) <p>
[Offload] [MIC 0] [Tag 0] [MIC->CPU Data]   0 (bytes) <p>

[Offload] [MIC 0] [File]                    ../src/multiply.c <p>
[Offload] [MIC 0] [Line]                    266 <p>
[Offload] [MIC 0] [Tag]                     Tag 1 <p>
[Offload] [HOST]  [Tag 1] [CPU Time]        47.980882(seconds) <p>
[Offload] [MIC 0] [Tag 1] [CPU->MIC Data]   44 (bytes) <p>
[Offload] [MIC 0] [Tag 1] [MIC Time]        69.405908(seconds) <p>
[Offload] [MIC 0] [Tag 1] [MIC->CPU Data]   12 (bytes) <p>

[Offload] [MIC 0] [File]                    ../src/multiply.c <p>
[Offload] [MIC 0] [Line]                    276 <p>
[Offload] [MIC 0] [Tag]                     Tag 2 <p>
[Offload] [HOST]  [Tag 2] [CPU Time]        0.243274(seconds) <p> 
[Offload] [MIC 0] [Tag 2] [CPU->MIC Data]   56 (bytes) <p>
[Offload] [MIC 0] [Tag 2] [MIC Time]        0.000054(seconds) <p>
[Offload] [MIC 0] [Tag 2] [MIC->CPU Data]   2516582412 (bytes) <p>

Freq = 3.566257 GHz <p>
Execution time = 54.168 seconds <p>

real    1m33.892s <p>
user    1m15.744s <p>
sys     0m1.846s <p>
```

# 7. N-Body

Execute N-body paralelized for Host
```
time ./nbody-v0d
```
Offload Report and Results:
```
ff
```
Execute N-body with load balacing between host and devices
```
time ./nbody-v3d
```
Offload Report and Results:
```
ff
```
