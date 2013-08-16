/*! Represents a Problem to solve */
class Problem {
	public:
		/** Problem's constructor for new problems
		 *
		 * @param m_machines The quantity of Machines the Problem's instance shall have
		 * @param m_processes The quantity of Processes the Problem's instance shall have
		 * @param workload_interval The interval the process durations shall be contained in
		 */
		Problem (unsigned short int m_machines, unsigned short int n_processes, unsigned int workload_interval);

		/** Problem's constructor to load a specific problem
		 *
		 * @param filename The name of the file which contains the Problem to solve
		 */
		Problem (std::string filename);

		/** A copy constructor for the Problem
		 *
		 * @param rhs The Problem instance to copy
		 */
		Problem (const Problem &problem);

		/*! Problem's destructor */
		~Problem ();

		/** Compares two integers and returns true if the first one is bigger than the second one
		 *
		 * @param i The first integer
		 * @param j The second integer
		 */
		// bool compare (unsigned int i, unsigned int j);

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

		/*! Writes the problem to the stdout */
		void query_problem_state ();

		/** Saves the Problem instance to a file
		 *
		 * @param filename The filename the problem's instance data shall be written to. If not given, the user will be asked for it
		 */
		void save_problem_instance (std::string *filename);

	protected:
		Machine **machines;
		unsigned short int machines_quantity;
		Process **processes;
		unsigned short int processes_quantity;
		unsigned int process_interval;
};
