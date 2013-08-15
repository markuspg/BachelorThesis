/*! Represents a Process */
class Process: public Object {
	public:
		/** Process' constructor
		 *
		 * @param pid The Process' id
		 * @param time The processing time of the Process
		 */
		Process (unsigned short int pid, unsigned int time);

		/** Returns the Process' processing time
		 *
		 * @return 
		 */
		unsigned int get_processing_time () const {return processing_time;}

	private:
		unsigned int processing_time;
};
