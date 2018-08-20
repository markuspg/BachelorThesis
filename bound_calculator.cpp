#include "bound_calculator.hpp"
#include "enums.h"
#include "process.hpp"

#include <cmath>
#include <iostream>

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
	// std::cout << "\nCreating a new BoundCalc instance with the following specifications:\n\tMachines:\t\t" << machines_quantity << "\n\tProcesses:\t\t" << processes_quantity << "\n\tCumulated processing times:\t" << cumulated_processing_times << "\n\tLongest processing time:\t" << longest_processing_time << "\n\n";

}

unsigned int BoundCalc::apply_LDM_algrithm () {
	unsigned int existing_bound = apply_SIMPLE_LINEAR_TIME_algorithm ();
	unsigned int test_capacity = static_cast<unsigned int> (fmax (static_cast<double>(existing_bound - (processes[0]->get_processing_time () / 2)), static_cast<double>(existing_bound / 2)));
	// Iteratively test different bin capacities beginning with an already known lower bound
	for ( ; test_capacity < existing_bound + processes[0]->get_processing_time (); test_capacity++) {
		unsigned short int test = get_LB_BPP_BP_lower_bound (test_capacity);
		if ((test > 0) && (test  <= machines_quantity))
			break;
	}
	
	return test_capacity;
}

unsigned int BoundCalc::apply_NAIVE_algorithm () {
	// std::cout << "\nApplying NAIVE algorihm\n";

	// Not using the processes_quantity like in the paper, because it doesn't make any sense
	// The result can be truncated, because it should be rounded down anyway
	unsigned int average_machine_runtime = ((static_cast<float>(cumulated_processing_times) / static_cast<float>(machines_quantity)) > static_cast<unsigned int>(cumulated_processing_times / machines_quantity)) ? static_cast<unsigned int>((cumulated_processing_times / machines_quantity) + 1) : (cumulated_processing_times / machines_quantity);

	return (average_machine_runtime > longest_processing_time) ? average_machine_runtime : longest_processing_time;
}

unsigned int BoundCalc::apply_SIMPLE_algorithm () {
	// std::cout << "\nApplying SIMPLE algorihm\n";

	unsigned int average_machine_runtime = static_cast<unsigned int>(cumulated_processing_times / machines_quantity);
	
	return average_machine_runtime;
}

unsigned int BoundCalc::apply_SIMPLE_LINEAR_TIME_algorithm () {
	// std::cout << "\nApplying SIMPLE_LINEAR_TIME algorithm\n";

	// Calculate the three needed quantities
	// std::cout << "Longest processing time:\t" << longest_processing_time << "\n";
	unsigned int pmpm1 = processes[machines_quantity - 1]->get_processing_time () + processes[machines_quantity]->get_processing_time ();
	// std::cout << "pmpm1:\t\t\t\t" << pmpm1 << "\n";
	unsigned int average_runtime = ((static_cast<float>(cumulated_processing_times) / static_cast<float>(machines_quantity)) > static_cast<unsigned int>(cumulated_processing_times / machines_quantity)) ? static_cast<unsigned int>((cumulated_processing_times / machines_quantity) + 1) : (cumulated_processing_times / machines_quantity);
	// std::cout << "Average runtime:\t\t" << average_runtime << "\n";

	if ((longest_processing_time >= pmpm1) && (longest_processing_time >= average_runtime))
		return longest_processing_time;
	if ((pmpm1 >= longest_processing_time) && (pmpm1 >= average_runtime))
		return pmpm1;
	else
		return average_runtime;
}

unsigned int BoundCalc::compute_upper_bound (unsigned int algo) {
	switch (algo) {
		case LDM:
			return convert_PCmax_lower_bound_to_PCmin_upper_bound (apply_LDM_algrithm ());
		case NAIVE:
			return convert_PCmax_lower_bound_to_PCmin_upper_bound (apply_NAIVE_algorithm ());
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

unsigned short int BoundCalc::get_LB_BPP_BP_lower_bound (unsigned int capacity) {
	// std::cout << "\nComputing valid BPP lower bound for a capacity of " << capacity << "\n";
	// Add all valid processing times to a vector
	std::vector<unsigned int> p;
	for (unsigned int j = machines_quantity + 1; j < processes_quantity; j++) {
		if (processes[j]->get_processing_time () > static_cast<unsigned int>(capacity / 2))
			continue;
		p.push_back (processes[j]->get_processing_time ());
		// std::cout << "Added Process " << processes[j]->get_id () << " with a duration of " << processes[j]->get_processing_time () << "\n";
	}

	// Create the BPPx vectors
	std::vector<unsigned short int> BPP_1, BPP_2;

	// Iterate over the vector and create a new vector of possible bounds
	for (std::vector<unsigned int>::const_iterator it = p.cbegin (); it != p.cend (); ++it) {
		// std::cout << "p = " << *it << "\n";

		// Create the three sets of Processes
		std::vector<unsigned int> J_1, J_2, J_3;
		for (unsigned int k = 0; k < processes_quantity; k++) {
			if (processes[k]->get_processing_time () > capacity - *it)
				J_1.push_back (processes[k]->get_processing_time ());
			if ((static_cast<unsigned int>(capacity / 2) < processes[k]->get_processing_time ()) && (processes[k]->get_processing_time () <= capacity - *it))
				J_2.push_back (processes[k]->get_processing_time ());
			if ((*it <= processes[k]->get_processing_time ()) && (processes[k]->get_processing_time () <= static_cast<unsigned int>(capacity / 2)))
				J_3.push_back (processes[k]->get_processing_time ());
		}

		/* // Output of the three vectors
		std::cout << "J_1 =";
		for (std::vector<unsigned int>::const_iterator j1it = J_1.cbegin (); j1it != J_1.cend (); ++j1it) {
			std::cout << " " << *j1it;
		}
		std::cout << "\n";
		std::cout << "J_2 =";
		for (std::vector<unsigned int>::const_iterator j2it = J_2.cbegin (); j2it != J_2.cend (); ++j2it) {
			std::cout << " " << *j2it;
		}
		std::cout << "\n";
		std::cout << "J_3 =";
		for (std::vector<unsigned int>::const_iterator j3it = J_3.cbegin (); j3it != J_3.cend (); ++j3it) {
			std::cout << " " << *j3it;
		}
		std::cout << "\n"; */

		// Calculate support values
		unsigned int cumulated_processing_times_J_2 = 0, cumulated_processing_times_J_3 = 0, J_2_rest = 0;
		for (std::vector<unsigned int>::const_iterator j2it = J_2.cbegin (); j2it != J_2.cend (); ++j2it) {
			cumulated_processing_times_J_2 += *j2it;
			J_2_rest += static_cast<unsigned int> ((capacity - *j2it) / *it);
		}
		// std::cout << "cumulated_processing_times_J_2 = " << cumulated_processing_times_J_2 << "\n";
		for (std::vector<unsigned int>::const_iterator j3it = J_3.cbegin (); j3it != J_3.cend (); ++j3it) {
			cumulated_processing_times_J_3 += *j3it;
		}
		// std::cout << "cumulated_processing_times_J_3 = " << cumulated_processing_times_J_3 << "\n";

		// Calculate and store BPP_1 and BPP_2
		BPP_1.push_back (static_cast<unsigned int> (J_1.size () + J_2.size () + fmax (0, ceil (static_cast<double> (cumulated_processing_times_J_3 - static_cast<double> (capacity * J_2.size ()) + cumulated_processing_times_J_2) / static_cast<double> (capacity)))));
		BPP_2.push_back (static_cast<unsigned int> (J_1.size () + J_2.size () + fmax (0, ceil (static_cast<double> (J_3.size () - static_cast<double> (J_2_rest)) / static_cast<double> (static_cast<unsigned int> (capacity / *it))))));
	}

	/* // Display the BPPs
	std::cout << "BPP_1 =";
	for (auto BPP1it = BPP_1.cbegin (); BPP1it != BPP_1.cend (); ++BPP1it) {
		std::cout << " " << *BPP1it;
	}
	std::cout << "\n";
	std::cout << "BPP_2 =";
	for (auto BPP2it = BPP_2.cbegin (); BPP2it != BPP_2.cend (); ++BPP2it) {
		std::cout << " " << *BPP2it;
	}
	std::cout << "\n"; */

	unsigned short int max_min_bin_requirement = 0;
	for (auto BPP1it = BPP_1.cbegin (), BPP2it = BPP_2.cbegin (); BPP1it != BPP_1.cend (), BPP2it != BPP_2.cend (); ++BPP1it, ++BPP2it) {
		if (*BPP1it > max_min_bin_requirement)
			max_min_bin_requirement = *BPP1it;
		if (*BPP2it > max_min_bin_requirement)
			max_min_bin_requirement = *BPP2it;
	}

	// std::cout << "The minimum bin requirement is " << max_min_bin_requirement << "\n";

	return max_min_bin_requirement;
}
