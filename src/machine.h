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

#include <set>

namespace bct {

class Process;

/*! Represents a Machine */
class Machine : public Object {
public:
  using ProcSet = std::set<Process *>;

  Machine(const unsigned short argMachineId);
  Machine(const Machine &argMachine);

  void AssignProcessToMachine(Process *const argProcess);
  void ComputeCompletionTime() noexcept;
  bool DeleteProcessFromMachine(Process *const argProcess);
  void Flush() noexcept;
  unsigned int GetCompletionTime() noexcept;
  ProcSet GetProcessesCopy() const;
  void SetAssignedProcesses(const ProcSet &argProcessesSet);

private:
  ProcSet assignedProccesses;
  //! Saves if the completion time of the Machine changed (_true_, if so)
  bool changed = false;
  unsigned int machineCompletionTime = 0;
};

} // namespace bct

#endif // MACHINE_H
