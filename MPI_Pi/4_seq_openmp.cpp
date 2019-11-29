// compile:
// clang++ -std=c++1y -Xpreprocessor -fopenmp -lomp 4_seq_openmp.cpp 

#include <iostream>
#include <random>
#include <cmath>
#include <fstream>
#include <cstdio>
#include <chrono>
#include "stdlib.h"
#include "omp.h"

#define NUM_THREADS 4

const uint64_t MAX_NB_RANDOM_POINT = 1000000000;

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
	int ncirc = 0;
	double r = 1.0;
	double pi, x, y;
	unsigned int iterSize = 10000;
	double epsilon = 0.00001;

	do {
		std::cout << "Enter desired accuracy (format e=0.0001)" << std::endl;
		std::cin >> epsilon;
	} while (epsilon >= 1 || epsilon <= 0);

	std::random_device	rand_dev;
	std::default_random_engine generator(rand_dev());
	std::uniform_real_distribution<double> distribution(0.0, 1.0);

  	int iter = 0;
  	bool done = false;
  	double error = 1;

  	Timer timer;
  	while (error > epsilon || (iterSize * iter) > MAX_NB_RANDOM_POINT) {
  		iter++;

  		#pragma omp parallel for private(x,y) reduction(+:ncirc) 
		for (int i = 0; i < iterSize; i++)
		{
			x = distribution(generator) * r * 2.0 - r;
			y = distribution(generator) * r * 2.0 - r;
			if ((x * x + y * y) <= r * r)
				ncirc++;
		}
		pi = 4.0 * ((double) ncirc / ((double) iterSize * iter));
		error = std::abs(M_PI - pi);
	}

	double sec = timer.elapsed();
	std::cout << "Number of iteration " << iter << std::endl;
	std::cout << "epsilon " << epsilon << std::endl; 
	printf("PI ~ %.20f\n", pi);
	printf("Time %f\n", sec);
	return 0;
}