#include <stdio.h>
#include <mpi.h>
#include <unistd.h>
#include <limits.h>

int main(int argc, char **argv){
    int N, me;
    char hostname[HOST_NAME_MAX];
	MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &N);
    MPI_Comm_rank(MPI_COMM_WORLD, &me);
    gethostname(hostname, HOST_NAME_MAX);
    printf("Hello World from task %d out of %d on %s\n", me, N, hostname);
	MPI_Finalize();
	return 0;
}
