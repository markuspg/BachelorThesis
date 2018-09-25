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

#include "scheduler.h"
#include "bound_calculator.h"
#include "machine.h"
#include "process.h"

#include <chrono>
#include <iostream>
#include <list>
#include <random>
#include <vector>

/** Scheduler's constructor
 *
 * @param problem A problem to generate a starting solution for
 *
 */
/*!
 * \brief Scheduler's constructor
 * \param[in] argProblem A Problem instance to generate a start solution for
 */
bct::Scheduler::Scheduler(const Problem &argProblem) : Problem{argProblem} {
  // std::cout << "\nCreating a new Scheduler instance with the following"
  //              " specifications:\n\tMachines:\t\t" << machinesQuantity
  //           << "\n\tProcesses:\t\t" << processesQuantity << "\n\n";
}

/*!
 * \brief Applies the intelligent LPT (assign longest unassigned job to the
 * machine with lowest workload) algorithm to create a starting solution for the
 * given instance stored in the Scheduler
 */
void bct::Scheduler::Apply_iLPT_Algorithm() {
  // std::cout << "\nApplying intelligent LPT algorihm\n";
  // Iterate over all Processes
  for (unsigned short j = 0; j < processesQuantity; ++j) {
    // Query the Machine with the lowest workload and assign the Process to it
    const auto lowestWorkloadMachine = QueryLowestWorkloadMachinesId();
    // std::cout << "\tMachine with lowest workload: "
    //           << lowestWorkloadMachine << ".\n";
    AssignProcessToMachineByIds(processes[j]->GetId(), lowestWorkloadMachine);
  }
}

/*!
 * \brief Applies the randomized LPT (assign longest or second-longest
 * unassigned job to the next free machine) algorithm to create a starting
 * solution for the given instance stored in the Scheduler \param[in]
 * argIterations The quantity of to-be-conducted randomized solution attempts
 */
bool bct::Scheduler::Apply_rLPT_Algorithm(const unsigned short argIterations) {
  // std::cout << "\nApplying randomized LPT algorihm\n";
  // Copy the processes array into a std::vector<Process*>
  std::vector<Process *> v_processes;
  v_processes.reserve(processesQuantity);
  for (unsigned short j = 0; j < processesQuantity; ++j) {
    v_processes.emplace_back(processes[j]);
  }

  // Compute a bound to cancel if reached
  BoundCalc boundCalculator{*this};
  auto upperBound = boundCalculator.ComputeUpperBound(
      BoundCalc::EUpperBoundAlgos::SIMPLE_LINEAR_TIME);

  // Initialize our storage of the best solution with the highest possible value
  auto bestSolution = 0u;
  // std::cout << "bestSolution's value is " << bestSolution << "\n";
  for (unsigned short k = 0; k < argIterations; ++k) {
    // std::cout << "Iteration " << k + 1;
    Flush();
    std::vector<Process *> v_processes_cpy{v_processes};
    // Iterate over all Processes
    for (unsigned short j = 0; j < processesQuantity - 1; ++j) {
      // Query the Machine with the lowest workload and assign the first or
      // second longest Process to it
      std::default_random_engine engine{
          std::chrono::system_clock::now().time_since_epoch().count()};
      std::uniform_int_distribution<unsigned short> generator{1, 2};
      unsigned short index = generator(engine) - 1;
      AssignProcessToMachineByIds(v_processes_cpy.at(index)->GetId(),
                                  QueryLowestWorkloadMachinesId());
      // After adding the Process delete it from the copied vector
      v_processes_cpy.erase(v_processes_cpy.begin() + index);
    }
    AssignProcessToMachineByIds(v_processes_cpy.at(0)->GetId(),
                                QueryLowestWorkloadMachinesId());
    v_processes_cpy.erase(v_processes_cpy.begin());

    // Check if the solution of the iteration is better than the hitherto
    // solution and store it if it is
    if (QueryLowestCompletionTime() > bestSolution) {
      bestSolution = QueryLowestCompletionTime();
      // std::cout << "The new solution " << bestSolution
      //           << " is superior and will be stored.\n";
      StoreCurrentSolution();
      if (bestSolution == upperBound) {
        // If the new solution corresponds to the upper bound, break
        break;
      }
    } else {
      // std::cout << "The new solution " << query_lowest_completion_time () <<
      // " is not superior.\n";
    }
  }

  return LoadStoredSolution();
}

/*!
 * \brief Applies the bin covering SIMPLE (SI) algorithm from Csirik, Frenk,
 * Labbe and Zhang (1999) to create a starting solution for the given instance
 * stored in the Scheduler
 */
void bct::Scheduler::Apply_SI_Algorithm() {
  // std::cout << "\nApplying SI algorihm\n";
  // Get the upper bound as start value for the bin capacity
  BoundCalc bound_calculator{*this};
  auto upperBound = bound_calculator.ComputeUpperBound(
      BoundCalc::EUpperBoundAlgos::SIMPLE_LINEAR_TIME);
  // std::cout << "The upper bound is " << upperBound << "\n";

  for (unsigned int k = 0; k < upperBound; ++k) {
    // Determine the bin capacity of the iteration
    unsigned int C = upperBound - k;
    // std::cout << "\nTrying to fill Machines up to a least processing"
    //              " time of " << C << "\n";

    // Add all Processes to a list
    std::list<Process *> l_processes;
    for (unsigned short j = 0; j < processesQuantity; ++j) {
      l_processes.emplace_back(processes[j]);
    }

    // Consecutively fill all Machines' schedules
    for (unsigned int i = 0; i < machinesQuantity; ++i) {
      // Assign the first n processes whose cumulated size is smaller than the
      // bin capacity
      while (true) {
        if ((machines[i]->GetCompletionTime() +
             l_processes.front()->GetProcessingTime()) < C) {
          // Check if there are still Processes to assign in the list
          if (l_processes.size() < 1) {
            break;
          }
          AssignProcessToMachineByIds(l_processes.front()->GetId(),
                                      machines[i]->GetId());
          // std::cout << "\t\t  Machine's duration: "
          //           << machines[i]->GetCompletionTime() << "\n";
          l_processes.pop_front();
        } else {
          // std::cout << "\tBiggest first fitting items got assigned"
          //              ", continuing with smallest ones.\n";
          break;
        }
      }
      // Assign the last n Processes to fill the bin
      while (true) {
        if (machines[i]->GetCompletionTime() < C) {
          if (l_processes.size() < 1) {
            break;
          }
          AssignProcessToMachineByIds(l_processes.back()->GetId(),
                                      machines[i]->GetId());
          // std::cout << "\t\t  Machine's duration: "
          //           << machines[i]->GetCompletionTime() << "\n";
          l_processes.pop_back();
        } else {
          // std::cout << "\tMachine's schedule is filled, continuing"
          //              " with next Machine.\n\n";
          break;
        }
      }
    }

    // Check if current solution is valid
    if (machines[machinesQuantity - 1]->GetCompletionTime() >= C) {
      // Assign any left over Processes using the LPT algorithm
      if (l_processes.size() > 0) {
        // std::cout << "All Machines got filled, but not all Processes"
        //              " assigned. Assigning them using the LPT"
        //              " algorithm, then leaving.\n";
        for (unsigned short j = 0; j < l_processes.size(); ++j) {
          AssignProcessToMachineByIds(
              l_processes.front()->GetId(),
              machines[QueryLowestWorkloadMachinesId()]->GetId());
          l_processes.pop_front();
        }
      } else {
        // std::cout << "All Machines got filled and all Processes got"
        //              " assigned, leaving.\n";
      }
      break;
    } else {
      // std::cout << "Solution not valid, flushing and retrying with a"
      //              " lower bin capacity C.\n";
      Flush();
    }
  }
}

/*!
 * \brief Applies the simple LPT (assign longest unassigned job to the next free
 * machine) algorithm to create a starting solution for the given instance
 * stored in the Scheduler
 */
void bct::Scheduler::Apply_sLPT_Algorithm() {
  // std::cout << "\nApplying simple LPT algorihm\n";
  for (unsigned int i = 0, j = 0; j < processesQuantity; ++i, ++j) {
    AssignProcessToMachineByIds(processes[j]->GetId(), machines[i]->GetId());
    if (i == (machinesQuantity - 1))
      i = -1;
  }
}

/*!
 * \brief Applies the STUPID algorithm to create a starting solution for the
 * given instance stored in the Scheduler
 */
void bct::Scheduler::Apply_STUPID_Algorithm() {
  // std::cout << "\nApplying stupid algorithm\n";
  for (unsigned short j = 0; j < processesQuantity; ++j) {
    AssignProcessToMachineByIds(processes[j]->GetId(), machines[0]->GetId());
  }
}

/*!
 * \brief Create a start solution using the given algorithm (STUPID, iLPT, sLPT)
 * \param[in] argChoice The algorithm to use
 * \param[in] argIterations The count of iterations the algorithm shall be
 * applied (only affects non-deterministic algorithms)
 */
void bct::Scheduler::CreateStartSolution(const EStartSolution argChoice,
                                         const unsigned short argIterations) {
  switch (argChoice) {
  case EStartSolution::STUPID:
    Apply_STUPID_Algorithm();
    break;
  case EStartSolution::iLPT:
    Apply_iLPT_Algorithm();
    break;
  case EStartSolution::rLPT:
    Apply_rLPT_Algorithm(argIterations);
    break;
  case EStartSolution::SI:
    Apply_SI_Algorithm();
    break;
  case EStartSolution::sLPT:
    Apply_sLPT_Algorithm();
    break;
  default:
    std::cerr << "\nERROR: Invalid start solution algorithm\n";
  }
}
