#include <stdio.h>
#include <omp.h>

#define N 100
#define RAND 1000

int m, n;
float a[N][N], b[N][N], c[N][N];

int main(int argc, char *argv[]) {
	printf("Input width and height of the matrix: ");
	scanf("%d%d", &m, &n);
	int i, j;
	srand(0);
	for (i = 0; i < m; i++) {
		for (j = 0; j < n; j++) {
			a[i][j] = (float)rand() / RAND;
			b[i][j] = (float)rand() / RAND;
		}
	}
	printf("Matrix A:\n");
	for (i = 0; i < m; i++) {
		for (j = 0; j < n; j++) {
			printf("%f ", a[i][j]);
		}
		printf("\n");
	}
	printf("Matrix B:\n");
	for (i = 0; i < m; i++) {
		for (j = 0; j < n; j++) {
			printf("%f ", b[i][j]);
		}
		printf("\n");
	}
	omp_set_num_threads(atoi(argv[1]));
	#pragma omp parallel shared(a, b, c) private(i, j)
	{
		#pragma omp for schedule(dynamic)
		for (i = 0; i < m; i++) {
			for (j = 0; j < n; j++) {
				c[i][j] = a[i][j] + b[i][j];
			}
		}
	}
	printf("A + B = \n");
	for (i = 0; i < m; i++) {
		for (j = 0; j < n; j++) {
			printf("%f ", c[i][j]);
		}
		printf("\n");
	}
	return 0;
}
