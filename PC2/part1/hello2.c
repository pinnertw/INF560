#include <stdio.h>
#include <mpi.h>
#include <stdbool.h>

int main(int argc, char **argv){
    int N, me, i;
    bool s;
    MPI_Status sta;
	MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &N);
    MPI_Comm_rank(MPI_COMM_WORLD, &me);
    if (me == 0){
        for(i = 1; i < N; i++){
            MPI_Recv(&s, 1, MPI_BYTE, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &sta);
        }
        printf("Hello World with %d ready task(s)\n", N);
    }
    else{
        MPI_Send(&s, 1, MPI_BYTE, 0, 0, MPI_COMM_WORLD);
    }
	MPI_Finalize();
	return 0;
}
