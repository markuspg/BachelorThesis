/*! An object used to calculate bounds */
class BoundCalc : public Problem {
	public:
		/** BoundCalc's constructor
		 *
		 * @param problem A Problem to copy data from
		 */
		BoundCalc (const Problem &problem);

		/** An alias for 'compute_upper_bounds' for lower bounds
		 *
		 * @param algo The algorithm to use
		 */
		float compute_lower_bound (unsigned int algo);

		/** Computes lower and upper bounds
		 *
		 * @param algo The algorithm to use
		 * @param invert True, if the lower bound shall be returned
		 */
		float compute_upper_bound (unsigned int algo, bool invert = false);
	
	private:
		/*! Applies the NAIVE algorithm to calculate an upper bound from Ghomi and Ghazvini 1998
		 * 
		 * @return The maximum of the cumulated processing times divided by the quantity of machines and the longest process
		 */
		float apply_NAIVE_algorithm ();

		/*! Applies the U_0 upper bound from Haouari and Jemmali 2008
		 *
		 * @return The average processing time over all machines in the case of perfect balanced distribution
		 */
		float apply_SIMPLE_algorithm ();

		unsigned int cumulated_processing_times;
};
