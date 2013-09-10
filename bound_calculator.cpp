#include "bound_calculator.hpp"

BoundCalc::BoundCalc (const Problem &problem):
	cumulated_processing_times (0),
	Problem (problem)
{
	for (unsigned short int j = 0; j < processes_quantity; j++) {
		cumulated_processing_times += processes[j]->get_processing_time ();
	}
	std::cout << "\nCreating a new BoundCalc instance with the following specifications:\n\tMachines:\t\t" << machines_quantity << "\n\tProcesses:\t\t" << processes_quantity << "\n\tUpper interval bound:\t" << process_interval << "\n\tCumulated processing times: " << cumulated_processing_times << "\n\n";

}

unsigned int BoundCalc::apply_NAIVE_algorithm () {
	std::cout << "\nApplying NAIVE algorihm\n";
	unsigned short int longest_processing_time = 0;
	for (unsigned short int j = 0; j < processes_quantity; j++) {
		if (processes[j]->get_processing_time () > longest_processing_time)
			longest_processing_time = processes[j]->get_processing_time ();
	}

	// Not using the processes_quantity like in the paper, because it doesn't make any sense
	// The result can be truncated, because it should be rounded down anyway
	unsigned int average_machine_runtime = static_cast<unsigned int>(cumulated_processing_times / machines_quantity);

	return (average_machine_runtime > longest_processing_time) ? average_machine_runtime : longest_processing_time;
}

unsigned int BoundCalc::apply_SIMPLE_algorithm () {
	std::cout << "\nApplying SIMPLE algorihm\n";

	unsigned int average_machine_runtime = static_cast<unsigned int>(cumulated_processing_times / machines_quantity);
	
	return average_machine_runtime;
}

unsigned int BoundCalc::compute_lower_bound (unsigned int algo) {
	return compute_upper_bound (algo, true);
}

unsigned int BoundCalc::compute_upper_bound (unsigned int algo, bool invert) {
	switch (algo) {
		case NAIVE:
			return apply_NAIVE_algorithm ();
		case SIMPLE:
			return apply_SIMPLE_algorithm ();
		default:
			std::cerr << "\nERROR: Invalid start solution algorithm\n";
	}
}

unsigned int BoundCalc::convert_PCmax_lower_bound_to_PCmin_upper_bound (unsigned int PCmax_lower_bound) {
	return ((cumulated_processing_times - PCmax_lower_bound) / (machines_quantity - 1));
}
