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

class Process;

#include <vector>

/*! Represents a Machine */
class Machine : public bct::Object {
	public:
		/** Machine's constructor
		 *
		 * @param mid The id of the Machine
		 * @param processes The quantity of processes of the current problem instance
		 */
		Machine (unsigned short int mid, unsigned short int processes);

		/** A copy constructor for the Machine
		 *
		 * @param rhs The Machine instance to copy
		 */
		Machine (const Machine &rhs);

		/** Assigns a Process to the Machine
		 *
		 * @param process A pointer to the Process to assign
		 */
		void assign_process_to_machine (Process *process);

		/*! Computes the Machine's completion time */
		void compute_completion_time ();

		/** Deletes a Process from the Machine
		 *
		 * @param process A pointer to the Process to delete
		 */
		void delete_process_from_machine (Process *process);

		/*! Flushes the Machine's Process vector */
		void flush ();

		/** Returns the Machine's completion time
		 *
		 * @return The Machine's completion time
		 */
		unsigned int get_completion_time ();

		/** Returns the Processes assigned to the Machine
		 *
		 * @return A copy of the vector of the Machine's Processes
		 */
		std::vector<Process*> *get_processes_copy () const;

		/** Sets the Processes assigned to the Machine to the given std::vector<Process*>
		 *
		 * @param processes_vector A std::vector containing pointers to the assigned Processes
		 */
		void set_assigned_processes_vector (std::vector<Process*> processes_vector);
		
	private:
		bool changed; //!< Saves if the completion time of the Machine changed
		unsigned int machine_completion_time; //!< The completion time of the machine
		std::vector<Process*> v_processes; //!< A vector containing pointers to the assigned Processes
};

#endif // MACHINE_H
