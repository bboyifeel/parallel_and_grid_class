#include <iostream>
#include <cmath>

double seqTrapeziumPiCalc(unsigned int n);
double errorApproximation(unsigned int n);

int main(int argc, char* argv[]) {
	unsigned int n = 0;
	
	std::cout << "Enter number of intervals (n) " << std::endl;
	std::cin >> n;

	double trapeziumPi		= seqTrapeziumPiCalc(n);
	double errorTrapezim	= std::abs(M_PI - trapeziumPi);
	double approximatedError = errorApproximation(n);
	std::cout << "Trapezium Pi = "	<< trapeziumPi << " with n = " << n << " intervals" << std::endl;
	std::cout << "Real Error = "	<< errorTrapezim << std::endl;
	std::cout << "Approximated Error = " << approximatedError << std::endl;

	return 0;
}

double f(double _val) {
	return 4.0 / (1 + _val * _val);
}

double seqTrapeziumPiCalc(unsigned int _n) {
	double result = 0;
	for (int i = 0; i < _n; i++)
		result += f(i / _n) + f((i + 1) / _n);
	result /= 2.0 * _n;
	return result;
}

double errorApproximation(unsigned int _n) {
	return (2.0/(3.0*_n*_n));
}
