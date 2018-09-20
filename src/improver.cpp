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
#include "improver.h"
#include "machine.h"
#include "process.h"

#include <chrono>
#include <iostream>
#include <random>

/*!
 * \brief Improver's constructor
 * \param[in] argProblem The Problem instance to create a start solution for
 */
bct::Improver::Improver(const Problem &argProblem):
    Problem{argProblem}
{
    // std::cout << "\nCreating a new Improver instance with the following"
    //              " specifications:\n\tMachines:\t\t" << machinesQuantity
    //           << "\n\tProcesses:\t\t" << processesQuantity << "\n\n";
}

void bct::Improver::Apply_PAIRWISE_algorithm(bool argGreedy) {
    // std::cout << "\nApplying pairwise interchange algorihm\n";

    // Compute the naive upper bound (Step 2: Compute lower bound)
    BoundCalc boundCalculator{*this};
    unsigned int upperBound
            = boundCalculator.ComputeUpperBound(BoundCalc::EUpperBoundAlgos::NAIVE);
    // std::cout << "The upper bound is " << upperBound << ".\n";

    // Create std::vector<Process*> to store advantageous swaps of Step 7
    std::vector<Process*> setA;
    std::vector<Process*> setB;

    // Create pointers to store n-th highest and lowest workload Machines
    Machine *pa = nullptr;
    Machine *pb = nullptr;

    // Create stop criterion for algorithm loop
    bool stop = false;
    while (stop == false) {
        for (unsigned short k = 0; k < machinesQuantity - 1; ++k) {
            // Query the maximum workload machine (Step 3: Distinguish PA)
            // std::cout << "Querying maximum workload Machine.\n";
            pa = machines[QueryLowestWorkloadMachinesId(k, true) - 1];
            if (k == 0) {
                ; // std::cout << "Maximum workload Machine PA is machine "
                //           << pa->GetId() << ".\n";
            } else {
                ; // std::cout << "The " << k + 1 << ". most loaded Machine is "
                // << pa->GetId() << ".\n";
            }

            // Query  the lowest workload machine (Step 4: Distinguish PB)
            pb = machines[QueryLowestWorkloadMachinesId(0, false) - 1];
            // std::cout << "Lowest workload machine PB is Machine "
            // << pb->GetId() << ".\n";

            // Stop, if the upper bound correlates to the current solution value (Step 5: If LB = M)
            if (upperBound == this->QueryLowestCompletionTime()) {
                // std::cout << "Current schedule correlates to upper bound => Terminating.\n";
                return;
            }

            // Stop if all machines have the same load (Step 6: Terminate if F_PA = F_PB)
            if (pa->GetCompletionTime() == pb->GetCompletionTime ()) {
                // std::cout << "Current schedule has all Machines finishing"
                //              " the same time => Terminating.\n";
                return;
            }

            // Evaluate all advantageous swaps (Step 7: Form sets A and B)
            auto maximumDifference = pa->GetCompletionTime()
                    - pb->GetCompletionTime ();
            // std::cout << "The maximum valid difference is " << maximumDifference << ".\n";
            // Fetch assigned Processes of both Machines
            auto aProcesses = pa->GetProcessesCopy();
            auto bProcesses = pb->GetProcessesCopy();
            // If a swap is advantageous, store it
            for (auto ita = aProcesses.cbegin(); ita != aProcesses.cend(); ++ita) {
                for (auto itb = bProcesses.cbegin(); itb != bProcesses.cend(); ++itb) {
                    if (!argGreedy) {
                        // Allowing <= yielding in no advantageous swaps for greater exploration of the solution space
                        if ((*ita)->GetProcessingTime() >= (*itb)->GetProcessingTime()) {
                            if (((*ita)->GetProcessingTime() - (*itb)->GetProcessingTime())
                                    <= maximumDifference) {
                                setA.emplace_back(*ita);
                                setB.emplace_back(*itb);
                            }
                        }
                    } else {
                        // Not using <= in both cases, because it does not lead to an advantageous swap
                        if ((*ita)->GetProcessingTime() > (*itb)->GetProcessingTime()) {
                            if (((*ita)->GetProcessingTime() - (*itb)->GetProcessingTime())
                                    < maximumDifference) {
                                setA.emplace_back(*ita);
                                setB.emplace_back(*itb);
                            }
                        }
                    }
                }
            }
            for (auto ita = setA.cbegin (), itb = setB.cbegin ();
                 ita != setA.cend (); ++ita, ++itb) {
                // std::cout << "Difference between PID " << (*ita)->GetId()
                //           << " & " << (*itb)->GetId() << " is "
                //           << (*ita)->GetProcessingTime()
                //              - (*itb)->GetProcessingTime() << ".\n";
            }

            if (setA.size () > 0) {
                // std::cout << "A set of advantageous swaps could be formed.\n";
                break;
            } else {
                // std::cout << "No set of advantageous swaps could be formed,"
                //              " omitting Machine " << pa->get_id () << ".\n";
                setA.clear();
                setB.clear();
            }
        }

        if (setA.size() > 0) {
            // Select the most advantageous swap (Step 8: Select two jobs a and b)
            auto bestSwap = 0u;
            unsigned short swapIndex = 0;
            unsigned short counter = 0;

            if (argGreedy == false) {
                std::default_random_engine engine{
                    std::chrono::system_clock::now().time_since_epoch().count()};
                std::uniform_int_distribution<unsigned int>generator{1, setA.size()};
                swapIndex = generator(engine) - 1;

                // std::cout << "Swap is " << setA.at(swapIndex)->GetId()
                //           << " & " << setB.at(swapIndex)->GetId()
                //           << " is " << setA.at(swapIndex)->GetProcessingTime()
                //              - setB.at(swapIndex)->GetProcessingTime() << ".\n";
            } else {
                for (auto ita = setA.cbegin (), itb = setB.cbegin ();
                     ita != setA.cend (); ++ita, ++itb) {
                    if (((*ita)->GetProcessingTime()
                         - (*itb)->GetProcessingTime()) > bestSwap) {
                        bestSwap = (*ita)->GetProcessingTime()
                                - (*itb)->GetProcessingTime();
                        swapIndex = counter;
                    }
                    counter++;
                }
                // std::cout << "Best swap is " << setA.at(swapIndex)->GetId()
                //           << " & " << setB.at(swapIndex)->GetId() << " is "
                //           << setA.at(swapIndex)->GetProcessingTime()
                //              - setB.at(swapIndex)->GetProcessingTime() << ".\n";
            }

            // Do the swap and compute new finish times (Step 9: Exchange two jobs a and b)
            pa->AssignProcessToMachine(setB.at(swapIndex));
            pb->AssignProcessToMachine(setA.at(swapIndex));
            pa->DeleteProcessFromMachine(setA.at(swapIndex));
            pb->DeleteProcessFromMachine(setB.at(swapIndex));
            // std::cout << "New completion times are:\n\tPA = "
            //           << pa->GetCompletionTime()
            //           << "\n\tPB = " << pb->GetCompletionTime() << "\n";
            setA.clear();
            setB.clear();
        }
        // The improvement process can be terminated, since no swaps do generate
        // new improvable solutions
        else {
            // std::cout << "No advantageous swaps can be found anymore, terminating.\n";
            stop = true;
        }
    }
}

/*!
 * \brief Improve a start solution using the given algorithm (PAIRWISE)
 * \param argAlgo The algorithm to use
 * \param argIterations The count of iterations the algorithm shall be applied (only affects non-deterministic algorithms)
 */
void bct::Improver::ImproveStartSolution(const EImprovementAlgos argAlgo,
                                         const unsigned short argIterations) {
    switch (argAlgo) {
    case EImprovementAlgos::gPAIRWISE:
        Apply_PAIRWISE_algorithm(true);
        break;
    case EImprovementAlgos::PAIRWISE:
        Apply_PAIRWISE_algorithm();
        break;
    default:
        std::cerr << "\nERROR: Invalid improvement algorithm\n";
    }
}
