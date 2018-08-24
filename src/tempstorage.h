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

class Machine;
class Process;

#include <vector>

/*! A temporary storage for Problem solutions */
class TemporaryStorage {
	public:
		/** TemporaryStorage's constructor
		 *
		 * @param current_solution The current solution in form of the lowest completion time
		 * @param m_machines The quantity of Machines of the Problem
		 * @param p_machines A pointer to the array of pointer to Machines to query their Process assignments
		 */
		TemporaryStorage (unsigned int current_solution, unsigned short int m_machines, Machine **p_machines);

		/*! TemporaryStorage's destructor */
		~TemporaryStorage ();

		/** Allows to load the assignments stored in the TemporaryStorage
		 */
		void load_temporarily_stored_solution (Machine **p_machines, Process **p_processes);

	private:
		unsigned short int machines_quantity; //!< Stores the quantity of Machines of the Problem
		unsigned int solution_value; //!< Stores the target value of the solution
		std::vector<Process*> **v_processes; //!< Stores the pointers of the Processes assigned to the Machines
};


#endif // TEMPORARY_STORAGE_H
