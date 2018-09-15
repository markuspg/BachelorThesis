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

#ifndef PROCESS_H
#define PROCESS_H

#include "object.h"

namespace bct {

/*! Represents a Process */
class Process: public Object {
public:
    Process(const unsigned short argPid, const unsigned argTime);
    Process(const Process &argProcess);

    unsigned short GetAssignedMachinesId() const noexcept;
    unsigned GetProcessingTime() const noexcept;
    void SetAssignedMachinesId(const unsigned short argMachineId) noexcept;

private:
    //! Stores if the Process is assigned to a Machine (>0 if assigned to a Machine)
    unsigned short assignedMachinesId = 0;
    //! The processing time needed to complete the Process
    unsigned processingTime;
};

} // namespace bct

inline unsigned short bct::Process::GetAssignedMachinesId() const noexcept {
    return  assignedMachinesId;
}

inline unsigned bct::Process::GetProcessingTime() const noexcept {
    return processingTime;
}

inline void bct::Process::SetAssignedMachinesId(
        const unsigned short argMachineId) noexcept {
    assignedMachinesId = argMachineId;
}

#endif // PROCESS_H
