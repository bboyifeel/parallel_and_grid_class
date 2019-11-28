#include <iostream>
#include <cmath>

double seqSimpsonsPiCalc(double n);

int main(int argc, char* argv[])
{
	int m = 0;
	std::cout << "Enter number of intervals (m) ";
	std::cin >> m;
	double simpsonsPi		= seqSimpsonsPiCalc(m);
	double errorSimpsons	= std::abs(M_PI - simpsonsPi);
	std::cout << "Simpsons Pi = "		<< simpsonsPi		<< " with m = " << m << " intervals" << std::endl;
	std::cout << "Simpsons Error = "	<< errorSimpsons	<< std::endl;
	return 0;
}

double f(double _val)
{
	return 4.0 / (1 + _val * _val);
}

double seqTrapeziumPiCalc(double _n)
{
	double result = 0;
	for (int i = 0; i < _n; i++)
		result += f(i / _n) + f((i + 1) / _n);
	result /= 2.0 * _n;
	return result;
}

double seqSimpsonsPiCalc(double _m)
{
	double result = f(0) + f(1);
	for (int i = 1; i < _m; i++)
		result += 2.0 * f(i / _m) + 4.0 * f((2.0*i - 1) / (2.0 * _m));
	result += 4.0 * f((2.0 * _m - 1) / (2.0 * _m));
	result /= 6.0 * _m;
	return result;
}