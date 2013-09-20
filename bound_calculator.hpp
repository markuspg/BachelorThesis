/*! An object used to calculate bounds */
class BoundCalc : public Problem {
	public:
		/** BoundCalc's constructor
		 *
		 * @param problem A Problem to copy data from
		 */
		BoundCalc (const Problem &problem);

		/** Computes lower and upper bounds
		 *
		 * @param algo The algorithm to use
		 */
		unsigned int compute_upper_bound (unsigned int algo);

		/** Returns the quantity of the cumulated processing times of all processes
		 *
		 * @return The cumulated processing time of all processes
		 */
		unsigned int get_cumulated_processing_times () const {return cumulated_processing_times;}
	
	private:
		/*! Applies the L_DM lower bound algorithm from Haouari and Jemmali 2008
		 *
		 * @return A valid PC_max lower bound
		 */
		unsigned int apply_LDM_algrithm ();

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

		/*! Returns a valid BPP lower bound LB_BPP from Haouari and Jemmali 2008
		 *
		 * @return A valid BPP lower bound
		 */
		unsigned short int get_LB_BPP_BP_lower_bound (unsigned int capacity);

		unsigned int cumulated_processing_times; //!< The processing time of all Processes of the Problem
		unsigned int longest_processing_time; //!< The processing time of the longest Process of the Problem
		unsigned int shortest_processing_time; //!< The processing time of the shortest Process of the Problem
};
