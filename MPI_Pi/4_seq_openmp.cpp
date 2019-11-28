#include "stdio.h"
#include "stdlib.h"
#include "omp.h"
#include <chrono>

#define NUM_THREADS 4

class Timer {
public:
	Timer() : beg_(clock_::now()) {}
	void reset() { beg_ = clock_::now(); }
	double elapsed() const {
		return std::chrono::duration_cast<second_>
			(clock_::now() - beg_).count();
	}

private:
	typedef std::chrono::high_resolution_clock clock_;
	typedef std::chrono::duration<double, std::ratio<1> > second_;
	std::chrono::time_point<clock_> beg_;
};


int main() {
	int n = 100000000;
	int ncirc = 0;
	double r = 1.0;
	double pi, x, y;
	
	srand(132345);
	Timer timer;
	#pragma omp parallel for private(x,y) reduction(+:ncirc) 
	for (int i = 0; i < n; i++)
	{
		x = (double)rand() / RAND_MAX * r * 2.0 - r;
		y = (double)rand() / RAND_MAX * r * 2.0 - r;
		if ((x * x + y * y) <= r * r)
			ncirc++;
	}
	double sec = timer.elapsed();
	pi = 4.0 * ((double)ncirc / (double)n);

	printf("PI ~ %.20f\n", pi);
	printf("Time %f\n", sec);
	return 0;
}