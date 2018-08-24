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

#ifndef IMPROVER_H
#define IMPROVER_H

#include "problem.h"

/*! A class to improve given starting solutions */
class Improver : public Problem {
	public:
		/** Improver's constructor
		 *
		 * @param problem A problem to generate a starting solution for
		 *
		 */
		Improver (const Problem &problem);

		/*! Applies the pairwise interchange algorithm of Fatemi and Ghazvini (1998) */
		void apply_gPAIRWISE_algorithm ();

		/*! Applies the pairwise interchange algorithm of Fatemi and Ghazvini (1998) */
		void apply_PAIRWISE_algorithm (bool greedy = false);

		/** Improve a start solution using the given algorithm (PAIRWISE)
		 *
		 * @param algo The algorithm to use
		 * @param iterations The count of iterations the algorithm shall be applied (only affects non-deterministic algorithms)
		 */
		void improve_start_solution (unsigned int algo, unsigned short int iterations = 50);
};

#endif // IMPROVER_H
