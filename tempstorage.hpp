/*! A temporary storage for Problem solutions */
class TemporaryStorage {
	public:
		/** TemporaryStorage's constructor
		 *
		 * @param current_solution The current solution in form of the lowest completion time
		 * @param n_processes The quantity of Processes of the Problem
		 * @param m_machines The quantity of Machines of the Problem
		 * @param p_machines A pointer to the array of pointer to Machines to query their Process assignments
		 */
		TemporaryStorage (unsigned int current_solution, unsigned short int n_processes, unsigned short int m_machines, Machine **p_machines);

		/*! TemporaryStorage's destructor */
		~TemporaryStorage ();

		/** Allows to load the assignments stored in the TemporaryStorage
		 */
		void load_temporarily_stored_solution (Machine **p_machines);

	private:
		unsigned short int machines_quantity; //!< Stores the quantity of Machines of the Problem
		unsigned int solution_value; //!< Stores the target value of the solution
		std::vector<Process*> **v_processes; //!< Stores the ids of the Processes assigned to the Machines
};
