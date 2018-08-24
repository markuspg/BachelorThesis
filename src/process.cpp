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

Process::Process (unsigned short int pid, unsigned int time):
	Object (pid),
	assigned_machines_id (0),
	processing_time (time)
{
	// std::cout << "\tCreated Process " << id << " with a duration of " << processing_time << "\n";
}

Process::Process (const Process &process):
    Object{process.GetId()},
	assigned_machines_id (process.get_assigned_machines_id ()),
	processing_time (process.get_processing_time ())
{
	// std::cout << "\tCreated Process " << id << " with a duration of " << processing_time << "\n";
}

void Process::set_assigned_machines_id (unsigned int mid) {
	assigned_machines_id = mid;
}
