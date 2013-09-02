/*! Represents a Process */
class Process: public Object {
	public:
		/** Process' constructor
		 *
		 * @param pid The Process' id
		 * @param time The processing time of the Process
		 */
		Process (unsigned short int pid, unsigned int time);

		/** A copy constructor for the Process
		 *
		 * @param rhs The Process instance to copy
		 */
		Process (const Process &rhs);

		/** Returns the Machine's id the Process is assigned to
		 *
		 * @return The Machine's id the Process is assigned to or 0, if the Process is not assigned
		 */
		unsigned short int get_assigned_machine () const {return assigned_machines_id;}

		/** Returns the Process' processing time
		 *
		 * @return The processing time of the Process
		 */
		unsigned int get_processing_time () const {return processing_time;}

		/** Sets the Machine's id  the Process is assigned to
		 *
		 * @param mid The id of the Machine the Process is assigned to
		 */
		void set_assigned_machines_id (unsigned int mid);

	private:
		unsigned short int assigned_machines_id; //!< Stores if the Process is assigned to a Machine (>0 if assigned to a Machine)
		unsigned int processing_time; //!< The processing time needed to complete the Process
};
