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

#ifndef TEMPORARY_STORAGE_H
#define TEMPORARY_STORAGE_H

#include <vector>

namespace bct {

class Machine;
class Process;

/*! A temporary storage for Problem solutions */
class TemporaryStorage {
public:
    TemporaryStorage (const unsigned int argCurrentSolution,
                      const unsigned short argMachinesQty,
                      Machine **argMachines);
    void LoadTemporarilyStoredSolution(Machine **argMachines,
                                       Process **argProcesses);

private:
    //! Stores the quantity of Machines of the Problem
    const unsigned short int machinesQuantity;
    //! Stores the pointers of the Processes assigned to the Machines
    std::vector<std::vector<Process*>> processes;
    //! Stores the target value of the solution
    const unsigned int solutionValue;
};

} // namespace bct

#endif // TEMPORARY_STORAGE_H
