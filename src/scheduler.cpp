/*
 * Copyright 2013, 2018 Markus Prasser
 *
 * This file is part of bct.
 *
 *  bct is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  bct is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with bct.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "bound_calculator.h"
#include "enums.h"
#include "machine.h"
#include "process.h"
#include "scheduler.h"

#include <chrono>
#include <iostream>
#include <list>
#include <random>
#include <vector>

bct::Scheduler::Scheduler (const Problem &problem):
	Problem (problem)
{
	// std::cout << "\nCreating a new Scheduler instance with the following specifications:\n\tMachines:\t\t" << machines_quantity << "\n\tProcesses:\t\t" << processes_quantity << "\n\n";
}

void bct::Scheduler::apply_iLPT_algorithm () {
	// std::cout << "\nApplying intelligent LPT algorihm\n";
	// Iterate over all Processes
	for (unsigned short int j = 0; j < processesQuantity; j++) {
		// Query the Machine with the lowest workload and assign the Process to it
		unsigned short int lowest_workload_machine = QueryLowestWorkloadMachinesId ();
		// std::cout << "\tMachine with lowest workload: " << lowest_workload_machine << ".\n";
        AssignProcessToMachineByIds(processes[j]->GetId(), lowest_workload_machine);
	}
}

void bct::Scheduler::apply_rLPT_algorithm (unsigned short int iterations) {
	// std::cout << "\nApplying randomized LPT algorihm\n";
	// Copy the processes array into a std::vector<Process*>
	std::vector<Process*> *v_processes = nullptr;
	v_processes = new std::vector<Process*> ();
	v_processes->reserve (processesQuantity);
	for (unsigned short int j = 0; j < processesQuantity; j++) {
		v_processes->push_back (processes[j]);
	}

	// Compute a bound to cancel if reached
	BoundCalc *bound_calculator = new BoundCalc (*this);
	unsigned int upper_bound = bound_calculator->ComputeUpperBound (SIMPLE_LINEAR_TIME);

	// Initialize our storage of the best solution with the highest possible value
	unsigned int best_solution = 0;
	// std::cout << "best_solution's value is " << best_solution << "\n";
	for (unsigned short int k = 0; k < iterations; k++) {
		// std::cout << "Iteration " << k + 1;
        Flush();
		std::vector<Process*> v_processes_cpy (*v_processes);
		// Iterate over all Processes
		for (unsigned short int j = 0; j < processesQuantity - 1; j++) {
			// Query the Machine with the lowest workload and assign the first or second longest Process to it
			std::default_random_engine engine (std::chrono::system_clock::now().time_since_epoch().count());
			std::uniform_int_distribution<unsigned short int> generator(1, 2);
			unsigned short int index = generator (engine) - 1;
            AssignProcessToMachineByIds(v_processes_cpy.at(index)->GetId(),
                                        QueryLowestWorkloadMachinesId());
			// After adding the Process delete it from the copied vector
			v_processes_cpy.erase (v_processes_cpy.begin () + index);
		}
        AssignProcessToMachineByIds(v_processes_cpy.at(0)->GetId(),
                                    QueryLowestWorkloadMachinesId());
		v_processes_cpy.erase (v_processes_cpy.begin ());

		// Check if the solution of the iteration is better than the hitherto solution and store it if it is
		if (QueryLowestCompletionTime () > best_solution) {
			best_solution = QueryLowestCompletionTime ();
			// std::cout << "The new solution " << best_solution << " is superior and will be stored.\n";
			StoreCurrentSolution ();
			if (best_solution == upper_bound)
				// If the new solution corresponds to the upper bound, break
				break;
		}
		else {
			// std::cout << "The new solution " << query_lowest_completion_time () << " is not superior.\n";
		}
	}

    LoadStoredSolution();

	delete v_processes;
}

void bct::Scheduler::apply_SI_algorithm () {
	// std::cout << "\nApplying SI algorihm\n";
	// Get the upper bound as start value for the bin capacity
	BoundCalc bound_calculator (*this);
	unsigned int upper_bound = bound_calculator.ComputeUpperBound (SIMPLE_LINEAR_TIME);
	// std::cout << "The upper bound is " << upper_bound << "\n";

	for (unsigned int k = 0; k < upper_bound; k++) {
		// Determine the bin capacity of the iteration
		unsigned int C = upper_bound - k;
		// std::cout << "\nTrying to fill Machines up to a least processing time of " << C << "\n";

		// Add all Processes to a list
		std::list<Process*> l_processes;
		for (unsigned short int j = 0; j < processesQuantity; j++) {
			l_processes.push_back (processes[j]);
		}

		// Consecutively fill all Machines' schedules
		for (unsigned int i = 0; i < machinesQuantity; i++) {
			// Assign the first n processes whose cumulated size is smaller than the bin capacity
			while (true) {
                if ((machines[i]->GetCompletionTime () + l_processes.front ()->GetProcessingTime()) < C) {
					// Check if there are still Processes to assign in the list
					if (l_processes.size () < 1) {
						break;
					}
                    AssignProcessToMachineByIds(l_processes.front()->GetId(),
                                                machines[i]->GetId());
					// std::cout << "\t\t  Machine's duration: " << machines[i]->get_completion_time () << "\n";
					l_processes.pop_front ();
				}
				else {
					// std::cout << "\tBiggest first fitting items got assigned, continuing with smallest ones.\n";
					break;
				}
			}
			// Assign the last n Processes to fill the bin
			while (true) {
				if (machines[i]->GetCompletionTime () < C) {
					if (l_processes.size () < 1) {
						break;
					}
                    AssignProcessToMachineByIds(l_processes.back ()->GetId(),
                                                machines[i]->GetId());
					// std::cout << "\t\t  Machine's duration: " << machines[i]->get_completion_time () << "\n";
					l_processes.pop_back ();
				}
				else {
					// std::cout << "\tMachine's schedule is filled, continuing with next Machine.\n\n";
					break;
				}
			}
		}

		// Check if current solution is valid
		if (machines[machinesQuantity - 1]->GetCompletionTime () >= C) {
			// Assign any left over Processes using the LPT algorithm
			if (l_processes.size () > 0) {
				// std::cout << "All Machines got filled, but not all Processes assigned. Assigning them using the LPT algorithm, then leaving.\n";
				for (unsigned short int j = 0; j < l_processes.size (); j++) {
                    AssignProcessToMachineByIds(
                                l_processes.front()->GetId(),
                                machines[QueryLowestWorkloadMachinesId()]->GetId());
					l_processes.pop_front ();
				}
			}
			else {
				// std::cout << "All Machines got filled and all Processes got assigned, leaving.\n";
			}
			break;
		}
		else {
			// std::cout << "Solution not valid, flushing and retrying with a lower bin capacity C.\n";
            Flush();
		}
	}
}

void bct::Scheduler::apply_sLPT_algorithm () {
	// std::cout << "\nApplying simple LPT algorihm\n";
	for (unsigned int i = 0, j = 0; j < processesQuantity; i++, j++) {
        AssignProcessToMachineByIds(processes[j]->GetId(), machines[i]->GetId());
		if (i == (machinesQuantity - 1))
			i = -1;
	}
}

void bct::Scheduler::apply_STUPID_algorithm () {
	// std::cout << "\nApplying stupid algorithm\n";
	for (unsigned short int j = 0; j < processesQuantity; j++) {
        AssignProcessToMachineByIds(processes[j]->GetId(), machines[0]->GetId());
	}
}

void bct::Scheduler::create_start_solution (unsigned int choice, unsigned short int iterations) {
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
