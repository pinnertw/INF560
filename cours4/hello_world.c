#include <omp.h>
#include <stdio.h>

void main() {
    #pragma omp parallel
//#pragma omp num_threads(2)
    {
        printf("Hello from thread %d\n", omp_get_thread_num());
    }
}
