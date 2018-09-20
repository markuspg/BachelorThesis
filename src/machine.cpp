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

#include <algorithm>
#include <iostream>
#include <numeric>

bct::Machine::Machine (const unsigned short argMachineId,
                       const unsigned short argTotalProcessQty):
    Object{argMachineId}
{
    assignedProccessesVec.reserve(argTotalProcessQty);
    // std::cout << "\tCreating Machine " << argMachineId
    //           << " able to contain " << argTotalProcessQty << " Processes\n";
}

bct::Machine::Machine(const Machine &argMachine):
    Object{argMachine.GetId()},
    changed{true}
{
    assignedProccessesVec = argMachine.GetProcessesCopy();
    ComputeCompletionTime();
    // std::cout << "\tCopied Machine " << id
    //           << " containing " << assignedProccessesVec.size ()
    //           << " Processes with a completion time of "
    //           << this->GetCompletionTime() << "\n";
}

void bct::Machine::AssignProcessToMachine(Process *const argProcess) {
    // std::cout << "Assigning Process " << argProcess->GetId()
    //           << " to Machine " << this->GetId() << ".\n";
    assignedProccessesVec.emplace_back(argProcess);
    changed = true;
}

void bct::Machine::ComputeCompletionTime() noexcept {
    // Reset machine completion time
    machineCompletionTime = 0;

    // Recompute it
    std::accumulate(assignedProccessesVec.cbegin(), assignedProccessesVec.cend(),
                    machineCompletionTime,
                    [](const unsigned int a, const Process *const b){
                        return a + b->GetProcessingTime();
    });

    changed = false;
}

bool bct::Machine::DeleteProcessFromMachine(Process *const argProcess) {
    const auto it = std::find(assignedProccessesVec.begin(),
                              assignedProccessesVec.end(),
                              argProcess);
    if (it != std::end(assignedProccessesVec)) {
        // std::cout << "Deleting Process " << (*it)->GetId()
        //           << "/" << argProcess->GetId()
        //           << " from Machine " << this->GetId() << ".\n";
        assignedProccessesVec.erase(it);
        changed = true;
        return true;
    }
    return false;
}

/*!
 * \brief Resets the Machine to zero, delete all Process assignments
 */
void bct::Machine::Flush() noexcept {
    assignedProccessesVec.clear();
    changed = false;
    machineCompletionTime = 0;
}

unsigned bct::Machine::GetCompletionTime() noexcept {
    if (changed == false) {
        return machineCompletionTime;
    } else {
        ComputeCompletionTime();
        return machineCompletionTime;
    }
}

/*!
 * \brief Gets all Processes assigned to the machine
 * \return A vector containing all the Processes of the Machine
 */
std::vector<bct::Process*> bct::Machine::GetProcessesCopy() const {
    return assignedProccessesVec;
}

void bct::Machine::SetAssignedProcessesVector(const ProcVec &argProcessesVec) {
    assignedProccessesVec = argProcessesVec;
    changed = true;
}
