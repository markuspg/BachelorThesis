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

#include "tempstorage.h"

TemporaryStorage::TemporaryStorage (unsigned int current_solution, unsigned short int m_machines, Machine **p_machines):
	machines_quantity (m_machines),
	solution_value (current_solution),
	v_processes (nullptr)
{
	// std::cout << "\tCreating a temporary storage containing " << machines_quantity << " Machine's assignments.\n";
	v_processes = new std::vector<Process*> *[m_machines];
	for (unsigned short int i = 0; i < m_machines; i++) {
		v_processes[i] = p_machines[i]->get_processes_copy ();
		// std::cout << "\t  Added a vector containing " << v_processes[i]->size () << " elements representing Machine " << p_machines[i]->get_id () << ".\n";
	}
}

TemporaryStorage::~TemporaryStorage () {
	for (unsigned short int i = 0; i < machines_quantity; i++) {
		delete v_processes[i];
	}

	delete v_processes;
}

void TemporaryStorage::load_temporarily_stored_solution (Machine **p_machines, Process **p_processes) {
	// std::cout << "\nLoading the temporarily stored solution.\n";

	// Restore information on the Machines
	for (unsigned short int i = 0; i < machines_quantity; i++) {
		// std::cout << "\tFlushing Machine " << p_machines[i]->get_id () << ".\n";
		p_machines[i]->flush ();
		// std::cout << "\tAssigning old solution vector.\n";
		p_machines[i]->set_assigned_processes_vector (*v_processes[i]);
	}

	// Restores information on the Processes
	for (unsigned short int i = 0; i < machines_quantity; i++) {
		for (std::vector<Process*>::const_iterator cit = v_processes[i]->cbegin (); cit != v_processes[i]->cend (); ++cit) {
            p_processes[(*cit)->GetId() - 1]->set_assigned_machines_id (i + 1);
		}

		p_machines[i]->compute_completion_time ();
	}
}
