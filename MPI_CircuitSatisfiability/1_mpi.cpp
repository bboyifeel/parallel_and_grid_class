#include <mpi.h>
#include <iostream>
#include <bitset>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

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
	std::cout << "found solution '0x" << std::hex << _sol << "' = " << std::bitset<16>(_sol) << " = " << std::dec << _sol;
}

int main(int argc, char *argv[])
{
	int32_t p = 0;   
	int32_t id = 0;
	char processor_name[MPI_MAX_PROCESSOR_NAME];

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &p);  // Get #processes
	MPI_Comm_rank(MPI_COMM_WORLD, &id); // Get current rank

	int name_len;
	MPI_Get_processor_name(processor_name, &name_len);
	
	for (int i = id; i < kinput_size; i += p )
	{
		if (check_circuit(i))
		{
			std::cout << "[Node " << id << "] ";
			print_solution(i);
			std::cout << std::endl;
		}
	}

	std::cout << "[Node " << id << "] done!" << std::endl;

	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Finalize();
	return 0;
}
