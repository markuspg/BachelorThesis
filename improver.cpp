#include "improver.hpp"

Improver::Improver (const Problem &problem):
	Problem (problem)
{
}

void Improver::apply_pairwise_algorithm () {
	std::cout << "\nApplying pairwise interchange algorihm\n";
}

void Improver::improve_start_solution (unsigned int choice, unsigned short int iterations) {
	switch (choice) {
		case PAIRWISE:
			apply_pairwise_algorithm ();
			break;
		default:
			std::cerr << "\nERROR: Invalid improvement algorithm\n";
	}
}
