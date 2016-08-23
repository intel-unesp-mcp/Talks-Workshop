
Basic MPI Program

```
mpiicc helloWorldMPI.c -o helloWorldMPI
mpirun -n 8 ./helloWorldMPI
```

Vector Example

```
icc VectorOpOpenMP.c -o  VectorOpOpenMP -fopenmp
mpiicc VectorOpMPI.c -o VectorOpMPI
./VectorOpOpenMP
mpirun -n 5 ./VectorOpMPI
```
