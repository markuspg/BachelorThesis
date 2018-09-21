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

#include "machine.h"
#include "problem.h"
#include "process.h"
#include "tempstorage.h"

#include <fstream>
#include <iostream>
#include <sstream>

/*!
 * \brief Construct a new Problem by the passed-in file
 * \param[in] argFilename The path of the file containing the Problem instance
 */
bct::Problem::Problem(const std::string &argFilename)
{
    std::cout << "\nThe problem saved in " << argFilename << " will be loaded.\n";

    // Open a saved problem as input
    std::ifstream input_file_stream{argFilename};

    // Read the lines, store them as str and convert it into the variables (waste is used to omit commentaries)
    std::string str, waste;
    std::getline(input_file_stream, waste);
    std::getline(input_file_stream, str);
    std::istringstream{str} >> machinesQuantity;
    std::getline(input_file_stream, waste);
    std::getline(input_file_stream, str);
    std::istringstream{str} >> processesQuantity;

    std::cout << "\nCreating a new Problem instance with the following"
                 " specifications:\n\tMachines:\t\t" << machinesQuantity
              << "\n\tProcesses:\t\t" << processesQuantity << "\n\n";

    // Create Machines
    machines = new Machine*[machinesQuantity];
    for (unsigned short i = 0; i < machinesQuantity; ++i) {
        machines[i] = new Machine{i + 1, processesQuantity};
    }

    // Create Processes
    // Add the vector's elements to a new Process array
    std::getline(input_file_stream, waste);
    processes.resize(processesQuantity, nullptr);
    unsigned int duration = 0;
    for (unsigned short j = 0; j < processesQuantity - 1; ++j) {
        std::getline(input_file_stream, str, ';');
        std::istringstream{str} >> duration;
        processes[j] = new Process{j + 1, duration};
    }
    std::getline(input_file_stream, str);
    std::istringstream{str} >> duration;
    processes[processesQuantity - 1] = new Process{processesQuantity, duration};

    // Close the file
    input_file_stream.close();
}

bct::Problem::Problem(const Problem &argProblem):
    machinesQuantity{argProblem.machinesQuantity},
    processesQuantity{argProblem.processesQuantity}
{
    // std::cout << "\nCreating new Problem instance with the following"
    //              " specifications:\n\tMachines:\t\t" << machinesQuantity
    //           << "\n\tProcesses:\t\t" << processesQuantity << "\n\n";

    // Create Machines
    Machine **tempMachines = nullptr;
    tempMachines = argProblem.GetMachinesPointer();
    machines = new Machine*[machinesQuantity];
    for (unsigned short i = 0; i < machinesQuantity; ++i) {
        machines[i] = new Machine{*tempMachines[i]};
    }

    // Create Processes
    const auto &tempProcesses = argProblem.GetProcesses();
    processes.resize(processesQuantity, nullptr);
    for (unsigned short j = 0; j < processesQuantity; ++j) {
        processes[j] = new Process{*tempProcesses[j]};
    }
}


bct::Problem::~Problem () {
    for (unsigned short i = 0; i < machinesQuantity; ++i) {
        delete machines[i];
    }
    delete [] machines;

    for (const auto process : processes) {
        delete process;
    }
}

void bct::Problem::AssignProcessToMachineByIds(const unsigned short argPid,
                                               const unsigned short argMid) {
    // std::cout << "\t  Assigning process " << processes[argPid - 1]->GetId()
    //           << " with a duration of "
    //           << processes[argPid - 1]->GetProcessingTime() << " to machine "
    //           << machines[argMid - 1]->GetId() << "\n";
    machines[argMid - 1]->AssignProcessToMachine(processes[argPid - 1]);
    processes[argPid - 1]->SetAssignedMachinesId(argMid);
}

/*!
 * \brief Check if the current state is a feasible solution
 * \return _True_, if the solution is feasible
 */
bool bct::Problem::CheckValidity() {
    for (unsigned short j = 0; j < processesQuantity; ++j) {
        // Check if every Process is assigned to a Machine
        if (processes[j]->GetAssignedMachinesId() == 0) {
            // std::cout << "\t  Process " << processes[j]->GetId()
            //           << " is not yet assigned => no feasible solution.\n";
            return false;
        }
        // Check if all Processes are assigned to valid Machines
        if (processes[j]->GetAssignedMachinesId() > machinesQuantity) {
            // std::cout << "\t  Process " << processes[j]->GetId()
            //           << " is not assigned to a valid machine => no feasible solution.\n";
            return false;
        }
    }

    // std::cout << "\t  All Processes are assigned => feasible solution.\n";
    return true;
}

/*!
 * \brief Flush (~clear) the assignments
 */
void bct::Problem::Flush () {
    // std::cout << "\nFlushing ...\n";

    // Flush the information stored in the Processes
    for (unsigned short j = 0; j < processesQuantity; ++j) {
        processes[j]->SetAssignedMachinesId(0);
    }

    // Flush the information stored in the Machines
    for (unsigned short i = 0; i < machinesQuantity; ++i) {
        machines[i]->Flush();
    }
}

void bct::Problem::LoadStoredSolution () {
    // Load the data stored in temporary_storage and delete it afterwards
    // Load the assignments of the Machines
    temporaryStorage->LoadTemporarilyStoredSolution(machines, processes);
    delete temporaryStorage;
    temporaryStorage = nullptr;
}

/*!
 * \brief Queries the completion time of the first completing Machine
 * \return The lowest completion time (which shall be maximized)
 */
unsigned int bct::Problem::QueryLowestCompletionTime () {
    unsigned int lowestCompletionTime = machines[0]->GetCompletionTime();

    // Iterate over all Machines, query their completion times and return the lowest
    for (unsigned short i = 1; i < machinesQuantity; ++i) {
        if (machines[i]->GetCompletionTime() < lowestCompletionTime)
            lowestCompletionTime = machines[i]->GetCompletionTime();
    }

    return lowestCompletionTime;
}

/*!
 * \brief Query the id of the Machine with the shortest or longest completion time
 * \param[in] argPlacement The first (=0), second (=1) or x-th Machine fullfilling the requirement
 * \param[in] argInvert True, if the Machine with the longest completion time shall be returned
 * \return The id of the Machine with the shortest (longest) completion time
 */
unsigned short bct::Problem::QueryLowestWorkloadMachinesId(
        const unsigned short argPlacement, const bool argInvert) {
    // Create a vector with all Machines which will be popped subsquently later
    std::vector<Machine*> popMachines;
    popMachines.reserve(machinesQuantity);
    for (unsigned short i = 0; i < machinesQuantity; ++i) {
        popMachines.emplace_back(machines[i]);
        // std::cout << "popMachines.push_back(Machine "
        //           << popMachines.at(i)->GetId() << ")\n";
    }
    // Create the vector, where the popped Machines' ids will be stored
    std::vector<unsigned short> pushMachineIds;
    pushMachineIds.reserve(machinesQuantity);

    // Pop the Machine with the lowest or highest workload until 'pop_machines' is empty
    while (popMachines.empty() == false) {
        // Take the first Machine in 'pop_machines' as start value
        unsigned int bestWorkload = popMachines.at(0)->GetCompletionTime();
        // std::cout << "bestWorkload " << bestWorkload << "\n";
        unsigned short bestWorkloadMachine = 0;
        // std::cout << "popMachines.size () = " << popMachines.size () << "\n";
        // Search over the complete array starting by index 1, since 0 will never become true
        for (unsigned short i = 0; i < popMachines.size (); ++i) {
            // Search for lowest workload Machine (default: invert == false)
            if (argInvert == false) {
                // std::cout << "Checking for lowest workload Machine.\n";
                // Check if current Machine has a lower workload than the known one
                if (popMachines.at(i)->GetCompletionTime() < bestWorkload) {
                    // std::cout << "Superior!\n";
                    bestWorkload = popMachines.at(i)->GetCompletionTime();
                    bestWorkloadMachine = i;
                }
            }
            // Search for highest workload Machine
            else {
                // std::cout << "Checking for highest workload Machine.\n";
                // Check if current Machine has a higher workload than the known one
                if (popMachines.at(i)->GetCompletionTime() > bestWorkload) {
                    // std::cout << "Superior!\n";
                    bestWorkload = popMachines.at(i)->GetCompletionTime();
                    bestWorkloadMachine = i;
                }
            }
        }
        // std::cout << "bestWorkloadMachine: " << bestWorkloadMachine << "\n";
        // All but one Machines have been assigned, assign the last one
        // std::cout << "Machine " << popMachines.at(bestWorkloadMachine)->GetId()
        //           << "/" << popMachines.at(bestWorkloadMachine)->GetCompletionTime()
        //           << " added \n";
        pushMachineIds.emplace_back(popMachines.at(bestWorkloadMachine)->GetId());
        popMachines.erase(popMachines.begin() + (bestWorkloadMachine));
    }

    /* for (auto it = push_machine_ids.cbegin (); it != push_machine_ids.cend (); ++it) {
        std::cout << "Machine " << *it << " with workload " << machines[*it - 1]->get_completion_time () << "\n";
    } */
    return (pushMachineIds.at (argPlacement));
}

/*!
 * \brief Write the Problem's state to stdout
 */
void bct::Problem::QueryState() {
    std::cout << "\nThe problem's state:\n";

    // Output information about Processes
    std::cout << "\t" << processesQuantity << " Processes\n";
    unsigned int overallCompletionTime = 0;
    for (unsigned short j = 0; j < processesQuantity; ++j) {
        std::cout << "\t  Process " << processes[j]->GetId() <<
                     ",\tduration: " << processes[j]->GetProcessingTime() << "\n";
        overallCompletionTime += processes[j]->GetProcessingTime();
    }
    std::cout << "\t  =>With an over all completion time of "
              << overallCompletionTime << "\n";

    // Output information about Machines
    std::cout << "\t" << machinesQuantity << " Machines\n";
    // Query the Machine's Processes after having checked, if the current solution is feasible
    if (CheckValidity() == true) {
        for (unsigned int i = 0; i < machinesQuantity; ++i) {
            auto procVec = machines[i]->GetProcessesCopy();
            std::cout << "\t    Processes assigned to Machine "
                      << machines[i]->GetId() << "\n";

            // Iterate over all assigned Processes and output the ids and processing times
            for (const auto process : procVec) {
                std::cout << "\t\t" << process->GetId() << "=>"
                          << process->GetProcessingTime()<< "\n";
            }

            // Output the complete completion time of the Machine
            std::cout << "\t\t=>Machine " << machines[i]->GetId()
                      << " completion time: " << machines[i]->GetCompletionTime () << "\n";
        }

        std::cout << "\tThe current target solution value is "
                  << QueryLowestCompletionTime() << "\n";
    }

}

/*!
 * \brief Save the current solution in the TemporaryStorage especially designed therefore
 */
void bct::Problem::StoreCurrentSolution() {
    // Checks if the current solution is valid
    if (CheckValidity() == true) {
        // Stores the current solution to temporary_storage
        // std::cout << "\nThe solution is valid and will be temporarily stored.\n";
        temporaryStorage = new TemporaryStorage{QueryLowestCompletionTime(),
                machinesQuantity, machines};
    } else {
        std::cout << "\nERROR: The solution was not valid and will therefore not be stored.\n";
    }
}

void bct::Problem::SaveInstance(const std::string &argFilename) {
    std::string filename;
    if (argFilename.empty() == true) {
        // Get the name of the file to write to
        filename = "problem.pbl";
        std::cout << "\nPlease enter the file name where the problem's instance"
                     " shall be saved (default: 'problem.pbl'): ";
        std::cin >> filename;
        std::cout << "The problem will be saved in " << filename << "\n";
    }

    // Store the Problem's data in the file
    std::ofstream outputFileStream{filename};
    outputFileStream << "# Machines\n" << machinesQuantity
                     << "\n# Processes\n" << processesQuantity << "\n";

    // Store the Process durations
    outputFileStream << "# Process durations\n";
    for (unsigned short j = 0; j < processesQuantity; ++j) {
        if (j > 0) {
            outputFileStream << ";";
        }
        outputFileStream << processes[j]->GetProcessingTime();
    }

    // Check if the current solution is valid and store Machine assignemts if so
    if (CheckValidity() == true) {
        outputFileStream << "\n# Machine assignments\n";
        for (unsigned short i = 0; i < machinesQuantity; ++i) {
            outputFileStream << "# Machine " << machines[i]->GetId() << "\n";
            auto procVec = machines[i]->GetProcessesCopy();
            for (auto cit = procVec.cbegin(); cit != procVec.cend(); ++cit) {
                if (cit != procVec.cbegin()) {
                    outputFileStream << ";";
                }
                outputFileStream << (*cit)->GetId();
            }
            outputFileStream << "\n";
        }
    } else {
        outputFileStream << "\n";
    }
    outputFileStream.close ();
}
