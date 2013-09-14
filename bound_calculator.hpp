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
		unsigned int compute_lower_bound (unsigned int algo);

		/** Computes lower and upper bounds
		 *
		 * @param algo The algorithm to use
		 * @param invert True, if the lower bound shall be returned
		 */
		unsigned int compute_upper_bound (unsigned int algo, bool invert = false);
	
	private:
		/*! Applies the NAIVE algorithm to calculate an PC_max lower bound bound from Ghomi and Ghazvini 1998
		 *
		 * @return The maximum of the cumulated processing times divided by the quantity of machines and the longest process
		 */
		unsigned int apply_NAIVE_algorithm ();

		/*! Applies the U_0 upper bound from Haouari and Jemmali 2008
		 *
		 * @return The average processing time over all machines in the case of perfect balanced distribution
		 */
		unsigned int apply_SIMPLE_algorithm ();

		/*! Applies the simple linear-time lower bound algorithm of Dell'Amico and Martello 1995 (L_1 in Haouari and Jemmali)
		 *
		 * @return The simple linear-time lower bound
		 */
		unsigned int apply_SIMPLE_LINEAR_TIME_algorithm ();

		/*! Converts a valid PC_max lower bound to a PC_min upper bound
		 *
		 * @param PCmax_lower_bound a PC_max lower bound to convert
		 * @return A valid PC_min upper bound
		 */
		unsigned int convert_PCmax_lower_bound_to_PCmin_upper_bound (unsigned int PCmax_lower_bound);

		unsigned int cumulated_processing_times; //!< The processing time of all Processes of the Problem
		unsigned int longest_processing_time; //!< The processing time of the longest Process of the Problem
		unsigned int shortest_processing_time; //!< The processing time of the shortest Process of the Problem
};
