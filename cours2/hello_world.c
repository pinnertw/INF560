#include <stdio.h>
#include <mpi.h>

int main(int argc, char **argv){
    int N;
	MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &N);
    printf("Number of processes = %d\n", N);
	MPI_Finalize();
	return 0;
}
