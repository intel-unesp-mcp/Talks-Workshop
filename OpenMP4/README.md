# OpenMP 4.0

Demonstrations <p>

* OpenMP Sample Program
* Vectorization
* Offload
* Target 
* Target Data
* Target Update
* Thread League
* N-Body 

The examples are based on Matrix Multiplication Intel Vtune and lotsofcore book chapters.

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
export OMP_NUM_THREADS=10
./OMP-hello
```

Results:

```
hello from hostname phi02.ncc.unesp.br
hello from hostname phi02.ncc.unesp.br
hello from hostname phi02.ncc.unesp.br
hello from hostname phi02.ncc.unesp.br
hello from hostname phi02.ncc.unesp.br
hello from hostname phi02.ncc.unesp.br
hello from hostname phi02.ncc.unesp.br
hello from hostname phi02.ncc.unesp.br
hello from hostname phi02.ncc.unesp.br
hello from hostname phi02.ncc.unesp.br
```

Launch the application on the Coprocessor from host
```
micnativeloadex ./OMP-hello.mic -e "OMP_NUM_THREADS=10 LD_LIBRARY_PATH=/opt/intel/lib/mic/"

```

Results:

```
hello from hostname phi02-mic0.ncc.unesp.br
hello from hostname phi02-mic0.ncc.unesp.br
hello from hostname phi02-mic0.ncc.unesp.br
hello from hostname phi02-mic0.ncc.unesp.br
hello from hostname phi02-mic0.ncc.unesp.br
hello from hostname phi02-mic0.ncc.unesp.br
hello from hostname phi02-mic0.ncc.unesp.br
hello from hostname phi02-mic0.ncc.unesp.br
hello from hostname phi02-mic0.ncc.unesp.br
hello from hostname phi02-mic0.ncc.unesp.br
sum of vector elements: 5789.473684
```

Thread Affinity

compact xeon
```
export KMP_AFFINITY=compact,verbose
./OMP_hello
```
compact xeon phi

```
micnativeloadex ./OMP-hello.mic -e "KMP_AFFINITY=compact,verbose OMP_NUM_THREADS=10 LD_LIBRARY_PATH=/opt/intel/lib/mic/"
```

scatter xeon
```
export KMP_AFFINITY=scatter,verbose
./OMP_hello
```

scatter xeon phi

```
micnativeloadex ./OMP-hello.mic -e "KMP_AFFINITY=scatter,verbose OMP_NUM_THREADS=10 LD_LIBRARY_PATH=/opt/intel/lib/mic/"
```

balanced xeon phi

```
micnativeloadex ./OMP-hello.mic -e "KMP_AFFINITY=balanced,verbose OMP_NUM_THREADS=10 LD_LIBRARY_PATH=/opt/intel/lib/mic/"
```



# 2. Vectorization

Examples using vectorization pragmas.

# SIMD (Matrix Multiplication)

Running Matrix Multiplication Example without #pragma omp simd

```
cd matrix/linux/
vim ../src/multiply.h
change to multiply3 :  #define MULTIPLY multiply3
make icc
export OMP_NUM_THREADS=36 
export KMP_AFFINITY=scatter
time ./matrix.icc
```

Results:

```
Matrix size: 10240
Using multiply kernel: multiply3
Freq = 1.199953 GHz
Execution time = 108.302 seconds

real	1m5.260s
user	33m5.603s
sys	0m4.578s
```

Running Matrix Multiplication Example with #pragma omp simd

```
cd matrix/linux/
vim ../src/multiply.h
change to multiply4 :  #define MULTIPLY multiply4
make icc
export OMP_NUM_THREADS=36 
export KMP_AFFINITY=scatter
time ./matrix.icc
```

Results:

```
Threads #: 36 OpenMP threads
Matrix size: 10240
Using multiply kernel: multiply4
Freq = 1.894894 GHz
Execution time = 57.440 seconds

real	0m55.969s
user	28m16.808s
sys	0m3.910s
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
Test completed in 1053563 nsec

real    0m21.074s 
user    0m21.059s
sys     0m0.004s 
```
Running Interpolation with vectorized functions

```
make clean
make OMP=yes
time ./main
```
Results:
```
Test completed in 342512 nsec <p>

real    0m6.853s 
user    0m6.849s 
sys     0m0.001s 
```

# Thread and Simd

```
cd matrix/linux/
vim ../src/multiply.h
change to multiply10 :  #define MULTIPLY multiply10
make icc
export OMP_NUM_THREADS=36 
export KMP_AFFINITY=scatter
time ./matrix.icc
```

Results:

```
Threads #: 36 OpenMP threads
Matrix size: 10240
Using multiply kernel: multiply10
^[[6~Freq = 2.799980 GHz
Execution time = 35.359 seconds

real	0m52.662s
user	25m59.963s
sys	0m2.288s
```

# 3. Offload

Examples using offload pragmas.

# 4. Target

```
cd matrix/linux/
vim ../src/multiply.h
change to multiply5 :  #define MULTIPLY multiply5
make icc
export OMP_NUM_THREADS=36 
export KMP_AFFINITY=scatter
time ./matrix.icc
```

Offload Report and Results:

```
Threads #: 36 OpenMP threads
Matrix size: 1024
Using multiply kernel: multiply5
[Offload] [MIC 0] [File]                    ../src/multiply.c
[Offload] [MIC 0] [Line]                    257
[Offload] [MIC 0] [Tag]                     Tag 0
[Offload] [HOST]  [Tag 0] [CPU Time]        0.745028(seconds)
[Offload] [MIC 0] [Tag 0] [CPU->MIC Data]   25165852 (bytes)
[Offload] [MIC 0] [Tag 0] [MIC Time]        0.310357(seconds)
[Offload] [MIC 0] [Tag 0] [MIC->CPU Data]   25165828 (bytes)

Freq = 2.299910 GHz
Execution time = 1.166 seconds

real	0m8.679s
user	0m0.396s
sys	0m0.158s
```

# 5. Target Data

```
cd matrix/linux/
vim ../src/multiply.h
change to multiply7 :  #define MULTIPLY multiply7
make icc
export OMP_NUM_THREADS=36 
export KMP_AFFINITY=scatter
time ./matrix.icc
```

Offload Report and Results:

```
Threads #: 36 OpenMP threads
Matrix size: 1024
Using multiply kernel: multiply7
[Offload] [MIC 0] [File]                    ../src/multiply.c
[Offload] [MIC 0] [Line]                    298
[Offload] [MIC 0] [Tag]                     Tag 0
[Offload] [HOST]  [Tag 0] [CPU Time]        0.521643(seconds)
[Offload] [MIC 0] [Tag 0] [CPU->MIC Data]   25165860 (bytes)
[Offload] [MIC 0] [Tag 0] [MIC Time]        0.000117(seconds)
[Offload] [MIC 0] [Tag 0] [MIC->CPU Data]   0 (bytes)

[Offload] [MIC 0] [File]                    ../src/multiply.c
[Offload] [MIC 0] [Line]                    300
[Offload] [MIC 0] [Tag]                     Tag 1
[Offload] [HOST]  [Tag 1] [CPU Time]        0.212695(seconds)
[Offload] [MIC 0] [Tag 1] [CPU->MIC Data]   36 (bytes)
[Offload] [MIC 0] [Tag 1] [MIC Time]        0.307464(seconds)
[Offload] [MIC 0] [Tag 1] [MIC->CPU Data]   4 (bytes)

[Offload] [MIC 0] [File]                    ../src/multiply.c
[Offload] [MIC 0] [Line]                    313
[Offload] [MIC 0] [Tag]                     Tag 2
[Offload] [HOST]  [Tag 2] [CPU Time]        0.001595(seconds)
[Offload] [MIC 0] [Tag 2] [CPU->MIC Data]   56 (bytes)
[Offload] [MIC 0] [Tag 2] [MIC Time]        0.000067(seconds)
[Offload] [MIC 0] [Tag 2] [MIC->CPU Data]   8388620 (bytes)

Freq = 2.300000 GHz
Execution time = 1.152 seconds

real	0m11.689s
user	0m0.384s
sys	0m0.154s
```

# 6. Target Update

```
cd matrix/linux/
vim ../src/multiply.h
change to multiply8 :  #define MULTIPLY multiply8
make icc
export OMP_NUM_THREADS=36 
export KMP_AFFINITY=scatter
time ./matrix.icc
```


Offload Report and Results:
```
Threads #: 36 OpenMP threads
Matrix size: 1024
Using multiply kernel: multiply8
[Offload] [MIC 0] [File]                    ../src/multiply.c
[Offload] [MIC 0] [Line]                    321
[Offload] [MIC 0] [Tag]                     Tag 0
[Offload] [HOST]  [Tag 0] [CPU Time]        0.529923(seconds)
[Offload] [MIC 0] [Tag 0] [CPU->MIC Data]   25165860 (bytes)
[Offload] [MIC 0] [Tag 0] [MIC Time]        0.000120(seconds)
[Offload] [MIC 0] [Tag 0] [MIC->CPU Data]   0 (bytes)

[Offload] [MIC 0] [File]                    ../src/multiply.c
[Offload] [MIC 0] [Line]                    323
[Offload] [MIC 0] [Tag]                     Tag 1
[Offload] [HOST]  [Tag 1] [CPU Time]        0.220683(seconds)
[Offload] [MIC 0] [Tag 1] [CPU->MIC Data]   36 (bytes)
[Offload] [MIC 0] [Tag 1] [MIC Time]        0.319010(seconds)
[Offload] [MIC 0] [Tag 1] [MIC->CPU Data]   4 (bytes)

[Offload] [MIC 0] [File]                    ../src/multiply.c
[Offload] [MIC 0] [Line]                    335
[Offload] [MIC 0] [Tag]                     Tag 2
[Offload] [HOST]  [Tag 2] [CPU Time]        0.000878(seconds)
[Offload] [MIC 0] [Tag 2] [CPU->MIC Data]   0 (bytes)
[Offload] [MIC 0] [Tag 2] [MIC Time]        0.000000(seconds)
[Offload] [MIC 0] [Tag 2] [MIC->CPU Data]   8388608 (bytes)

[Offload] [MIC 0] [File]                    ../src/multiply.c
[Offload] [MIC 0] [Line]                    336
[Offload] [MIC 0] [Tag]                     Tag 3
[Offload] [HOST]  [Tag 3] [CPU Time]        0.003795(seconds)
[Offload] [MIC 0] [Tag 3] [CPU->MIC Data]   56 (bytes)
[Offload] [MIC 0] [Tag 3] [MIC Time]        0.000071(seconds)
[Offload] [MIC 0] [Tag 3] [MIC->CPU Data]   8388620 (bytes)

Freq = 2.280953 GHz
Execution time = 1.192 seconds

real	0m11.658s
      user	0m0.400s
sys	0m0.164s
```

# 6. Thread League

First example
```
icc OMP4-teams.c -o OMP4-teams -fopenmp
./OMP4-teams
```

Results:
```
i 0 n 20 idteam 0 idthread 0 teams 2
 i 1 n 20 idteam 0 idthread 0 teams 2
 i 2 n 20 idteam 0 idthread 0 teams 2
 i 3 n 20 idteam 0 idthread 0 teams 2
 i 4 n 20 idteam 0 idthread 1 teams 2
 i 5 n 20 idteam 0 idthread 1 teams 2
 i 6 n 20 idteam 0 idthread 1 teams 2
 i 10 n 20 idteam 1 idthread 0 teams 2
 i 11 n 20 idteam 1 idthread 0 teams 2
 i 12 n 20 idteam 1 idthread 0 teams 2
 i 13 n 20 idteam 1 idthread 0 teams 2
 i 7 n 20 idteam 0 idthread 2 teams 2
 i 8 n 20 idteam 0 idthread 2 teams 2
 i 9 n 20 idteam 0 idthread 2 teams 2
 i 14 n 20 idteam 1 idthread 1 teams 2
 i 15 n 20 idteam 1 idthread 1 teams 2
 i 16 n 20 idteam 1 idthread 1 teams 2
 i 17 n 20 idteam 1 idthread 2 teams 2
 i 18 n 20 idteam 1 idthread 2 teams 2
 i 19 n 20 idteam 1 idthread 2 teams 2
```

Second example
```
cd matrix/linux/
vim ../src/multiply.h
change to multiply6 :  #define MULTIPLY multiply6
make icc
export OMP_NUM_THREADS=36 
export KMP_AFFINITY=scatter
time ./matrix.icc
```

Results:
```
Threads #: 36 OpenMP threads
Matrix size: 10240
Using multiply kernel: multiply6
[Offload] [MIC 0] [File]                    ../src/multiply.c
[Offload] [MIC 0] [Line]                    278
[Offload] [MIC 0] [Tag]                     Tag 0
[Offload] [HOST]  [Tag 0] [CPU Time]        5.529319(seconds)
[Offload] [MIC 0] [Tag 0] [CPU->MIC Data]   2516582436 (bytes)
[Offload] [MIC 0] [Tag 0] [MIC Time]        0.000103(seconds)
[Offload] [MIC 0] [Tag 0] [MIC->CPU Data]   0 (bytes)

[Offload] [MIC 0] [File]                    ../src/multiply.c
[Offload] [MIC 0] [Line]                    280
[Offload] [MIC 0] [Tag]                     Tag 1
[Offload] [HOST]  [Tag 1] [CPU Time]        47.793024(seconds)
[Offload] [MIC 0] [Tag 1] [CPU->MIC Data]   44 (bytes)
[Offload] [MIC 0] [Tag 1] [MIC Time]        69.134064(seconds)
[Offload] [MIC 0] [Tag 1] [MIC->CPU Data]   12 (bytes)

[Offload] [MIC 0] [File]                    ../src/multiply.c
[Offload] [MIC 0] [Line]                    290
[Offload] [MIC 0] [Tag]                     Tag 2
[Offload] [HOST]  [Tag 2] [CPU Time]        0.243337(seconds)
[Offload] [MIC 0] [Tag 2] [CPU->MIC Data]   56 (bytes)
[Offload] [MIC 0] [Tag 2] [MIC Time]        0.000056(seconds)
[Offload] [MIC 0] [Tag 2] [MIC->CPU Data]   2516582412 (bytes)

Freq = 3.591324 GHz
Execution time = 53.695 seconds

real	1m36.542s
user	1m15.442s
sys	0m2.129s
```

# 7. N-Body

Execute N-body paralelized for Host
```
time ./nbody-v0d
```
Offload Report and Results:
```
Before to start:
  Position of first particle is (68.0375,10.3722,92.6457)
  Position of last particle is (-38.4714,-66.0263,50.164)
At the end:
  Position of first particle is (58.4124,18.315,85.2011)
  Position of last particle is (-44.7462,-69.5117,54.9645)
Time was 151.36s

real	2m31.390s
user	181m29.107s
sys	0m1.429s
```
Execute N-body with load balacing between host and devices
```
time ./nbody-v3d
```
Offload Report and Results:
```
Before to start:
  Position of first particle is (68.0375,56.9286,-96.2513)
  Position of last particle is (34.3678,50.164,96.6014)
At the end:
  Position of first particle is (77.4527,62.5269,-104.999)
  Position of last particle is (39.1683,58.7158,86.7716)
Time was 42.2567s

real	0m50.338s
user	45m35.095s
sys	0m41.243s
```
