#include <omp.h>
#include <stdio.h>

void main() {
    int nb_max_threads = omp_get_max_threads();
    #pragma omp parallel
//#pragma omp num_threads(2)
    {
        printf("Hello from thread %d/%d\n", omp_get_thread_num(), nb_max_threads);
    }
}
