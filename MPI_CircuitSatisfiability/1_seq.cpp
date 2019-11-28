#include <iostream>
#include <bitset>
#include <vector>
#include <cstdint>

const int32_t kinput_size = 65536;

bool extract_bit(int32_t _n, int32_t _i)
{
	return (_n & (1 << _i) ? 1 : 0);
}

bool check_circuit(int32_t input)
{
	bool v[16];
	for (int i = 0; i < 16; i++)
		v[i] = extract_bit(input, i);
	return ((v[0] || v[1]) && (!v[1] || !v[3]) && (v[2] || v[3])
		&& (!v[3] || !v[4]) && (v[4] || !v[5]) && (v[5] || !v[6])
		&& (v[5] || v[6]) && (v[6] || !v[15]) && (v[7] || !v[8])
		&& (!v[7] || !v[13]) && (v[8] || v[9]) && (v[8] || !v[9])
		&& (!v[9] || !v[10]) && (v[9] || v[11]) && (v[10] || v[11])
		&& (v[12] || v[13]) && (v[13] || !v[14]) && (v[14] || v[15]));
}

void print_solution(int32_t _sol)
{
	std::cout << "found solution '0x" << std::hex << _sol << "' = " << std::bitset<16>(_sol) << std::endl;
}

int main(int argc, char *argv[])
{
	std::vector<int32_t> solutions;

	for (int i = 0; i < kinput_size; i++)
	{
		if (check_circuit(i))
			solutions.push_back(i);
	}

	for (auto i : solutions)
	{
		print_solution(i);
	}

	std::cout << "Total number of solutions: " << solutions.size() << std::endl;
}