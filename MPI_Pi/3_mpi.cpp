#include <mpi.h>
#include <iostream>
#include <random>
#include <cmath>
#include <fstream>
#include <cstdio>
#include <vector>

const uint32_t masterNode = 0;
const int tagXDotsPack = 1;
const int tagYDotsPack = 2;
uint64_t packSize = 100000;
const uint64_t MAX_NB_RANDOM_POINT = 100000000;

const double r = 1.0;


void runMonteCarloPiCalc(int argc, char *argv[]);

int main(int argc, char *argv[]) {
	runMonteCarloPiCalc(argc, argv);
	return 0;
}

void managerCode(uint32_t nodesSize
	, std::default_random_engine generator
	, std::uniform_real_distribution<double> distribution) {

	for (int node = 1; node < nodesSize; node++) {
		
		// std::vector<double> xDotsPack = generateVector(packSize, generator, distribution);
		// std::vector<double> yDotsPack = generateVector(packSize, generator, distribution);
		double xDotsPack[packSize];
		double yDotsPack[packSize];

		for(int i = 0; i < packSize; i++) { 
			xDotsPack[i] = distribution(generator);
			yDotsPack[i] = distribution(generator);
		}

		MPI_Send(xDotsPack
				,packSize
				,MPI_DOUBLE
				, /*dest*/ node
				,tagXDotsPack
				,MPI_COMM_WORLD);
		MPI_Send(yDotsPack
				,packSize
				,MPI_DOUBLE
				, /*dest*/ node
				,tagYDotsPack
				,MPI_COMM_WORLD);
	}
}


uint32_t workerCode() {
	// std::vector<double> xDotsPack(packSize);
	// std::vector<double> yDotsPack(packSize);
	double xDotsPack[packSize];
	double yDotsPack[packSize];
	double x, y;
	uint32_t localResult = 0;
	
	MPI_Status status;

	MPI_Recv(xDotsPack
			, packSize
			, MPI_DOUBLE
			, masterNode
			, tagXDotsPack
			, MPI_COMM_WORLD
			, &status);

	MPI_Recv(yDotsPack
			, packSize
			, MPI_DOUBLE
			, masterNode
			, tagYDotsPack
			, MPI_COMM_WORLD
			, &status);

	for (int i = 0; i < packSize; i++) {
		x = xDotsPack[i];
		y = yDotsPack[i];
		
		// r = 1 --> r*r = 1
		if ((x * x + y * y) <= r * r)
			localResult++;
	}

	return localResult;
}


void runMonteCarloPiCalc(int argc, char *argv[]) {
	int32_t nodesSize = 0;
	int32_t rank = 0;

	int localCount = 0;
	double epsilon = 0.0001;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &nodesSize);	// Get #processes
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);	// Get current rank

	std::random_device rand_dev;
	std::default_random_engine generator(rand_dev());
	std::uniform_real_distribution<double> distribution(-1.0, 1.0);

	if (rank == masterNode) {
		std::cout << nodesSize << std::endl;
		do {
			//std::cout << "Enter desired accuracy (format e=0.0001)" << std::endl;
			//std::cin >> epsilon;
		} while (epsilon >= 1 || epsilon <= 0);
		std::cout << "Computing..." << std::endl;
	}

	MPI_Barrier(MPI_COMM_WORLD);
	double startTime = MPI_Wtime();
	
	//////
	uint32_t localIterResult = 0;
	uint32_t totalInCircle = 0;

	int stop = 0;
	uint32_t iter = 0;
	do {
		iter++;
		if (rank == masterNode) { // server 
			managerCode(nodesSize, generator, distribution);
		}
		else {
			localIterResult = workerCode();
		}

		uint64_t globalIterResult = 0;
		MPI_Reduce(&localIterResult, &globalIterResult, 1, MPI_INT, MPI_SUM, masterNode, MPI_COMM_WORLD);
		double pi = 0;

		if (rank == masterNode) {
			totalInCircle += globalIterResult;
			uint64_t total = iter * packSize * (nodesSize - 1);
			pi = 4.0 * ((double) totalInCircle / (double) total);
			
			double error = std::abs(M_PI - pi);
			
			if (error < epsilon || total > MAX_NB_RANDOM_POINT) { // tell to stop
				stop = 1;
				std::cout << "Pi " << pi << std::endl;
				std::cout << "Iteration number " << iter << std::endl;
			}
		}
		MPI_Bcast(&stop, /*count*/1, MPI_INT, masterNode, MPI_COMM_WORLD);
	} while(!stop);

	double endTime = MPI_Wtime();
	if (rank==masterNode) {
		std::cout << "Time " << endTime - startTime << std::endl;
	}
	//////
	MPI_Finalize();
}

// 2 1.57538e+09
// 6 - 1.57538e+09