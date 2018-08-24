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

/*! Represents a Process */
class Process: public bct::Object {
	public:
		/** Process' constructor
		 *
		 * @param pid The Process' id
		 * @param time The processing time of the Process
		 */
		Process (unsigned short int pid, unsigned int time);

		/** A copy constructor for the Process
		 *
		 * @param rhs The Process instance to copy
		 */
		Process (const Process &rhs);

		/** Returns the Machine's id the Process is assigned to
		 *
		 * @return The Machine's id the Process is assigned to or 0, if the Process is not assigned
		 */
		unsigned short int get_assigned_machines_id () const {return assigned_machines_id;}

		/** Returns the Process' processing time
		 *
		 * @return The processing time of the Process
		 */
		unsigned int get_processing_time () const {return processing_time;}

		/** Sets the Machine's id  the Process is assigned to
		 *
		 * @param mid The id of the Machine the Process is assigned to
		 */
		void set_assigned_machines_id (unsigned int mid);

	private:
		unsigned short int assigned_machines_id; //!< Stores if the Process is assigned to a Machine (>0 if assigned to a Machine)
		unsigned int processing_time; //!< The processing time needed to complete the Process
};

#endif // PROCESS_H
