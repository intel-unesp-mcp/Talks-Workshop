#include <mpi.h>

int main(int argc, char *argv[])
{
  int rank, size, nn, i, j, contRank, begin, end, rest, amountPerRank ;

  double *a;

  nn=20;
  a = (double*) malloc(nn*sizeof(double));

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Status status;

  if (rank == 0) { 
    for( i=0; i<nn; i++) {
      a[i] = 2;
    }
    for( contRank=1; contRank<size; contRank++) { // rank 0 sends the vectors
      MPI_Send(&a[0], nn, MPI_DOUBLE, contRank, 1 , MPI_COMM_WORLD);
    }
  } else { 
    MPI_Recv(&a[0], nn, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD, &status);
  }

  if (rank != 0) { 
    amountPerRank = (nn / (size-1));
    rest = (nn % (size-1));

    begin=(rank-1)*amountPerRank;
    end=amountPerRank*rank;

    if (rank == (size -1))
      end=end+rest;

    printf("rank %d begin %d end %d rest %d amountPerRank %d \n", rank, begin,end, rest , amountPerRank);

    for( j=begin; j < end ; j++) {
      a[j] = a[j] + cos(a[j]);
    }

    for( j=begin; j < end ; j++) {
        printf("Rank: %d of total: %d a[%d]=%f\n", rank, size, j, a[j]);
    }
  }

  MPI_Barrier(MPI_COMM_WORLD);
  MPI_Finalize();

  free(a);
  return 0;
}