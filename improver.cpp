#include "improver.hpp"

Improver::Improver (const Problem &problem):
	Problem (problem)
{
	std::cout << "\nCreating a new Improver instance with the following specifications:\n\tMachines:\t\t" << machines_quantity << "\n\tProcesses:\t\t" << processes_quantity << "\n\tUpper interval bound:\t" << process_interval << "\n\n";
}

void Improver::apply_pairwise_algorithm () {
	std::cout << "\nApplying pairwise interchange algorihm\n";

	// Compute the naive upper bound (Step 2: Compute lower bound)
	typedef boost::numeric::converter<int,float> float2int;
	BoundCalc bound_calculator (*this);
	// The result can be truncated, because it should be rounded down anyway
	unsigned int upper_bound = static_cast<unsigned int>(bound_calculator.compute_upper_bound (NAIVE));
	std::cout << "The upper bound is " << upper_bound << ".\n";

	while (true) {
		// Query the maximum workload machine (Step 3: Distinguish PA)
		Machine *pa = machines[query_lowest_workload_machines_id (true) - 1];
		std::cout << "Maximum workload machine PA is machine " << pa->get_id () << ".\n";
		
		// Query  the lowest workload machine (Step 4: Distinguish PB)
		Machine *pb = machines[query_lowest_workload_machines_id () -1 ];
		std::cout << "Lowest workload machine PB is machine " << pb->get_id () << ".\n";
		
		// Stop, if the lower bound correlates to the current solution value (Step 5: If LB = M)
		if (upper_bound == this->query_lowest_completion_time ()) {
			std::cout << "Current schedule correlates to upper bound => Terminating.\n";
			return;
		}
		
		// Stop if all machines have the same load (Step 6: Terminate if F_PA = F_PB)
		if (pa->get_completion_time () == pb->get_completion_time ()) {
			std::cout << "Current schedule has all machines finishing the same time => Terminating.\n";
			return;
		}

		// Evaluate all advantageous swaps (Step 7: Form sets A and B)
		unsigned int maximum_difference = pa->get_completion_time () - pb->get_completion_time ();
		std::cout << "The maximum valid difference is " << maximum_difference << ".\n";
		std::vector<Process*> *a_processes = nullptr, *b_processes = nullptr;
		a_processes = pa->get_processes ();
		b_processes = pb->get_processes ();
		std::vector<unsigned int> set_a ();
		std::vector<unsigned int> set_b ();
		// for (std::vector<Process*>::iterator ita = a_processes->cbegin (); ita != a_processes->cend (); ++ita) {
		for (auto ita = a_processes->cbegin (); ita != a_processes->cend (); ++ita) {
			for (auto itb = b_processes->cbegin (); itb != b_processes->cend (); ++itb) {
				if (0 <= ((*ita)->get_processing_time () - (*itb)->get_processing_time ()) <= maximum_difference) {
					set_a 
				}
			}
		}
		delete a_processes;
		delete b_processes;

		break;
	}
}

void Improver::improve_start_solution (unsigned int algo, unsigned short int iterations) {
	switch (algo) {
		case PAIRWISE:
			apply_pairwise_algorithm ();
			break;
		default:
			std::cerr << "\nERROR: Invalid improvement algorithm\n";
	}
}
