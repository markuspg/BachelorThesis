#include "bound_calculator.hpp"

BoundCalc::BoundCalc (const Problem &problem):
	Problem (problem)
{
	std::cout << "\nCreating a new BoundCalc instance with the following specifications:\n\tMachines:\t\t" << machines_quantity << "\n\tProcesses:\t\t" << processes_quantity << "\n\tUpper interval bound:\t" << process_interval << "\n\n";
}

float BoundCalc::apply_NAIVE_algorithm () {
	unsigned int cumulated_processing_times = 0;
	for (unsigned int i = 0; i < processes_quantity; i++) {
		cumulated_processing_times += processes[i]->get_processing_time ();
	}
	
	return ((static_cast<float>(cumulated_processing_times)/static_cast<float>(machines_quantity)) > processes[0]->get_processing_time ()) ? (static_cast<float>(cumulated_processing_times)/static_cast<float>(machines_quantity)) : processes[0]->get_processing_time ();
}

float BoundCalc::compute_lower_bound (unsigned int algo) {
	return compute_upper_bound (algo, true);
}

float BoundCalc::compute_upper_bound (unsigned int algo, bool invert) {
	switch (algo) {
		case NAIVE:
			apply_NAIVE_algorithm ();
			break;
		default:
			std::cerr << "\nERROR: Invalid start solution algorithm\n";
	}
}
