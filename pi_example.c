#include "stdio.h"
#include "stdlib.h"
#include "omp.h"

#define NUM_THREADS 4


void piClassic();
void piMonteCarlo();


int main() {
	piMonteCarlo();
	return 0;
}


void piClassic() {
	int n = 10000000;
	int i , nthreads;
	double pi, step, sum[NUM_THREADS];
	step = 1.0 / (double) n;
	
	omp_set_num_threads(NUM_THREADS);
	#pragma omp parallel
	{
		int i , my_id, local_nthreads;
		double x;
		local_nthreads = omp_get_num_threads();
		my_id = omp_get_thread_num();
		if (my_id == 0) nthreads = local_nthreads;
		for ( i = my_id, sum[my_id] = 0.0; i < n; i = i + local_nthreads) {
			x = (i + 0.5) * step;
			sum[my_id] += 4.0 / (1.0 + x * x);
		}
	}

	for( i = 0, pi = 0.0; i < nthreads; i++)
		pi += sum[i] * step;

	printf ("PI ~ %.20f\n", pi );
}


void piMonteCarlo() {
	int n = 100000000;
	int i , ncirc = 0;
	
	double pi, x, y, r = 1.0;
	
	srand(12345);
	
	#pragma omp parallel for private (x,y) reduction(+:ncirc )
	for ( i = 0; i < n; i++) {
		x = (double) rand() / RAND_MAX * r * 2.0 - r;
		y = (double) rand() / RAND_MAX * r * 2.0 - r;
		if ((x * x + y * y) <= r * r) ncirc ++;
	}
	
	pi = 4.0 * ((double) ncirc / (double) n);
	printf ("PI ~ %.20f\n", pi );
}