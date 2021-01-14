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
  int *max_arrays;
  int nb_stop_node, nb_message, nb_max_received;
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
      for(i=0; i<m; i++) {
          for(j=0; j<n; j++){
              tab[i][j] = lrand48()%n;
          }
      }

      /* start the measurement */
      t1=MPI_Wtime();

      /* search for the max value from others*/
      max_arrays = malloc(m * sizeof(int));
      nb_stop_node = 0;
      nb_message = 0;
      nb_max_received = 0;

      if(size > 1){
          while(nb_stop_node < size - 1 || nb_max_received < m){
              // Receive whether availability or the maximum of an array.
              MPI_Recv(&max, 1, MPI_INTEGER, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &sta);
              //printf("Receive the max %d from the machine %d, nb_array sent = %d\n", max, sta.MPI_SOURCE, nb_message);
              // max = -1 : the machine is available
              if (max != -1){
                  max_arrays[sta.MPI_TAG] = max;
                  nb_max_received++;
              }
              if (nb_message < m){
                  //printf("Sending message from 0 to %d\n", sta.MPI_SOURCE);
                  max = -1;
                  MPI_Send(&max, 1, MPI_INTEGER, sta.MPI_SOURCE, nb_message, MPI_COMM_WORLD);
                  //printf("Sending matrix %d/%d from 0 to %d\n", nb_message, m, sta.MPI_SOURCE);
                  MPI_Send(tab[nb_message], n, MPI_INTEGER, sta.MPI_SOURCE, nb_message, MPI_COMM_WORLD);
                  nb_message++;
              }
              else{
                  max = -2;
                  //printf("Sending stopping signal to the machine\n");
                  MPI_Send(&max, 1, MPI_INTEGER, sta.MPI_SOURCE, m+1, MPI_COMM_WORLD);
                  nb_stop_node++;
              }
          }
      }
      /* if size = 1: do all the stuff with the machine rank 0. */
      if (size == 1){
          for(i=0; i<m; i++){
              max = tab[i][0];
              for (j=0; j<n; j++){
                  if (tab[i][j] > max){
                      max = tab[i][j];
                  }
              }
          }
      }
      t2=MPI_Wtime();
      printf("(Seed %d, Size %d) Computation time : %g s\n", s, n, t2 - t1);
      for (i=0; i<m; i++){
          printf("Max value of %dth array = %d \n", i, max_arrays[i]);
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
    max = -1;
    MPI_Send(&max, 1, MPI_INTEGER, 0, n+1, MPI_COMM_WORLD);
    while (max == -1){
        //printf("(%d) Waiting the message indicating whether if there is a matrix to proceed\n", rank);
        MPI_Recv(&max, 1, MPI_INTEGER, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &sta);
        //printf("(%d) Received : %d\n", rank, max);
        if (max == -1){
            //printf("Receiving the matrix with machine (%d)\n", rank);
            MPI_Recv(tab, n, MPI_INTEGER, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &sta);
            max = tab[0];
            for(i=0; i<n; i++){
                if(tab[i] > max) {
                  max = tab[i];
                }
            }
            // We send the maximum
            //printf("(%d) Sending the maximum (%d) to machine 0\n", rank, max);
            MPI_Send(&max, 1, MPI_INTEGER, 0, sta.MPI_TAG, MPI_COMM_WORLD);
            max = -1;
        }
        else{
            //printf("(%d) End of the work.\n", rank);
            break;
        }
    }
  }
  MPI_Finalize();
  return 0;
}
