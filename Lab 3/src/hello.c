#include <stdio.h>
#include "mpi.h"

int main(int argc, char* argv[]) {
	int myid, num_proc;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &num_proc);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);
	printf("Hello World! This is %d of %d.\n", myid, num_proc);
	MPI_Finalize();
	return 0;
}
