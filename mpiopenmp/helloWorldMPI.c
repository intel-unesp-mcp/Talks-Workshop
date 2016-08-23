#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
  int rank, size; 

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  char hostname[1024];
  gethostname(hostname, 1024);

  printf("Rank: %d of total: %d Hostname: %s\n", rank, size, hostname);

  MPI_Finalize();

  return 0;
}

