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
                                        Machine **argMachines):
    machinesQuantity{argMachinesQty},
    solutionValue{argCurrentSolution}
{
    // std::cout << "\tCreating a temporary storage containing "
    //           << machinesQuantity << " Machine's assignments.\n";
    processes = new std::vector<Process*>*[argMachinesQty];
    for (unsigned short i = 0; i < argMachinesQty; ++i) {
        processes[i] = new std::vector<Process*>{argMachines[i]->GetProcessesCopy()};
        // std::cout << "\t  Added a vector containing " << processes[i]->size()
        //           << " elements representing Machine "
        //           << argMachines[i]->GetId() << ".\n";
	}
}

bct::TemporaryStorage::~TemporaryStorage () {
    for (unsigned short i = 0; i < machinesQuantity; ++i) {
        delete processes[i];
	}

    delete processes;
}

/*!
 * \brief Allows to load the assignments stored in the TemporaryStorage
 * \param argMachines Pointer to the Machines to be set up
 * \param argProcesses Pointer to the Processes to be set up
 */
void bct::TemporaryStorage::LoadTemporarilyStoredSolution(
        Machine **argMachines, Process **argProcesses) {
	// std::cout << "\nLoading the temporarily stored solution.\n";

	// Restore information on the Machines
    for (unsigned short i = 0; i < machinesQuantity; ++i) {
        // std::cout << "\tFlushing Machine " << argMachines[i]->GetId() << ".\n";
        argMachines[i]->Flush();
		// std::cout << "\tAssigning old solution vector.\n";
        argMachines[i]->SetAssignedProcessesVector(*processes[i]);
	}

	// Restores information on the Processes
    for (unsigned short i = 0; i < machinesQuantity; ++i) {
        for (std::vector<Process*>::const_iterator cit = processes[i]->cbegin ();
             cit != processes[i]->cend (); ++cit) {
            argProcesses[(*cit)->GetId() - 1]->SetAssignedMachinesId(i + 1);
		}

        argMachines[i]->ComputeCompletionTime();
	}
}
