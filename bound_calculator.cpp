#include "bound_calculator.hpp"

BoundCalc::BoundCalc (const Problem &problem):
	Problem (problem)
{
	std::cout << "\nCreating a new BoundCalc instance with the following specifications:\n\tMachines:\t\t" << machines_quantity << "\n\tProcesses:\t\t" << processes_quantity << "\n\tUpper interval bound:\t" << process_interval << "\n\n";
}

float BoundCalc::apply_NAIVE_algorithm () {
	unsigned int cumulated_processing_times = 0;
	unsigned short int longest_processing_time = 0;
	for (unsigned short int i = 0; i < processes_quantity; i++) {
		cumulated_processing_times += processes[i]->get_processing_time ();
		if (processes[i]->get_processing_time () > longest_processing_time)
			longest_processing_time = processes[i]->get_processing_time ();
	}

	// Not using the processes_quantity like in the paper, because it doesn't make any sense
	float average_machine_runtime = cumulated_processing_times / machines_quantity;

	return (average_machine_runtime > longest_processing_time) ? average_machine_runtime : longest_processing_time;
}

float BoundCalc::compute_lower_bound (unsigned int algo) {
	return compute_upper_bound (algo, true);
}

float BoundCalc::compute_upper_bound (unsigned int algo, bool invert) {
	switch (algo) {
		case NAIVE:
			return apply_NAIVE_algorithm ();
		default:
			std::cerr << "\nERROR: Invalid start solution algorithm\n";
	}
}
