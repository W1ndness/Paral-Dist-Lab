#include <stdio.h>
#include <omp.h>

int main(int argc, char *argv[]) {
	int nthreads, tid;
	char buf[32];
	omp_set_num_threads(4);
	#pragma omp parallel private(tid)
	{
		tid = omp_get_thread_num();
		printf("Hello OpenMP!! --  from thread %d\n", tid);
		if (tid == 0) {
			nthreads = omp_get_num_threads();
		}
	}	
	printf("Number of threads %d.\n", nthreads);
 	return 0;
}
