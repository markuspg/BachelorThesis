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

#include "process.h"

#include <iostream>

bct::Process::Process(const unsigned short argPid, const unsigned int argTime)
    : Object{argPid}, processingTime{argTime} {
  // std::cout << "\tCreated Process " << id
  //           << " with a duration of " << processingTime << "\n";
}

bct::Process::Process(const Process &argProcess)
    : Object{argProcess.id}, assignedMachinesId{argProcess.assignedMachinesId},
      processingTime{argProcess.processingTime} {
  // std::cout << "\tCreated Process " << id <<
  //              " with a duration of " << processingTime << "\n";
}
