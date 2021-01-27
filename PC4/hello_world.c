#include <omp.h>
#include <stdio.h>

void main() {
    #pragma omp parallel
//#pragma omp num_threads(2)
    {
        printf("Hello from thread %d/%d\n", omp_get_thread_num(), omp_get_num_threads());
    }
}
