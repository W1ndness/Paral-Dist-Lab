#include <stdio.h>
#include <math.h>
#include <time.h>
#include "mpi.h"

#define N 100
#define RAND 10

void mat_gen(int* A, int n) {
	int i = 0, j = 0;
	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			A[i * n + j] = rand() % RAND;
		}
	}
}

void mat_show(int* A, int r, int c) {
    int i = 0, j = 0;
    for (i = 0; i < r; i++) {
        for (j = 0; j < c; j++) {
            printf("%d ", A[i * r + j]);
        }
        printf("\n");
    }
}

void mat_mul(int* A, int* B, int* C, int r, int n) {
    int i = 0, j = 0, k = 0;
    for (i = 0; i < r; i++) {
        for (j = 0; j < n; j++) {
            C[i * n + j] = 0;
            for (k = 0; k < n; k++) {
                C[i * n + j] += A[i * n + k] * B[k + j * n];
            }
        }
    }
}

int main(int argc, char* argv[]) {
    srand(0);
	int myid, num_proc;
	int n = atoi(argv[1]);
	int st = 0;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);
	MPI_Comm_size(MPI_COMM_WORLD, &num_proc);
	MPI_Status status;
	
	if (num_proc == 1) { // not as parallel
        int A[n * n], B[n * n], C[n * n];
        mat_gen(&A, n);
		mat_gen(&B, n);
		printf("Matrix A:\n");
		mat_show(&A, n, n);
		printf("Matrix B:\n");
		mat_show(&B, n, n);
		mat_mul(&A, &B, &C, n, n);
		printf("Matrix C:\n");
		mat_show(&C, n, n);
	}
	else { // as parallel
        int rows = n / (num_proc - 1);
        if (myid == 0) {
            int A[n * n], B[n * n], C[n * n];
            mat_gen(&A, n);
            mat_gen(&B, n);
            printf("Matrix A:\n");
            mat_show(&A, n, n);
            printf("Matrix B:\n");
            mat_show(&B, n, n);
            int i = 0, j = 0;
            for (i = 0; i < num_proc - 1; i++) {
                st = rows * i;
                MPI_Send(&A[st * n], rows * n, MPI_INT, i + 1, 1, MPI_COMM_WORLD);
                MPI_Send(&B[0], n * n, MPI_INT, i + 1, 2, MPI_COMM_WORLD);
            }
            for (i = 0; i < num_proc - 1; i++) {
                st = rows * i;
                MPI_Recv(&C[st * n], rows * n, MPI_INT, i + 1, 3, MPI_COMM_WORLD, &status);
            }
            printf("Matrix C:\n");
            mat_show(&C, n, n);
        }
        else {
            int A[rows * n], B[n * n], C[rows * n];
            MPI_Recv(&A[0], rows * n, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
            MPI_Recv(&B[0], n * n, MPI_INT, 0, 2, MPI_COMM_WORLD, &status);
            mat_mul(&A, &B, &C, rows, n);
            MPI_Send(&C[0], rows * n, MPI_INT, 0, 3, MPI_COMM_WORLD);
        }
	}
	MPI_Finalize();
	return 0;
}
