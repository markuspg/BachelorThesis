/*! A class to generate and represent starting solutions */
class Scheduler : public Problem {
	public:
		/** Scheduler's constructor
		 *
		 * @param problem A problem to generate a starting solution for
		 *
		 */
		Scheduler (const Problem &problem);

		/*! Applies the intelligent LPT (assign job to machine with lowest workload) algorithm to create a starting solution for the given instance stored in the Scheduler */
		void apply_iLPT_algorithm ();

		/*! Applies the simple LPT (assign job to next free machine) algorithm to create a starting solution for the given instance stored in the Scheduler */
		void apply_sLPT_algorithm ();

		/*! Applies the STUPID algorithm to create a starting solution for the given instance stored in the Scheduler */
		void apply_STUPID_algorithm ();

		/** Create a start solution using the given algorithm (STUPID, iLPT, sLPT)
		 *
		 * @param algo The algorithm to use
		 */
		void create_start_solution (unsigned int algo);
};
