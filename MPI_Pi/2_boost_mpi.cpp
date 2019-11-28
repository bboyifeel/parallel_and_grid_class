#include <mpi.h>
#include <iostream>
#include <cmath>

#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>
#include <boost/mpi.hpp>
namespace mpi = boost::mpi;

void mpiSimpsonsPiCalc(int argc, char *argv[]);

int main(int argc, char *argv[])
{
	mpiSimpsonsPiCalc(argc, argv);
	return 0;
}

double f(double _val)
{
	return 4.0 / (1 + _val * _val);
}

double intervalSimpsonsPiCalc(int _rank, int _mpiSize, double _m)
{
	double result = 0;
	for (int i = _rank + 1; i < _m; i+= _mpiSize)
		result += 2.0 * f(i / _m) + 4.0 * f((2.0*i - 1) / (2.0 * _m));
	return result;
}

void mpiSimpsonsPiCalc(int argc, char *argv[])
{
	int32_t size	= 0;
	int32_t rank	= 0;
	int32_t m		= 0;
	double	pi		= 0;

	mpi::environment env(argc, argv);
	mpi::communicator world;
	
	size = world.size();
	rank = world.rank();

	if (rank == 0)
	{
		std::cout << "Enter number of intervals (m)" << std::endl;
		std::cin >> m;
	}

	world.barrier();
	mpi::timer appTimer;
	mpi::broadcast(world, m, 0);
	double intervalResult = intervalSimpsonsPiCalc(rank, size, m);
	reduce(world, &intervalResult, 1, &pi, std::plus<double>(), 0);
	double elapsedTime = appTimer.elapsed();

	if (rank == 0)
	{
		pi += f(0) + f(1);
		pi += 4.0 * f((2.0 * m - 1) / (2.0 * m));
		pi /= 6.0 * m;
		double error = std::abs(M_PI - pi);

		std::cout << "Simpson's Pi = "		<< pi		<< " with m = " << m << " intervals" << std::endl;
		std::cout << "Simpson's Error = "	<< error	<< std::endl;
		std::cout << "WTime difference = "	<< elapsedTime << std::endl;
	}
}