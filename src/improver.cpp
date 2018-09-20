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
#include "improver.h"
#include "machine.h"
#include "process.h"

#include <chrono>
#include <iostream>
#include <random>

bct::Improver::Improver (const Problem &problem):
	Problem (problem)
{
	// std::cout << "\nCreating a new Improver instance with the following specifications:\n\tMachines:\t\t" << machines_quantity << "\n\tProcesses:\t\t" << processes_quantity << "\n\n";
}

void bct::Improver::apply_PAIRWISE_algorithm (bool greedy) {
	// std::cout << "\nApplying pairwise interchange algorihm\n";

	// Compute the naive upper bound (Step 2: Compute lower bound)
	BoundCalc bound_calculator (*this);
	unsigned int upper_bound = bound_calculator.compute_upper_bound (NAIVE);
	// std::cout << "The upper bound is " << upper_bound << ".\n";

	// Create std::vector<Process*> to store advantageous swaps of Step 7
	std::vector<Process*> set_a;
	std::vector<Process*> set_b;
	
	// Create pointers to store n-th highest and lowest workload Machines
	Machine *pa = nullptr;
	Machine *pb = nullptr;

	// Create stop criterion for algorithm loop
	bool stop = false;
	while (!stop) {
		for (unsigned short int k = 0; k < machinesQuantity - 1; k++) {
			// Query the maximum workload machine (Step 3: Distinguish PA)
			// std::cout << "Querying maximum workload Machine.\n";
            pa = machines[QueryLowestWorkloadMachinesId(k, true) - 1];
			if (k == 0)
				; // std::cout << "Maximum workload Machine PA is machine " << pa->get_id () << ".\n";
			else
				; // std::cout << "The " << k + 1 << ". most loaded Machine is " << pa->get_id () << ".\n";
			
			// Query  the lowest workload machine (Step 4: Distinguish PB)
            pb = machines[QueryLowestWorkloadMachinesId(0, false) - 1];
			// std::cout << "Lowest workload machine PB is Machine " << pb->get_id () << ".\n";
			
			// Stop, if the upper bound correlates to the current solution value (Step 5: If LB = M)
            if (upper_bound == this->QueryLowestCompletionTime()) {
				// std::cout << "Current schedule correlates to upper bound => Terminating.\n";
				return;
			}
			
			// Stop if all machines have the same load (Step 6: Terminate if F_PA = F_PB)
			if (pa->GetCompletionTime () == pb->GetCompletionTime ()) {
				// std::cout << "Current schedule has all Machines finishing the same time => Terminating.\n";
				return;
			}
			
			// Evaluate all advantageous swaps (Step 7: Form sets A and B)
			unsigned int maximum_difference = pa->GetCompletionTime () - pb->GetCompletionTime ();
			// std::cout << "The maximum valid difference is " << maximum_difference << ".\n";
            // Fetch assigned Processes of both Machines
            auto a_processes = pa->GetProcessesCopy();
            auto b_processes = pb->GetProcessesCopy();
			// If a swap is advantageous, store it
            for (auto ita = a_processes.cbegin(); ita != a_processes.cend(); ++ita) {
                for (auto itb = b_processes.cbegin(); itb != b_processes.cend(); ++itb) {
					if (!greedy) {
						// Allowing <= yielding in no advantageous swaps for greater exploration of the solution space
                        if ((*ita)->GetProcessingTime() >= (*itb)->GetProcessingTime()) {
                            if (((*ita)->GetProcessingTime() - (*itb)->GetProcessingTime()) <= maximum_difference) {
								set_a.push_back (*ita);
								set_b.push_back (*itb);
							}
						}
					}
					else {
						// Not using <= in both cases, because it does not lead to an advantageous swap
                        if ((*ita)->GetProcessingTime() > (*itb)->GetProcessingTime()) {
                            if (((*ita)->GetProcessingTime() - (*itb)->GetProcessingTime()) < maximum_difference) {
								set_a.push_back (*ita);
								set_b.push_back (*itb);
							}
						}
					}
				}
			}
			for (auto ita = set_a.cbegin (), itb = set_b.cbegin (); ita != set_a.cend (); ++ita, ++itb) {
				// std::cout << "Difference between PID " << (*ita)->get_id () << " & " << (*itb)->get_id () << " is " << (*ita)->get_processing_time () - (*itb)->get_processing_time () << ".\n";
			}
			
            if (set_a.size () > 0) {
				// std::cout << "A set of advantageous swaps could be formed.\n";
				break;
			}
			else {
				// std::cout << "No set of advantageous swaps could be formed, omitting Machine " << pa->get_id () << ".\n";
				set_a.clear ();
				set_b.clear ();
			}
		}
		
		if (set_a.size () > 0) {
			// Select the most advantageous swap (Step 8: Select two jobs a and b)
			unsigned int best_swap = 0;
			unsigned short int swap_index = 0;
			unsigned short int counter = 0;

			if (!greedy) {
				std::default_random_engine engine(std::chrono::system_clock::now().time_since_epoch().count());
				std::uniform_int_distribution<unsigned int> generator(1, set_a.size ());
				swap_index = generator (engine) - 1;

				// std::cout << "Swap is " << set_a.at (swap_index)->get_id () << " & " << set_b.at (swap_index)->get_id () << " is " << set_a.at (swap_index)->get_processing_time () - set_b.at (swap_index)->get_processing_time () << ".\n";
			}
			else {
				for (auto ita = set_a.cbegin (), itb = set_b.cbegin (); ita != set_a.cend (); ++ita, ++itb) {
                    if (((*ita)->GetProcessingTime() - (*itb)->GetProcessingTime()) > best_swap) {
                        best_swap = (*ita)->GetProcessingTime() - (*itb)->GetProcessingTime();
						swap_index = counter;
					}
					counter++;
				}
				// std::cout << "Best swap is " << set_a.at (swap_index)->get_id () << " & " << set_b.at (swap_index)->get_id () << " is " << set_a.at (swap_index)->get_processing_time () - set_b.at (swap_index)->get_processing_time () << ".\n";
			}
			
			// Do the swap and compute new finish times (Step 9: Exchange two jobs a and b)
			pa->AssignProcessToMachine (set_b.at (swap_index));
			pb->AssignProcessToMachine (set_a.at (swap_index));
			pa->DeleteProcessFromMachine (set_a.at (swap_index));
			pb->DeleteProcessFromMachine (set_b.at (swap_index));
			// std::cout << "New completion times are:\n\tPA = " << pa->get_completion_time () << "\n\tPB = " << pb->get_completion_time () << "\n";
			set_a.clear ();
			set_b.clear ();
		}
		// The improvement process can be terminated, since no swaps do generate new improvable solutions
		else {
			// std::cout << "No advantageous swaps can be found anymore, terminating.\n";
			stop = true;
		}
	}
}

void bct::Improver::improve_start_solution (unsigned int algo,
                                            unsigned short int iterations) {
	switch (algo) {
		case gPAIRWISE:
			apply_PAIRWISE_algorithm (true);
			break;
		case PAIRWISE:
			apply_PAIRWISE_algorithm ();
			break;
		default:
			std::cerr << "\nERROR: Invalid improvement algorithm\n";
	}
}
