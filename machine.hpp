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
		
	private:
		bool changed;
		unsigned int machine_completion_time;
		std::vector<Process*> v_processes;
};
