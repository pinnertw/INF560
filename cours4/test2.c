#include <omp.h>
#include <stdio.h>

int main() {
    float a;
    int p;
    a = 91680.
    #pragma omp parallel
    {
        int p = 0;
#ifdef _OPENMP
        p = omp_in_parallel();
#endif
        printf("a: %f ; p: %d\n", a, p) ;
    }
    return 0;
}
