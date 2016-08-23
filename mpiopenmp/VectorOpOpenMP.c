int main(int argc, char *argv[])
{
  int nn, i, j;

  double *a;

  nn=16;//9000;

  a = (double*) malloc(nn*sizeof(double));

  for( i=0; i<nn; i++) 
    a[i] = 2;

  #pragma omp for
  for( j=0; j<nn; j++) 
    a[j] = a[j] + cos(a[j]);

  for( j=0; j<nn; j++) 
    printf("a[%d]=%f\n", j, a[j]);

  free(a);
  return 0;
}