#include <iostream>
#include <cmath>

double seqSimpsonsPiCalc(unsigned int n);
double errorApproximation(unsigned int n) ;

int main(int argc, char* argv[]) {
	unsigned int n = 0;
	do {
		std::cout << "Enter number of intervals (n%2==0) ";
		std::cin >> n;
	} while (n%2 != 0);

	double simpsonsPi = seqSimpsonsPiCalc(n);
	double errorSimpsons = std::abs(M_PI - simpsonsPi);
	double approximatedError = errorApproximation(n);
	std::cout << "Simpsons Pi = " << simpsonsPi << " with n = " << n << " intervals" << std::endl;
	std::cout << "Simpsons Error = " << errorSimpsons << std::endl;
	std::cout << "Approximated Error = " << approximatedError << std::endl;
	return 0;
}

double f(double _val) {
	return 4.0 / (1 + _val * _val);
}

double errorApproximation(unsigned int _n) {
	return 8.0/(15.0*_n*_n*_n*_n);
}

double seqSimpsonsPiCalc(unsigned int _m) {
	double m = static_cast<double>(_m/2);
	double result = f(0) + f(1);
	for (int i = 1; i < m; i++)
		result += 2.0 * f(i / m) + 4.0 * f((2.0*i - 1) / (2.0 * m));
	result += 4.0 * f((2.0 * m - 1) / (2.0 * m));
	result /= 6.0 * m;
	return result;
}