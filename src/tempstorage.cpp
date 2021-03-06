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
#include "process.h"
#include "tempstorage.h"

#include <iostream>

/*!
 * \brief TemporaryStorage's constructor
 * \param argCurrentSolution The current solution in form of the lowest completion time
 * \param argMachinesQty The quantity of Machines of the Problem
 * \param argMachines A pointer to the array of pointer to Machines to query their Process
 */
bct::TemporaryStorage::TemporaryStorage(const unsigned int argCurrentSolution,
                                        const unsigned short argMachinesQty,
                                        const std::vector<Machine*> &argMachines):
    machinesQuantity{argMachinesQty},
    processes(argMachinesQty),
    solutionValue{argCurrentSolution}
{
    // std::cout << "\tCreating a temporary storage containing "
    //           << machinesQuantity << " Machine's assignments.\n";
    for (unsigned short i = 0; i < argMachinesQty; ++i) {
        processes[i] = argMachines[i]->GetProcessesCopy();
        // std::cout << "\t  Added a vector containing " << processes[i].size()
        //           << " elements representing Machine "
        //           << argMachines[i]->GetId() << ".\n";
    }
}

/*!
 * \brief Allows to load the assignments stored in the TemporaryStorage
 * \param argMachines Pointer to the Machines to be set up
 * \param argProcesses Pointer to the Processes to be set up
 */
void bct::TemporaryStorage::LoadTemporarilyStoredSolution(
        const std::vector<Machine*> &argMachines,
        const std::vector<Process*> &argProcesses) {
    // std::cout << "\nLoading the temporarily stored solution.\n";

    // Restore information on the Machines
    for (unsigned short i = 0; i < machinesQuantity; ++i) {
        // std::cout << "\tFlushing Machine " << argMachines[i]->GetId() << ".\n";
        argMachines[i]->Flush();
        // std::cout << "\tAssigning old solution vector.\n";
        argMachines[i]->SetAssignedProcesses(processes[i]);
    }

    // Restores information on the Processes
    for (unsigned short i = 0; i < machinesQuantity; ++i) {
        for (const auto process : processes[i]) {
            argProcesses[process->GetId() - 1]->SetAssignedMachinesId(i + 1);
        }

        argMachines[i]->ComputeCompletionTime();
    }
}
