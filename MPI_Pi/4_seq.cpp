#include <iostream>
#include <random>
#include <cmath>
const int MAX_NB_RANDOM_POINT = 1000;
void runMonteCarloPiCalc();

int main(int argc, char* argv[])
{
	runMonteCarloPiCalc();
	return 0;
}

void runMonteCarloPiCalc()
{
	int ncirc = 0;
	double r = 1.0;
	double pi, x, y;
	double accuracy = 0.0001;
	int i = 0;
	double error = M_PI;

	std::random_device	rand_dev;
	std::default_random_engine generator(rand_dev());
	std::uniform_real_distribution<double> distribution(0.0, 1.0);

	std::cout << "Enter desired accuracy (format e=0.0001)" << std::endl;
	std::cin >> accuracy;

	for (i = 1; i < MAX_NB_RANDOM_POINT && error > accuracy; i++)
	{
		x = distribution(generator) * r * 2.0 - r;
		y = distribution(generator) * r * 2.0 - r;
		if ((x * x + y * y) <= r * r)
			ncirc++;
		pi = 4.0 * ((double)ncirc / (double)i);
		error = std::abs(M_PI - pi);
	}
	printf("PI ~ %.20f\n", pi);
}