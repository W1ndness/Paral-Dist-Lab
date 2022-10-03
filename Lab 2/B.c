#include <stdio.h>
#include <omp.h>

static omp_lock_t lock;

int main(int args, char *argv[]) {
	int i;
	omp_init_lock(&lock);
	
	omp_set_num_threads(5);
	#pragma omp parallel for
	for (i = 0; i < 5; i++) {
		omp_set_lock(&lock);
		printf("%d +\n", omp_get_thread_num());
		printf("%d -\n", omp_get_thread_num());
		omp_unset_lock(&lock);
	}
	
	omp_destroy_lock(&lock);
	return 0;
}
