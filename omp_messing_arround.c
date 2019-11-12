#include "stdio.h"
#include "omp.h"

void main(){
	#pragma omp parallel
	{
		int my_id = omp_get_thread_num();
		int num_ids = omp_get_num_threads();
		printf ("[Thread %d] There are %d threads.\n", my_id, num_ids);
	}
}