/*! A class to improve given starting solutions */
class Improver : public Problem {
	public:
		/** Improver's constructor
		 *
		 * @param scheduler A problem to generate a starting solution for
		 *
		 */
		Improver (const Problem &problem);

		/*! Applies the pairwise interchange algorithm of Fatemi and Ghazvini (1998) */
		void apply_pairwise_algorithm ();

		/** Improve a start solution using the given algorithm (PAIRWISE)
		 *
		 * @param algo The algorithm to use
		 * @param iterations The count of iterations the algorithm shall be applied (only affects non-deterministic algorithms)
		 */
		void improve_start_solution (unsigned int algo, unsigned short int iterations);
};
