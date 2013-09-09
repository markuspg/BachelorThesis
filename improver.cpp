#include "improver.hpp"

Improver::Improver (const Problem &problem):
	Problem (problem)
{
	std::cout << "\nCreating a new Improver instance with the following specifications:\n\tMachines:\t\t" << machines_quantity << "\n\tProcesses:\t\t" << processes_quantity << "\n\tUpper interval bound:\t" << process_interval << "\n\n";
}

void Improver::apply_pairwise_algorithm () {
	std::cout << "\nApplying pairwise interchange algorihm\n";
}

void Improver::improve_start_solution (unsigned int algo, unsigned short int iterations = 50) {
	switch (algo) {
		case PAIRWISE:
			apply_pairwise_algorithm ();
			break;
		default:
			std::cerr << "\nERROR: Invalid improvement algorithm\n";
	}
}
