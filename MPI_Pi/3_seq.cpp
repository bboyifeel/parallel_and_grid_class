#include <iostream>
#include <random>
#include <cmath>

void runMonteCarloPiCalc();

int main(int argc, char* argv[]) {
	runMonteCarloPiCalc();
	return 0;
}

void runMonteCarloPiCalc() {
	int ncirc = 0;
	double r = 1.0;
	double pi, x, y;
	int i = 0;
	unsigned int MAX_NB_RANDOM_POINT = 1000;

	std::random_device	rand_dev;
	std::default_random_engine generator(rand_dev());
	std::uniform_real_distribution<double> distribution(0.0, 1.0);

	std::cout << "Enter desired number of dots to generate" << std::endl;
	std::cin >> MAX_NB_RANDOM_POINT;

	for (i = 0; i < MAX_NB_RANDOM_POINT; i++)
	{
		x = distribution(generator) * r * 2.0 - r;
		y = distribution(generator) * r * 2.0 - r;
		if ((x * x + y * y) <= r * r)
			ncirc++;
	}

	pi = 4.0 * ((double) ncirc / (double)i);
	printf("PI ~ %.20f\n", pi);
}