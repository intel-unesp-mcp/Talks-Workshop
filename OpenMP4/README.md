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

```
export OMP_NUM_THREADS=36 
export KMP_AFFINITY=scatter
time ./matrix.icc
```

Vtune Code

# 3. Offload

# 4. Transfer block

# 5. Update Data

# 6. Thread League

