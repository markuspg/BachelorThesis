#include "scheduler.hpp"

Scheduler::Scheduler (const Problem &problem):
	Problem (problem)
{
	std::cout << "\nCreating a new Scheduler instance with the following specifications:\n\tMachines:\t\t" << machines_quantity << "\n\tProcesses:\t\t" << processes_quantity << "\n\n";
}

void Scheduler::apply_iLPT_algorithm () {
	std::cout << "\nApplying intelligent LPT algorihm\n";
	// Iterate over all Processes
	for (unsigned short int i = 0; i < processes_quantity; i++) {
		// Query the Machine with the lowest workload and assign the Process to it
		unsigned short int lowest_workload_machine = query_lowest_workload_machines_id ();
		std::cout << "\tMachine with lowest workload: " << lowest_workload_machine << ".\n";
		assign_process_to_machine_by_ids (processes[i]->get_id (), lowest_workload_machine);
	}
}

void Scheduler::apply_rLPT_algorithm (unsigned short int iterations) {
	std::cout << "\nApplying randomized LPT algorihm\n";
	// Copy the processes array into a std::vector<Process*>
	std::vector<Process*> *v_processes = nullptr;
	v_processes = new std::vector<Process*> ();
	v_processes->reserve (processes_quantity);
	for (unsigned short int i = 0; i < processes_quantity; i++) {
		v_processes->push_back (processes[i]);
	}

	// Initialize our storage of the best solution with the highest possible value
	unsigned int best_solution = 0;
	std::cout << "best_solution's value is " << best_solution << "\n";
	for (unsigned short int j = 0; j < iterations; j++) {
		std::cout << "Iteration " << j + 1;
		flush ();
		std::vector<Process*> v_processes_cpy (*v_processes);
		// Iterate over all Processes
		for (unsigned short int k = 0; k < processes_quantity - 1; k++) {
			// Query the Machine with the lowest workload and assign the first or second longest Process to it
			unsigned short int index = rand () % 2;
			assign_process_to_machine_by_ids (v_processes_cpy.at(index)->get_id (), query_lowest_workload_machines_id ());
			// After adding the Process delete it from the copied vector
			v_processes_cpy.erase (v_processes_cpy.begin () + index);
		}
		assign_process_to_machine_by_ids (v_processes_cpy.at(0)->get_id (), query_lowest_workload_machines_id ());
		v_processes_cpy.erase (v_processes_cpy.begin ());

		// Check if the solution of the iteration is better than the hitherto solution and store it if it is
		if (query_lowest_completion_time () > best_solution) {
			best_solution = query_lowest_completion_time ();
			std::cout << "The new solution " << best_solution << " is superior and will be stored.\n";
			store_current_solution ();
		}
		else {
			std::cout << "The new solution " << query_lowest_completion_time () << " is not superior.\n";
		}
	}

	load_stored_solution ();

	delete v_processes;
}

void Scheduler::apply_SI_algorithm () {
	std::cout << "\nApplying SI algorihm\n";
	// Get the upper bound as start value for the bin capacity
	BoundCalc bound_calculator (*this);
	unsigned int upper_bound = bound_calculator.compute_upper_bound (SIMPLE_LINEAR_TIME);
	std::cout << "The upper bound is " << upper_bound << "\n";

	for (unsigned int k = 0; k < upper_bound; k++) {
		// Determine the bin capacity of the iteration
		unsigned int C = upper_bound - k;
		std::cout << "\nTrying to fill Machines up to a least processing time of " << C << "\n";

		// Add all Processes to a list
		std::list<Process*> l_processes;
		for (unsigned short int j = 0; j < processes_quantity; j++) {
			l_processes.push_back (processes[j]);
		}

		// Consecutively fill all Machines' schedules
		for (unsigned int i = 0; i < machines_quantity; i++) {
			// Assign the first n processes whose cumulated size is smaller than the bin capacity
			while (true) {
				if ((machines[i]->get_completion_time () + l_processes.front ()->get_processing_time ()) < C) {
					// Check if there are still Processes to assign in the list
					if (l_processes.size () < 1) {
						break;
					}
					assign_process_to_machine_by_ids (l_processes.front ()->get_id (), machines[i]->get_id ());
					std::cout << "\t\t  Machine's duration: " << machines[i]->get_completion_time () << "\n";
					l_processes.pop_front ();
				}
				else {
					std::cout << "\tBiggest first fitting items got assigned, continuing with smallest ones.\n";
					break;
				}
			}
			// Assign the last n Processes to fill the bin
			while (true) {
				if (machines[i]->get_completion_time () < C) {
					if (l_processes.size () < 1) {
						break;
					}
					assign_process_to_machine_by_ids (l_processes.back ()->get_id (), machines[i]->get_id ());
					std::cout << "\t\t  Machine's duration: " << machines[i]->get_completion_time () << "\n";
					l_processes.pop_back ();
				}
				else {
					std::cout << "\tMachine's schedule is filled, continuing with next Machine.\n\n";
					break;
				}
			}
		}

		// Check if current solution is valid
		if (machines[machines_quantity - 1]->get_completion_time () >= C) {
			// Assign any left over Processes using the LPT algorithm
			if (l_processes.size () > 0) {
				std::cout << "All Machines got filled, but not all Processes assigned. Assigning them using the LPT algorithm, then leaving.\n";
				for (unsigned short int j = 0; j < l_processes.size (); j++) {
					assign_process_to_machine_by_ids (l_processes.front ()->get_id (), machines[query_lowest_workload_machines_id ()]->get_id ());
					l_processes.pop_front ();
				}
			}
			else {
				std::cout << "All Machines got filled and all Processes got assigned, leaving.\n";
			}
			break;
		}
		else {
			std::cout << "Solution not valid, flushing and retrying with a lower bin capacity C.\n";
			flush ();
		}
	}
}

void Scheduler::apply_sLPT_algorithm () {
	std::cout << "\nApplying simple LPT algorihm\n";
	for (unsigned int i = 0, j = 0; i < processes_quantity; i++, j++) {
		assign_process_to_machine_by_ids (processes[i]->get_id (), machines[j]->get_id ());
		if (j == (machines_quantity - 1))
			j = -1;
	}
}

void Scheduler::apply_STUPID_algorithm () {
	std::cout << "\nApplying stupid algorithm\n";
	for (unsigned short int i = 0; i < processes_quantity; i++) {
		assign_process_to_machine_by_ids (processes[i]->get_id (), machines[0]->get_id ());
	}
}

void Scheduler::create_start_solution (unsigned int choice, unsigned short int iterations) {
	switch (choice) {
		case STUPID:
			apply_STUPID_algorithm ();
			break;
		case iLPT:
			apply_iLPT_algorithm ();
			break;
		case rLPT:
			apply_rLPT_algorithm (iterations);
			break;
		case SI:
			apply_SI_algorithm ();
			break;
		case sLPT:
			apply_sLPT_algorithm ();
			break;
		default:
			std::cerr << "\nERROR: Invalid start solution algorithm\n";
	}
}
