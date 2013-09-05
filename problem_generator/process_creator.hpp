/*! An object which creates std::vector of processing times */
class ProcessCreator {
	public:
		/** ProcessCreator's constructor
		 *
		 * @param distribution The probability distribution to use
		 */
		ProcessCreator (unsigned int distribution, unsigned short int processes, unsigned int processing_times_interval_ub);
		
		unsigned int *return_processing_times ();

	private:
		std::default_random_engine *engine;
		unsigned int distribution;
		unsigned int interval;
		unsigned short int processes;
		unsigned int *processing_times;
		unsigned int seed;
};
