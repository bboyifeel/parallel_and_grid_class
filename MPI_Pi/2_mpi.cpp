#include <mpi.h>
#include <iostream>
#include <cmath>

void mpiSimpsonsPiCalc(int argc, char *argv[]);

int main(int argc, char *argv[]) {
	mpiSimpsonsPiCalc(argc, argv);
	return 0;
}

double f(double _val) {
	return 4.0 / (1 + _val * _val);
}

double intervalSimpsonsPiCalc(int _rank, int _mpiSize, double _m) {
	double result = 0;
	for (int i = _rank + 1; i < _m; i+= _mpiSize)
		result += 2.0 * f(i / _m) + 4.0 * f((2.0*i - 1) / (2.0 * _m));
	return result;
}

void mpiSimpsonsPiCalc(int argc, char *argv[]) {
	int32_t size	= 0;
	int32_t rank	= 0;
	int32_t m		= 0;
	double	pi		= 0;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);	// Get #processes
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);	// Get current rank

	if (rank == 0)
	{
		std::cout << "Enter number of intervals (m)" << std::endl;
		std::cin >> m;
	}

	MPI_Barrier(MPI_COMM_WORLD);
	double startTime = MPI_Wtime();
	MPI_Bcast(&m, /*count*/1, MPI_INT, /*root*/0, MPI_COMM_WORLD);
	double intervalResult = intervalSimpsonsPiCalc(rank, size, m);
	MPI_Reduce(&intervalResult, &pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
	double endTime = MPI_Wtime();

	if (rank == 0)
	{
		pi += intervalResult;
		pi += f(0) + f(1);
		pi += 4.0 * f((2.0 * m - 1) / (2.0 * m));
		pi /= 6.0 * m;
		double error = std::abs(M_PI - pi);

		std::cout << "Simpson's Pi = "		<< pi		<< " with m = " << m << " intervals" << std::endl;
		std::cout << "Simpson's Error = "	<< error	<< std::endl;
		std::cout << "WTime difference = "	<< endTime - startTime << std::endl;
	}

	MPI_Finalize();
}