#include "problem.hpp"

bool compare (unsigned int i, unsigned int j) {
	return (i > j);
}

Problem::Problem (unsigned short int m_machines, unsigned short int n_processes, unsigned int workload_interval):
	machines (nullptr),
	machines_quantity (m_machines),
	processes (nullptr),
	processes_quantity (n_processes),
	process_interval (workload_interval)
{
	std::cout << "\nCreating new problem instance with the following specifications:\n\tMachines:\t\t" << machines_quantity << "\n\tProcesses:\t\t" << processes_quantity << "\n\tUpper interval bound:\t" << process_interval << "\n\n";

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

Problem::Problem (std::string filename) {
	std::cout << "\nThe problem saved in " << filename << " will be loaded.\n";

	// Open a saved problem as input
	std::ifstream input_file_stream (filename);

	// Read the lines, store them as str and convert it into the variables
	std::string str;
	std::getline (input_file_stream, str, ';');
	std::istringstream (str) >> machines_quantity;
	std::getline (input_file_stream, str, ';');
	std::istringstream (str) >> processes_quantity;
	std::getline (input_file_stream, str, ';');
	std::istringstream (str) >> process_interval;

	std::cout << "\nCreating a new problem instance with the following specifications:\n\tMachines:\t\t" << machines_quantity << "\n\tProcesses:\t\t" << processes_quantity << "\n\tUpper interval bound:\t" << process_interval << "\n\n";

	// Create Machines
	machines = new Machine* [machines_quantity];
	for (unsigned short int i = 0; i < machines_quantity; i++) {
		machines[i] = new Machine (i + 1, processes_quantity);
	}

	// Create Processes
	// Add the vector's elements to a new Process array
	processes = new Process* [processes_quantity];
	unsigned int duration = 0;
	for (unsigned short int i = 0; i < processes_quantity; i++) {
		duration = 0;
		std::getline (input_file_stream, str, ';');
		std::istringstream (str) >> duration;
		processes[i] = new Process (i + 1, duration);
	}
	
	// Close the file
	input_file_stream.close();
}

Problem::Problem (const Problem &problem):
	machines_quantity (problem.get_machines_quantity ()),
	processes_quantity (problem.get_processes_quantity ()),
	process_interval (problem.get_process_interval ())
{
	machines = problem.get_machines_pointer ();
	processes = problem.get_processes_pointer ();
}


Problem::~Problem () {
	for (unsigned short int i = 0; i < machines_quantity; i++) {
		delete machines[i];
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

void Problem::query_state () {
	std::cout << "\nThe problem's state:\n";

	// Output information about Processes
	std::cout << "\t" << processes_quantity << " processes\n";
	unsigned int overall_completion_time = 0;
	for (unsigned short int i = 0; i < processes_quantity; i++) {
		std::cout << "\t  Process " << processes[i]->get_id () << ", duration: " << processes[i]->get_processing_time () << "\n";
		overall_completion_time += processes[i]->get_processing_time ();
	}
	std::cout << "\t  =>With an over all completion time of " << overall_completion_time << "\n";

	// Output information about Machines
	std::cout << "\t" << machines_quantity << " machines\n";

	// Output information about the process interval
	std::cout << "\t" << process_interval << " interval span\n";
}

unsigned short int Problem::query_lowest_workload_machine () {
	unsigned short int lowest_workload_machine = 0;
	unsigned int lowest_workload = 0;
	lowest_workload = machines[0]->get_completion_time ();
	for (unsigned short int i = 1; i < machines_quantity; i++) {
		if (machines[i]->get_completion_time () < lowest_workload) {
			lowest_workload = machines[i]->get_completion_time ();
			lowest_workload_machine = i;

		}
	}

	return lowest_workload_machine;
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
	output_file_stream << machines_quantity << ";" << processes_quantity << ";" << process_interval << ";";
	for (unsigned short int i = 0; i < processes_quantity; i++) {
		output_file_stream << processes[i]->get_processing_time () << ";";
	}
	output_file_stream << "\n";
	output_file_stream.close ();
}
