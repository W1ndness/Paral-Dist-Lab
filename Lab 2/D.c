#include <stdio.h>
#include <omp.h>

#define N 100
#define RAND 100

int n, m, k;
float a[N][N], b[N][N], c[N][N];

int main(int argc, char *argv[]) {
	printf("Input width and height of the matrix A: ");
	scanf("%d%d", &n, &m);
	printf("Input height of the matrix B: ");
	scanf("%d", &k);
	int i, j, t;
	srand(0);
	for (i = 0; i < n; i++) {
		for (j = 0; j < m; j++) {
			a[i][j] = rand() % RAND;
		}
	}
	for (j = 0; j < m; j++) {
		for (t = 0; t < k; t++) {
			b[j][t] = rand() % RAND;
		}
	}
	printf("Matrix A:\n");
	for (i = 0; i < n; i++) {
		for (j = 0; j < m; j++) {
			printf("%f ", a[i][j]);
		}
		printf("\n");
	}
	printf("Matrix B:\n");
	for (i = 0; i < m; i++) {
		for (j = 0; j < k; j++) {
			printf("%f ", b[i][j]);
		}
		printf("\n");
	}
	omp_set_num_threads(atoi(argv[1]));
	#pragma omp parallel shared(a, b, c) private(i, j, t)
	{
		#pragma omp for schedule(dynamic)
		for (i = 0; i < n; i++) {
			for (j = 0; j < k; j++) {
				for (t = 0; t < m; t++) {
					c[i][j] += a[i][t] * b[t][j];
				}
			}
		}
	}
	printf("A * B = \n");
	for (i = 0; i < n; i++) {
		for (j = 0; j < k; j++) {
			printf("%f ", c[i][j]);
		}
		printf("\n");
	}
	return 0;
}
