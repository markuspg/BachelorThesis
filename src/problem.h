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

#ifndef PROBlEM_H
#define PROBlEM_H

#include <string>
#include <vector>

namespace bct {

class Machine;
class Process;
class TemporaryStorage;

/*! Represents a Problem to solve */
class Problem {
public:
    Problem(const std::string &argFilename);
    Problem (const Problem &argProblem);
    ~Problem();
    void AssignProcessToMachineByIds(const unsigned short argPid,
                                     const unsigned short argMid);
    bool CheckValidity();
    void Flush();
    Machine **GetMachinesPointer() const noexcept;
    unsigned short GetMachinesQuantity() const noexcept;
    const std::vector<Process*> &GetProcesses() const noexcept;
    unsigned short GetProcessesQuantity() const noexcept;
    void LoadStoredSolution();
    unsigned int QueryLowestCompletionTime ();
    unsigned short QueryLowestWorkloadMachinesId(
            const unsigned short argPlacement = 0, const bool argInvert = false);
    void QueryState();
    void StoreCurrentSolution();
    void SaveInstance(const std::string &argFilename = "");

protected:
    //! A pointer to the Machines of the Problem
    Machine **machines = nullptr;
    //! The quantity of Machines in the Problem
    unsigned short machinesQuantity;
    //! A vector of the Processes forming the Problem
    std::vector<Process*> processes;
    //! The quantity of Processes of the Problem
    unsigned short processesQuantity;
    //! An object to temporarily store assignments
    TemporaryStorage *temporaryStorage;
};

inline Machine **Problem::GetMachinesPointer() const noexcept {
    return machines;
}

inline unsigned short Problem::GetMachinesQuantity() const noexcept {
    return machinesQuantity;
}

inline const std::vector<Process*> &Problem::GetProcesses() const noexcept {
    return processes;
}

inline unsigned short Problem::GetProcessesQuantity() const noexcept {
    return processesQuantity;
}

} // namespace bct

#endif // PROBlEM_H
