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

#ifndef MACHINE_H
#define MACHINE_H

#include "object.h"

#include <vector>

namespace bct {

class Process;

/*! Represents a Machine */
class Machine : public Object {
public:
    using ProcVec = std::vector<Process*>;
    /*!
     * \brief Machine's constructor
     * \param[in] argMachineId The id of the to-be-represented machine
     * \param[in] argTotalProcessQty The problem instances total quantity of processes
     */
    Machine(const unsigned short argMachineId,
            const unsigned short argTotalProcessQty);
    Machine(const Machine &argMachine);

    void AssignProcessToMachine(Process *const argProcess);
    void ComputeCompletionTime() noexcept;
    bool DeleteProcessFromMachine(Process *const argProcess);
    void Flush() noexcept;
    unsigned GetCompletionTime() noexcept;
    ProcVec GetProcessesCopy() const;
    void SetAssignedProcessesVector(const ProcVec &argProcessesVec);

private:
    ProcVec assignedProccessesVec;
    //! Saves if the completion time of the Machine changed (_true_, if so)
    bool changed = false;
    unsigned int machineCompletionTime = 0;
};

} // namespace bct

#endif // MACHINE_H
