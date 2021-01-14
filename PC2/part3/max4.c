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

  /* start the measurement */
  t1=MPI_Wtime();

  if (rank == 0){
      /* Allocate the arrays */
      int * tab[m];
      for (i = 0; i < m; i++){
          tab[i] = (int *)malloc(n * sizeof(int));
          if ( tab[i] == NULL ) { 
              fprintf( stderr, "Unable to allocate %d elements\n", n ) ;
              return 1 ; 
          }
      }

      /* Initialize the arrays */
      //printf("Initializing arrays\n");
      for(i=0; i<m; i++) {
          for(j=0; j<n; j++){
              tab[i][j] = lrand48()%n;
          }
      }
      //printf("Finished initializing arrays\n");

      /* search for the max value from others*/
      for(i=0; i<m; i++){
          //printf("Processing the %dth array\n", i);
          /* if size != 1 then send arrays */
          if (size != 1){
              //printf("Sending message to the machine %d\n", i%(size-1) + 1);
              MPI_Send(tab[i], n, MPI_INTEGER, i%(size-1) + 1, i, MPI_COMM_WORLD);
          }
      }
      t2=MPI_Wtime();
      //printf("Finished sending message\n");
      printf("(Seed %d, Size %d) Computation time : %g s\n", s, n, t2-t1);
      for(i=0; i<m; i++){
          /* if size = 1 */
          if (size == 1){
              max = tab[i][0];
              for (j=0; j<n; j++){
                  if (tab[i][j] > max){
                      max = tab[i][j];
                  }
              }
          }
          else{
              MPI_Recv(&max, 1, MPI_INTEGER, MPI_ANY_SOURCE, i, MPI_COMM_WORLD, &sta);
          }
          /* stop the measurement */
          printf("Max value of %dth array = %d\n", i, max);
      }


#if DEBUG
      printf("the array contains:\n");
      for(i=0; i<n; i++) {
        printf("%d  ", tab[i]);
      }
      printf("\n");
#endif
  }
  else{
    int * tab;
    tab = malloc(n * sizeof(int));
    j=0;
    if (m%(size-1) >= rank){
        j=-1;
    }
    for(; j<m/(size-1); j++){
        //printf("Waiting %dth times for the signal...\n", j+1);
        MPI_Recv(tab, n, MPI_INTEGER, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &sta);
        max = tab[0];
        for(i=0; i<n; i++){
            if(tab[i] > max) {
              max = tab[i];
            }
        }
        //printf("Sending signal back to 0 with tag = %d...\n", sta.MPI_TAG);
        MPI_Send(&max, 1, MPI_INTEGER, 0, sta.MPI_TAG, MPI_COMM_WORLD);
    }
  }
  MPI_Finalize();
  return 0;
}
