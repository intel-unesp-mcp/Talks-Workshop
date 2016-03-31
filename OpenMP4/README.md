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

Matrix size: 10240
Using multiply kernel: multiply5
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

# 4. Target Data

```
time ./matrix.icc
```
Using multiply kernel: multiply7
[Offload] [MIC 0] [File]                    ../src/multiply.c
[Offload] [MIC 0] [Line]                    284
[Offload] [MIC 0] [Tag]                     Tag 0
[Offload] [HOST]  [Tag 0] [CPU Time]        5.487947(seconds)
[Offload] [MIC 0] [Tag 0] [CPU->MIC Data]   2516582436 (bytes)
[Offload] [MIC 0] [Tag 0] [MIC Time]        0.000127(seconds)
[Offload] [MIC 0] [Tag 0] [MIC->CPU Data]   0 (bytes)

[Offload] [MIC 0] [File]                    ../src/multiply.c
[Offload] [MIC 0] [Line]                    286
[Offload] [MIC 0] [Tag]                     Tag 1
[Offload] [HOST]  [Tag 1] [CPU Time]        32.173884(seconds)
[Offload] [MIC 0] [Tag 1] [CPU->MIC Data]   36 (bytes)
[Offload] [MIC 0] [Tag 1] [MIC Time]        46.540515(seconds)
[Offload] [MIC 0] [Tag 1] [MIC->CPU Data]   4 (bytes)

[Offload] [MIC 0] [File]                    ../src/multiply.c
[Offload] [MIC 0] [Line]                    299
[Offload] [MIC 0] [Tag]                     Tag 2
[Offload] [HOST]  [Tag 2] [CPU Time]        0.082984(seconds)
[Offload] [MIC 0] [Tag 2] [CPU->MIC Data]   56 (bytes)
[Offload] [MIC 0] [Tag 2] [MIC Time]        0.000060(seconds)
[Offload] [MIC 0] [Tag 2] [MIC->CPU Data]   838860812 (bytes)

Freq = 3.582878 GHz
Execution time = 37.925 seconds

real    1m8.707s
user    0m50.756s
sys     0m1.591s

# 5. Target Update
target update

```
time ./matrix.icc
```

Matrix size: 10240
Using multiply kernel: multiply8
[Offload] [MIC 0] [File]                    ../src/multiply.c
[Offload] [MIC 0] [Line]                    307
[Offload] [MIC 0] [Tag]                     Tag 0
[Offload] [HOST]  [Tag 0] [CPU Time]        5.417288(seconds)
[Offload] [MIC 0] [Tag 0] [CPU->MIC Data]   2516582436 (bytes)
[Offload] [MIC 0] [Tag 0] [MIC Time]        0.000132(seconds)
[Offload] [MIC 0] [Tag 0] [MIC->CPU Data]   0 (bytes)

[Offload] [MIC 0] [File]                    ../src/multiply.c
[Offload] [MIC 0] [Line]                    309
[Offload] [MIC 0] [Tag]                     Tag 1
[Offload] [HOST]  [Tag 1] [CPU Time]        30.656746(seconds)
[Offload] [MIC 0] [Tag 1] [CPU->MIC Data]   36 (bytes)
[Offload] [MIC 0] [Tag 1] [MIC Time]        44.345895(seconds)
[Offload] [MIC 0] [Tag 1] [MIC->CPU Data]   4 (bytes)

[Offload] [MIC 0] [File]                    ../src/multiply.c
[Offload] [MIC 0] [Line]                    321
[Offload] [MIC 0] [Tag]                     Tag 2
[Offload] [HOST]  [Tag 2] [CPU Time]        0.080083(seconds)
[Offload] [MIC 0] [Tag 2] [CPU->MIC Data]   0 (bytes)
[Offload] [MIC 0] [Tag 2] [MIC Time]        0.000000(seconds)
[Offload] [MIC 0] [Tag 2] [MIC->CPU Data]   838860800 (bytes)

[Offload] [MIC 0] [File]                    ../src/multiply.c
[Offload] [MIC 0] [Line]                    322
[Offload] [MIC 0] [Tag]                     Tag 3
[Offload] [HOST]  [Tag 3] [CPU Time]        0.088541(seconds)
[Offload] [MIC 0] [Tag 3] [CPU->MIC Data]   56 (bytes)
[Offload] [MIC 0] [Tag 3] [MIC Time]        0.000084(seconds)
[Offload] [MIC 0] [Tag 3] [MIC->CPU Data]   838860812 (bytes)

Freq = 3.594468 GHz
Execution time = 36.299 seconds

real    1m6.286s
user    0m48.385s
sys     0m1.665s


# 6. Thread League

```
time ./matrix.icc
```

Using multiply kernel: multiply6
[Offload] [MIC 0] [File]                    ../src/multiply.c
[Offload] [MIC 0] [Line]                    264
[Offload] [MIC 0] [Tag]                     Tag 0
[Offload] [HOST]  [Tag 0] [CPU Time]        5.436198(seconds)
[Offload] [MIC 0] [Tag 0] [CPU->MIC Data]   2516582436 (bytes)
[Offload] [MIC 0] [Tag 0] [MIC Time]        0.000143(seconds)
[Offload] [MIC 0] [Tag 0] [MIC->CPU Data]   0 (bytes)

[Offload] [MIC 0] [File]                    ../src/multiply.c
[Offload] [MIC 0] [Line]                    266
[Offload] [MIC 0] [Tag]                     Tag 1
[Offload] [HOST]  [Tag 1] [CPU Time]        47.980882(seconds)
[Offload] [MIC 0] [Tag 1] [CPU->MIC Data]   44 (bytes)
[Offload] [MIC 0] [Tag 1] [MIC Time]        69.405908(seconds)
[Offload] [MIC 0] [Tag 1] [MIC->CPU Data]   12 (bytes)

[Offload] [MIC 0] [File]                    ../src/multiply.c
[Offload] [MIC 0] [Line]                    276
[Offload] [MIC 0] [Tag]                     Tag 2
[Offload] [HOST]  [Tag 2] [CPU Time]        0.243274(seconds)
[Offload] [MIC 0] [Tag 2] [CPU->MIC Data]   56 (bytes)
[Offload] [MIC 0] [Tag 2] [MIC Time]        0.000054(seconds)
[Offload] [MIC 0] [Tag 2] [MIC->CPU Data]   2516582412 (bytes)

Freq = 3.566257 GHz
Execution time = 54.168 seconds

real    1m33.892s
user    1m15.744s
sys     0m1.846s
