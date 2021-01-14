/**
 * INF560 - TD2
 *
 * Part 2: Work Decomposition
 * Sequential Max
 */
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char**argv) {
  int rank, size;
  int s ;
  int n ;
  int m ;
  int i, j;
  int max ;
  int max_remote;
  int quotient, rest;
  double t1, t2;
  MPI_Status sta;

  /* MPI Initialization */
  MPI_Init(&argc, &argv);

  /* Get the rank of the current task and the number
   * of MPI processe
   */
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  /* Check the input arguments */
  if(argc <4) {
    printf("Usage: %s S N\n", argv[0]);
    printf( "\tS: seed for pseudo-random generator\n" ) ;
    printf( "\tN: size of the array\n" ) ;
    printf( "\tM: number of the arrays\n" ) ;
    exit( 1 ) ;
  }

  s = atoi(argv[1]);
  n = atoi(argv[2]);
  m = atoi(argv[3]);
  srand48(s);

  /* Allocate the array */
  int * tab[m];
  for (i = 0; i < m; i++){
      tab[i] = (int *)malloc(n * sizeof(int));
      if ( tab[i] == NULL ) { 
          fprintf( stderr, "Unable to allocate %d elements\n", n ) ;
          return 1 ; 
      }
  }

  /* Initialize the array */
  for(i=0; i<m; i++) {
      for(j=0; j<n; j++){
          tab[i][j] = lrand48()%n;
      }
  }

  /* start the measurement */
  t1=MPI_Wtime();

  /* search for the max value */
  quotient = n / size;
  for (j=0; j<m; j++){
      if (rank != size - 1){
          max=tab[j][quotient * rank];
          for(i=quotient*rank; i<quotient*(rank+1); i++) {
            if(tab[j][i] > max) {
              max = tab[j][i];
            }
          }
      }
      else{
          max=tab[j][quotient*rank];
          for(i=quotient*rank; i<n; i++){
              if (tab[j][i] > max){
                  max = tab[j][i];
              }
          }
      }

      // Send/receive meassages
      if (rank == 0){
          //printf("Max found in the first part %d\n", max);
          for (i = 1; i < size; i++){
              MPI_Recv(&max_remote, 1, MPI_INTEGER, i, j, MPI_COMM_WORLD, &sta);
              if (max_remote > max){
                  //printf("New max (%d) found by %d\n", max_remote, i);
                  max = max_remote;
              }
          }
      }
      else{
          MPI_Send(&max, 1, MPI_INTEGER, 0, j, MPI_COMM_WORLD);
      }
      /* stop the measurement */
      t2=MPI_Wtime();

#if DEBUG
      printf("the array contains:\n");
      for(i=0; i<n; i++) {
        printf("%d  ", tab[i]);
      }
      printf("\n");
#endif
      if (rank == 0){
          printf("Max value of %dth array = %d\n", j, max);
      }

  }
  if (rank == 0){
    printf("(Seed %d, Size %d) Computation time : %g s\n", s, n, t2-t1);
  }
  MPI_Finalize();
  return 0;
}
