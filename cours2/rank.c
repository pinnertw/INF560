#include <stdio.h>
#include <mpi.h>

int main(int argc, char **argv){
    int N, me;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &N);
    MPI_Comm_rank(MPI_COMM_WORLD, &me);
    printf("My rank is %d out of %d\n", me, N);
    MPI_Finalize();
    return 0;
}
