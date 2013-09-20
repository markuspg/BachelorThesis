/*! Represents a Problem to solve */
class Problem {
	public:
		/** Problem's constructor to load a specific problem
		 *
		 * @param filename The name of the file which contains the Problem to solve
		 */
		Problem (std::string filename);

		/** A copy constructor for the Problem
		 *
		 * @param rhs The Problem instance to copy
		 */
		Problem (const Problem &rhs);

		/** Problem's destructor
		 */
		~Problem ();

		/** Assigns a Process to a Machine using their ids
		 *
		 * @param pid The id of the Process
		 * @param mid The id of the Machine
		 */
		void assign_process_to_machine_by_ids (unsigned short int pid, unsigned short int mid);

		/** Checks if the current state is a feasible solution
		 *
		 * @return True, if the solution is feasible
		 */
		bool check_validity ();

		/*! Flushes the assignments */
		void flush ();

		/** Returns a pointer to the Machines
		 *
		 * @return A pointer to the Machines
		 */
		Machine **get_machines_pointer () const {return machines;}

		/** Returns the quantity of Machines
		 *
		 * @return The quantity of Machines
		 */
		unsigned short int get_machines_quantity () const {return machines_quantity;}

		/** Returns the upper bound of the processing times interval
		 *
		 * @return The maximum processing time of the Problem
		 */
		unsigned int get_process_interval () const {return process_interval;}

		/** Returns a pointer to the Processes
		 *
		 * @return A pointer to the Processes
		 */
		Process **get_processes_pointer () const {return processes;}

		/** Returns the quantity of Processes
		 *
		 * @return The quantity of Processes
		 */
		unsigned short int get_processes_quantity () const {return processes_quantity;}

		/*! Loads the temporarily stored solution */
		void load_stored_solution ();

		/** Queries the completion time of the first completing Machine
		 *
		 * @return The lowest completion time (Which shall be maximized)
		 */
		unsigned int query_lowest_completion_time ();

		/** Query the id of the Machine with the shortest or longest completion time
		 *
		 * @param placement The first (=0), second (=1) or x-th Machine fullfilling the requirement
		 * @param invert True, if the Machine with the longest completion time shall be returned
		 * @return The id of the Machine with the shortest completion time
		 */
		unsigned short int query_lowest_workload_machines_id (unsigned short int placement = 0, bool invert = false);

		/*! Writes the Problem's state to stdout */
		void query_state ();

		/*! Saves the current solution in the TemporaryStorage especially designed therefore */
		void store_current_solution ();

		/** Saves the Problem instance to a file
		 *
		 * @param filename The filename the problem's instance data shall be written to. If not given, the user will be asked for it
		 */
		void save_instance (std::string *filename);

	protected:
		Machine **machines; //!< A pointer to the Machines of the Problem
		unsigned short int machines_quantity; //!< The quantity of Machines in the Problem
		Process **processes; //!< A pointer to the Processes forming the Problem
		unsigned short int processes_quantity; //!< The quantity of Processes of the Problem
		unsigned int process_interval; //!< The upper bound of the Process durations interval
		TemporaryStorage *temporary_storage; //!< An object to temporarily store assignments
};
