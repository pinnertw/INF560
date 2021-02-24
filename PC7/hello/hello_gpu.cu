#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <cuda_runtime.h>

__global__ void
compute_stringKernel(char* res, char* a, char* b, char* c, int length){
/* Add on device with kernel execution */
    int i;
    i = blockIdx.x * blockDim.x + threadIdx.x;
    if (i<length){
        res[i] = a[i] + b[i] + c[i] ; 
    }
}

/* Function computing the final string to print */
void compute_string( char * res, char * a, char * b, char *c, int length ) 
{
    int total_size=length*sizeof(char);

    /* Define device variables */
    char * d_res ;
    char * d_a ;
    char * d_b ;
    char * d_c ;

    /* Allocation of memory */
    cudaMalloc((void **)&d_res, total_size);
    cudaMalloc((void **)&d_a, total_size);
    cudaMalloc((void **)&d_b, total_size);
    cudaMalloc((void **)&d_c, total_size);

    /* Copy array from CPU to device */
    cudaMemcpy(d_res, res, total_size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_a, a, total_size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_b, b, total_size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_c, c, total_size, cudaMemcpyHostToDevice);

    /* execute the kernel */
    compute_stringKernel<<<1, 30>>>(d_res, d_a, d_b, d_c, length);

    /* return the result from device to CPU */
    cudaMemcpy(res, d_res, total_size, cudaMemcpyDeviceToHost);
}

int main()
{

char * res ;

char a[30] = { 40, 70, 70, 70, 80, 0, 50, 80, 80, 70, 70, 0, 40, 80, 79, 70, 0, 40, 50, 50, 0, 70, 80, 0, 30, 50, 30, 30, 0, 0 } ;
char b[30] = { 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 
10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 0, 0 } ;
char c[30] = { 22, 21, 28, 28, 21, 22, 27, 21, 24, 28, 20, 22, 20, 24, 22, 
29, 22, 21, 20, 25, 22, 25, 20, 22, 27, 25, 28, 25, 0, 0 } ;

res = (char *)malloc( 30 * sizeof( char ) ) ;


/* This function call should be programmed in CUDA */
/* -> need to allocate and transfer data to/from the device */
compute_string( res, a, b, c, 30 ) ;

printf( "%s\n", res ) ;

return 0 ;
}
