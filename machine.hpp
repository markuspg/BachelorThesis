/*! Represents a Machine */
class Machine : public Object {
	public:
		/** Machine's constructor
		 *
		 * @param mid The id of the Machine
		 * @param processes The quantity of processes of the current problem instance
		 */
		Machine (unsigned short int mid, unsigned short int processes);

		/** Assigns a Process to the Machine
		 *
		 * @param process A pointer to the Process to assign
		 */
		void assign_process_to_machine (Process *process);

		/*! Computes the Machine's completion time */
		void compute_completion_time ();

		/*! Flushes the Machine's Process vector */
		void flush ();

		/** Returns the Machine's completion time
		 *
		 * @return The Machine's completion time
		 */
		unsigned int get_completion_time ();

		/** Returns the Processes assigned to the Machine
		 *
		 * @return A vector of the Machine's Processes
		 */
		std::vector<Process*> *get_processes ();

		/** Sets the Processes assigned to the Machine to the given std::vector<Process*>
		 *
		 * @param processes_vector A std::vector containing pointers to the assigned Processes
		 */
		void set_assigned_processes_vector (std::vector<Process*> processes_vector);
		
	private:
		bool changed; //!< Saves if the completion time of the Machine changed
		unsigned int machine_completion_time; //!< The completion time of the machine
		std::vector<Process*> v_processes; //!< A vector containing pointers to the assigned Processes
};
