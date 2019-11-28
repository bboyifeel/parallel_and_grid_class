#include <mpi.h>
#include <iostream>
#include <cmath>

#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>
#include <boost/mpi.hpp>
namespace mpi = boost::mpi;

void mpiSimpsonsPiCalc(int argc, char *argv[]);

int main(int argc, char *argv[]) {
	mpiSimpsonsPiCalc(argc, argv);
	return 0;
}

double f(double _val) {
	return 4.0 / (1 + _val * _val);
}

double intervalSimpsonsPiCalc(int _rank, int _mpiSize, uint32_t _n) {
	double m = static_cast<double>(_n/2);
	double result = 0;
	for (int i = _rank + 1; i < m; i+= _mpiSize)
		result += 2.0 * f(i / m) + 4.0 * f((2.0*i - 1) / (2.0 * m));
	return result;
}

double errorApproximation(unsigned int _n) {
	return 8.0/(15.0*_n*_n*_n*_n);
}

void mpiSimpsonsPiCalc(int argc, char *argv[]) {
	int32_t size	= 0;
	int32_t rank	= 0;
	uint32_t n		= 0;
	double	pi		= 0;

	mpi::environment env(argc, argv);
	mpi::communicator world;
	
	size = world.size();
	rank = world.rank();

	if (rank == 0)
	{
		do {
			std::cout << "Enter number of intervals (n%2==0) ";
			std::cin >> n;
		} while (n%2 != 0);
	}

	world.barrier();
	mpi::timer appTimer;
	mpi::broadcast(world, n, 0);
	double intervalResult = intervalSimpsonsPiCalc(rank, size, n);
	reduce(world, &intervalResult, 1, &pi, std::plus<double>(), 0);
	double elapsedTime = appTimer.elapsed();

	if (rank == 0)
	{
		double m = static_cast<double>(n/2);
		pi += f(0) + f(1);
		pi += 4.0 * f((2.0 * m - 1) / (2.0 * m));
		pi /= 6.0 * m;

		double realError = std::abs(M_PI - pi);
		double approximatedError = errorApproximation(n);
		std::cout << "Simpson's Pi = " << pi
				<< " with n = " << n
				<< " intervals" << std::endl;
		std::cout << "Simpson's Real Error = " << realError << std::endl;
		std::cout << "Approximated Error = " << approximatedError << std::endl;
		std::cout << "WTime difference = " << elapsedTime << std::endl;
	}
}