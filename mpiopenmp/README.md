
Basic MPI Program (Host)

```
mpiicc helloWorldMPI.c -o helloWorldMPI
mpirun -host localhost -n 8 ./helloWorldMPI
```

Basic MPI Program (Device)

```
mpiicc helloWorldMPI.c -o helloWorldMPI.mic -mmic
mpirun -host mic0 -n 8 ./helloWorldMPI.mic
```
Vector Example

```
icc VectorOpOpenMP.c -o  VectorOpOpenMP -fopenmp
mpiicc VectorOpMPI.c -o VectorOpMPI
./VectorOpOpenMP
mpirun -host localhost -n 5 ./VectorOpMPI
```
