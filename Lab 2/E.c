#include <stdio.h>
#include <omp.h>
#include <time.h>

#define N 4

int a[N][N], b[N][N];
int c[N][N];
int p[8][N / 2][N / 2];

int main(int argc, char* argv[]) {
	srand(0);
	int i = 0, j = 0, k = 0, t = 0;
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			a[i][j] = rand() % 20;
			b[i][j] = rand() % 20;
		}
	}
	printf("Matrix A: \n");
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			printf("%d ", a[i][j]);
		}
		printf("\n");
	}
	printf("Matrix B: \n");
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			printf("%d ", b[i][j]);
		}
		printf("\n");
	}
	omp_set_num_threads(atoi(argv[1]));
	printf("\n=============================================\n");
	printf("Starting method 1...");
	int start = 0, end = 0;
	start = clock();
	#pragma omp parallel shared(a, b, c) private(i, j, k)
	{
		#pragma omp for schedule(dynamic)
		for (i = 0; i < N; i++) {
			for (j = 0; j < N; j++) {
				for (k = 0; k < N; k++) {
					c[i][j] += a[i][k] * b[k][j];
				}
			}
		}
	}
	end = clock();
	printf("\n=============================================\n");
	printf("Matrix C: \n");
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			printf("%d ", c[i][j]);
		}
		printf("\n");
	}
	printf("Method 1, consuming %dms\n", end - start);
	printf("\n=============================================\n");

	printf("\n=============================================\n");
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			c[i][j] = 0;
		}
	}
	printf("Starting method 2...\n");
	start = clock();
	#pragma omp parallel shared(a, b, c) private(i, j, k)
	{
		#pragma omp for schedule(dynamic)
		for (i = 0; i < N; i++) {
			for (k = 0; k < N; k++) {
				for (j = 0; j < N; j++) {
					c[i][j] += a[i][k] * b[k][j];
				}
			}
		}
	}
	end = clock();
	
	printf("=============================================\n");
	printf("Matrix C: \n");
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			printf("%d ", c[i][j]);
		}
		printf("\n");
	}
	printf("Method 2, consuming %dms\n", end - start);
	printf("\n=============================================\n");
	return 0;
}