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

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "problem.h"

/*! A class to generate and represent starting solutions */
class Scheduler : public Problem {
	public:
		/** Scheduler's constructor
		 *
		 * @param problem A problem to generate a starting solution for
		 *
		 */
		Scheduler (const Problem &problem);

		/*! Applies the intelligent LPT (assign longest unassigned job to the machine with lowest workload) algorithm to create a starting solution for the given instance stored in the Scheduler */
		void apply_iLPT_algorithm ();

		/*! Applies the randomized LPT (assign longest or second-longest unassigned job to the next free machine) algorithm to create a starting solution for the given instance stored in the Scheduler */
		void apply_rLPT_algorithm (unsigned short int iterations);

		/*! Applies the bin covering SIMPLE (SI) algorithm from Csirik, Frenk, Labbe and Zhang (1999) to create a starting solution for the given instance stored in the Scheduler */
		void apply_SI_algorithm ();

		/*! Applies the simple LPT (assign longest unassigned job to the next free machine) algorithm to create a starting solution for the given instance stored in the Scheduler */
		void apply_sLPT_algorithm ();

		/*! Applies the STUPID algorithm to create a starting solution for the given instance stored in the Scheduler */
		void apply_STUPID_algorithm ();

		/** Create a start solution using the given algorithm (STUPID, iLPT, sLPT)
		 *
		 * @param algo The algorithm to use
		 * @param iterations The count of iterations the algorithm shall be applied (only affects non-deterministic algorithms)
		 */
		void create_start_solution (unsigned int algo, unsigned short int iterations = 50);
};

#endif // SCHEDULER_H
