#include "problem.hpp"

bool compare (unsigned int i, unsigned int j) {
	return (i > j);
}

Problem::Problem (unsigned short int m_machines, unsigned short int n_processes, unsigned int workload_interval):
	machines (nullptr),
	machines_quantity (m_machines),
	processes (nullptr),
	processes_quantity (n_processes),
	process_interval (workload_interval),
	temporary_storage (nullptr)
{
	std::cout << "\nCreating new Problem instance with the following specifications:\n\tMachines:\t\t" << machines_quantity << "\n\tProcesses:\t\t" << processes_quantity << "\n\tUpper interval bound:\t" << process_interval << "\n\n";

	// Create Machines
	machines = new Machine* [machines_quantity];
	for (unsigned short int i= 0; i < machines_quantity; i++) {
		machines[i] = new Machine (i + 1, processes_quantity);
	}

	// Create Processes
	// Create a vector containing randomly generated process durations
	std::vector<unsigned int> process_durations;
	process_durations.reserve (processes_quantity);
	for (unsigned short int j = 0; j < processes_quantity; j++) {
		process_durations.push_back (rand () % workload_interval + 1);
		std::cout << "\tProcess duration: " << process_durations.at (j) << "\n";
	}
	// Sort the vector in descending order
	std::sort (process_durations.begin (), process_durations.end (), compare);
	// Add the vector's elements to a new Process array
	processes = new Process* [processes_quantity];
	unsigned short int i = 0;
	for (std::vector<unsigned int>::iterator it = process_durations.begin (); it != process_durations.end (); ++it) {
		processes[i] = new Process (i + 1, *it);
		++i;
	}
}

Problem::Problem (std::string filename):
	machines (nullptr),
	processes (nullptr)
{
	std::cout << "\nThe problem saved in " << filename << " will be loaded.\n";

	// Open a saved problem as input
	std::ifstream input_file_stream (filename);

	// Read the lines, store them as str and convert it into the variables (waste is used to omit commentaries)
	std::string str, waste;
	std::getline (input_file_stream, waste);
	std::getline (input_file_stream, str);
	std::istringstream (str) >> machines_quantity;
	std::getline (input_file_stream, waste);
	std::getline (input_file_stream, str);
	std::istringstream (str) >> processes_quantity;
	std::getline (input_file_stream, waste);
	std::getline (input_file_stream, str);
	std::istringstream (str) >> process_interval;

	std::cout << "\nCreating a new Problem instance with the following specifications:\n\tMachines:\t\t" << machines_quantity << "\n\tProcesses:\t\t" << processes_quantity << "\n\tUpper interval bound:\t" << process_interval << "\n\n";

	// Create Machines
	machines = new Machine* [machines_quantity];
	for (unsigned short int i = 0; i < machines_quantity; i++) {
		machines[i] = new Machine (i + 1, processes_quantity);
	}

	// Create Processes
	// Add the vector's elements to a new Process array
	std::getline (input_file_stream, waste);
	processes = new Process* [processes_quantity];
	unsigned int duration = 0;
	for (unsigned short int i = 0; i < processes_quantity - 1; i++) {
		std::getline (input_file_stream, str, ';');
		std::istringstream (str) >> duration;
		processes[i] = new Process (i + 1, duration);
	}
	std::getline (input_file_stream, str);
	std::istringstream (str) >> duration;
	processes[processes_quantity - 1] = new Process (processes_quantity, duration);
	
	// Close the file
	input_file_stream.close();
}

Problem::Problem (const Problem &problem):
	machines_quantity (problem.get_machines_quantity ()),
	processes_quantity (problem.get_processes_quantity ()),
	process_interval (problem.get_process_interval ())
{
	std::cout << "\nCreating new Problem instance with the following specifications:\n\tMachines:\t\t" << machines_quantity << "\n\tProcesses:\t\t" << processes_quantity << "\n\tUpper interval bound:\t" << process_interval << "\n\n";
	// Create Machines
	Machine **temp_machines = nullptr;
	temp_machines = problem.get_machines_pointer ();
	machines = new Machine* [machines_quantity];
	for (unsigned short int j = 0; j < machines_quantity; j++) {
		machines[j] = new Machine (*temp_machines[j]);
	}

	// Create Processes
	Process **temp_processes = nullptr;
	temp_processes = problem.get_processes_pointer ();
	processes = new Process* [processes_quantity];
	for (unsigned short int i = 0; i < processes_quantity; i++) {
		processes[i] = new Process (*temp_processes[i]);
	}
}


Problem::~Problem () {
	for (unsigned short int j = 0; j < machines_quantity; j++) {
		delete machines[j];
	}
	delete [] machines;
	
	for (unsigned short int i = 0; i < processes_quantity; i++) {
		delete processes[i];
	}
	delete [] processes;
}

/*bool Problem::compare (unsigned int i, unsigned int j) {
	return (i > j);
}*/

void Problem::assign_process_to_machine (unsigned short int pid, unsigned short int mid) {
	std::cout << "\t  Assigning process " << processes[pid - 1]->get_id () << " with a duration of " << processes[pid - 1]->get_processing_time () << " to machine " << machines[mid - 1]->get_id () << "\n";
	machines[mid - 1]->assign_process_to_machine (processes[pid - 1]);
	processes[pid - 1]->set_assigned_machines_id (mid);
}


bool Problem::check_validity () {
	for (unsigned int i = 0; i < processes_quantity; i++) {
		// Check if every Process is assigned to a Machine
		if (processes[i]->get_assigned_machines_id () == 0) {
			std::cout << "\t  Process " << processes[i]->get_id () << " is not yet assigned => no feasible solution.\n";
			return false;
		}
		// Check if all Processes are assigned to valid Machines
		if (processes[i]->get_assigned_machines_id () > machines_quantity) {
			std::cout << "\t  Process " << processes[i]->get_id () << " is not assigned to a valid machine => no feasible solution.\n";
			return false;
		}
	}

	std::cout << "\t  All Processes are assigned => feasible solution.\n";
	return true;
}

void Problem::flush () {
	std::cout << "\nFlushing ...\n";

	// Flush the information stored in the Processes
	for (unsigned short int i = 0; i < processes_quantity; i++) {
		processes[i]->set_assigned_machines_id(0);
	}
	
	// Flush the information stored in the Machines
	for (unsigned short int j = 0; j < machines_quantity; j++) {
		machines[j]->flush ();
	}
}

void Problem::load_stored_solution () {
	// Load the data stored in temporary_storage and delete it afterwards
	// Load the assignments of the Machines
	temporary_storage->load_temporarily_stored_solution (machines);
	delete temporary_storage;
	temporary_storage = nullptr;

	// Restore the assignments stored in the Processes
	for (unsigned short int i = 0; i < machines_quantity; i++) {
		std::vector<Process*> *vecptr = nullptr;
		vecptr = machines[i]->get_processes ();
		unsigned short int id = machines[i]->get_id ();

		for (std::vector<Process*>::iterator it = vecptr->begin (); it != vecptr->end (); ++it) {
			processes[(*it)->get_id () - 1]->set_assigned_machines_id (id);
		}

		machines[i]->compute_completion_time ();

		delete vecptr;
	}
}

unsigned int Problem::query_lowest_completion_time () {
	unsigned int lowest_completion_time = machines[0]->get_completion_time ();

	// Iterate over all Machines, query their completion times and return the lowest
	for (unsigned short int i = 1; i < machines_quantity; i++) {
		if (machines[i]->get_completion_time () < lowest_completion_time)
			lowest_completion_time = machines[i]->get_completion_time ();
	}

	return lowest_completion_time;
}

unsigned short int Problem::query_lowest_workload_machines_id (bool invert) {
	unsigned short int lowest_workload_machine = 0;
	unsigned int lowest_workload = machines[0]->get_completion_time ();
	for (unsigned short int i = 1; i < machines_quantity; i++) {
		if (!invert) {
			if (machines[i]->get_completion_time () < lowest_workload) {
				lowest_workload = machines[i]->get_completion_time ();
				lowest_workload_machine = i;
			}
		}
		else {
			if (machines[i]->get_completion_time () > lowest_workload) {
				lowest_workload = machines[i]->get_completion_time ();
				lowest_workload_machine = i;
			}
		}
	}

	return (lowest_workload_machine + 1);
}

void Problem::query_state () {
	std::cout << "\nThe problem's state:\n";

	// Output information about Processes
	std::cout << "\t" << processes_quantity << " processes with durations interval [1," << process_interval << "]\n";
	unsigned int overall_completion_time = 0;
	for (unsigned short int i = 0; i < processes_quantity; i++) {
		std::cout << "\t  Process " << processes[i]->get_id () << ", duration: " << processes[i]->get_processing_time () << "\n";
		overall_completion_time += processes[i]->get_processing_time ();
	}
	std::cout << "\t  =>With an over all completion time of " << overall_completion_time << "\n";

	// Output information about Machines
	std::cout << "\t" << machines_quantity << " machines\n";
	// Query the Machine's Processes after having checked, if the current solution is feasible
	if (check_validity ()) {
		for (unsigned int j = 0; j < machines_quantity; j++) {
			std::vector <Process*> *vecptr = nullptr;
			vecptr = machines[j]->get_processes ();
			std::cout << "\t    Processes assigned to Machine " << machines[j]->get_id () << "\n";

			// Iterate over all assigned Processes and output the ids and processing times
			for (std::vector<Process*>::iterator it = vecptr->begin (); it != vecptr->end (); ++it) {
				std::cout << "\t\t" << (*it)->get_id () << "=>" << (*it)->get_processing_time () << "\n";
			}

			// Output the complete completion time of the Machine
			std::cout << "\t\t=>Machine " << machines[j]->get_id () << " completion time: " << machines[j]->get_completion_time () << "\n";

			delete vecptr;
		}

		std::cout << "\tThe current target solution value is " << query_lowest_completion_time () << "\n";
	}

}

void Problem::store_current_solution () {
	// Checks if the current solution is valid
	if (check_validity ()) {
		// Stores the current solution to temporary_storage
		std::cout << "\nThe solution is valid and will be temporarily stored.\n";
		temporary_storage = new TemporaryStorage (query_lowest_completion_time (), processes_quantity, machines_quantity, machines);
	}
	else {
		std::cout << "\nERROR: The solution was not valid and will therefore not be stored.\n";
	}
}

void Problem::save_instance (std::string *filename) {
	if (filename == nullptr) {
		// Get the name of the file to write to
		filename = new std::string ("problem.pbl");
		std::cout << "\nPlease enter the file name where the problem's instance shall be saved (default: 'problem.pbl'): ";
		std::cin >> *filename;
		std::cout << "The problem will be saved in " << *filename << "\n";
	}
	
	// Store the Problem's data in the file
	std::ofstream output_file_stream (*filename);
	output_file_stream << "# Machines\n" << machines_quantity << "\n# Processes\n" << processes_quantity << "\n# Processing times upper bound\n" << process_interval << "\n";

	// Store the Process durations
	output_file_stream << "# Process durations\n";
	for (unsigned short int i = 0; i < processes_quantity; i++) {
		if (i > 0)
			output_file_stream << ";";
		output_file_stream << processes[i]->get_processing_time ();
	}

	// Check if the current solution is valid and store Machine assignemts if so
	if (check_validity ()) {
		output_file_stream << "\n# Machine assignments\n";
		for (unsigned short int j = 0; j < machines_quantity; j++) {
			std::vector <Process*> *vecptr = nullptr;
			output_file_stream << "# Machine " << machines[j]->get_id () << "\n";
			vecptr = machines[j]->get_processes ();
			for (std::vector<Process*>::iterator it = vecptr->begin (); it != vecptr->end (); ++it) {
				if (it != vecptr->begin ())
					output_file_stream << ";";
				output_file_stream << (*it)->get_id ();
			}
			output_file_stream << "\n";
		}
	}
	else
		output_file_stream << "\n";
	output_file_stream.close ();
}
