
Basic MPI Program

```
mpiicc helloWorldMPI.c -o helloWorldMPI
mpirun -host localhost -n 8 ./helloWorldMPI
```

Vector Example

```
icc VectorOpOpenMP.c -o  VectorOpOpenMP -fopenmp
mpiicc VectorOpMPI.c -o VectorOpMPI
./VectorOpOpenMP
mpirun -host localhost -n 5 ./VectorOpMPI
```
