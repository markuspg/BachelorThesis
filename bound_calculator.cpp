#include "bound_calculator.hpp"

BoundCalc::BoundCalc (const Problem &problem):
	cumulated_processing_times (0),
	longest_processing_time (0),
	Problem (problem),
	shortest_processing_time (0)
{
	unsigned int processing_time = 0;
	for (unsigned short int j = 0; j < processes_quantity; j++) {
		processing_time = processes[j]->get_processing_time ();
		cumulated_processing_times += processing_time;
		if (processing_time > longest_processing_time)
			longest_processing_time = processing_time;
		if (processing_time < shortest_processing_time)
			shortest_processing_time = processing_time;
	}
	std::cout << "\nCreating a new BoundCalc instance with the following specifications:\n\tMachines:\t\t" << machines_quantity << "\n\tProcesses:\t\t" << processes_quantity << "\n\tUpper interval bound:\t" << process_interval << "\n\tCumulated processing times:\t" << cumulated_processing_times << "\n\tLongest processing time:\t" << longest_processing_time << "\n\n";

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

unsigned int BoundCalc::apply_SIMPLE_LINEAR_TIME_algorithm () {
	std::cout << "\nApplying SIMPLE_LINEAR_TIME algorithm\n";

	// Calculate the three needed quantities
	std::cout << "Longest processing time:\t" << longest_processing_time << "\n";
	unsigned int pmpm1 = processes[machines_quantity - 1]->get_processing_time () + processes[machines_quantity]->get_processing_time ();
	std::cout << "pmpm1:\t\t\t\t" << pmpm1 << "\n";
	unsigned int average_runtime = ((static_cast<float>(cumulated_processing_times) / machines_quantity) > static_cast<unsigned int>(cumulated_processing_times / machines_quantity)) ? static_cast<unsigned int>((cumulated_processing_times / machines_quantity) + 1) : (cumulated_processing_times / machines_quantity);
	std::cout << "Average runtime:\t\t" << average_runtime << "\n";

	if ((longest_processing_time >= pmpm1) && (longest_processing_time >= average_runtime))
		return longest_processing_time;
	if ((pmpm1 >= longest_processing_time) && (pmpm1 >= average_runtime))
		return pmpm1;
	else
		return average_runtime;
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
		case SIMPLE_LINEAR_TIME:
			return convert_PCmax_lower_bound_to_PCmin_upper_bound (apply_SIMPLE_LINEAR_TIME_algorithm ());
		default:
			std::cerr << "\nERROR: Invalid bound calculation algorithm\n";
	}
}

unsigned int BoundCalc::convert_PCmax_lower_bound_to_PCmin_upper_bound (unsigned int PCmax_lower_bound) {
	return static_cast<unsigned int>((cumulated_processing_times - PCmax_lower_bound) / (machines_quantity - 1));
}
