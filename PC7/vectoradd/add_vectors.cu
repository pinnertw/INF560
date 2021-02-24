#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <cuda.h>

#define DEBUG false

__global__ void
vectoraddKernel(int* A, int* B, int* C, int n){
    int i;
    int index;
    int nb_threads;
    index=blockIdx.x * blockDim.x + threadIdx.x;
    nb_threads=blockDim.x * gridDim.x;
    if (index<n){
        for (i=index; i<n; i+=nb_threads){
            C[i] = A[i]+B[i];
        }
    }
}

void vectoradd(int* A, int* B, int* C, int n){
    int total_size = n*sizeof(int);

    /* Define device variables */
    int* dA;
    int* dB;
    int* dC;

    /* Allocation of memory */
    cudaMalloc((void **)&dA, total_size);
    cudaMalloc((void **)&dB, total_size);
    cudaMalloc((void **)&dC, total_size);

    /* Copy array from CPU to device */
    cudaMemcpy(dA, A, total_size, cudaMemcpyHostToDevice);
    cudaMemcpy(dB, B, total_size, cudaMemcpyHostToDevice);

    /* execute the kernel */
    vectoraddKernel<<<1, 100>>>(dA, dB, dC, n);

    /* return the result from device to CPU */
    cudaMemcpy(C, dC, total_size, cudaMemcpyDeviceToHost);

}

int main(int argc, char**argv) {
  int s ;
  int n ;
  int j;
  struct timeval t1, t2;
  double duration;

  /* Check the input arguments */
  if(argc <3) {
    printf("Usage: %s N S\n", argv[0]);
    printf( "\tN: size of the array\n" ) ;
    printf( "\tS: seed for pseudo-random generator\n" ) ;
    exit( 1 ) ;
  }

  n = atoi(argv[1]);
  s = atoi(argv[2]);
  srand48(s);

  /* Allocate the array */
  int * A;
  int * B;
  int * C;
  A = (int *)malloc(n * sizeof(int));
  B = (int *)malloc(n * sizeof(int));
  C = (int *)malloc(n * sizeof(int));
  if ( A == NULL || B == NULL || C == NULL) { 
      fprintf( stderr, "Unable to allocate %d elements\n", n ) ;
      return 1 ; 
  }

  /* Initialize the array */
  for(j=0; j<n; j++){
      A[j] = lrand48()%n;
      B[j] = lrand48()%n;
  }

  /* start the measurement */
  gettimeofday(&t1, NULL);

  /* Add A, B to C */
  vectoradd(A, B, C, n);

  /* end the measurement */
  gettimeofday(&t2, NULL);
  duration = (t2.tv_sec - t1.tv_sec)+((t2.tv_usec-t1.tv_usec)/1e6);

#if DEBUG
  printf("the array 1 contains:\n");
  for(i=0; i<n; i++) {
    printf("%d  ", A[i]);
  }
  printf("\n");
  printf("the array 2 contains:\n");
  for(i=0; i<n; i++) {
    printf("%d  ", B[i]);
  }
  printf("\n");
  /* print result */
  printf("results:\n");
  for(i=0; i<n; i++) {
    printf("%d  ", C[i]);
  }
  printf("\n");
#endif
  printf("(Seed %d, Size %d) Computation time : %g s\n", s, n, duration);
  return 0;
}
