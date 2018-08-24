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
