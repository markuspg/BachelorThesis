#include "problem.hpp"

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

	std::cout << "\nCreating a new Problem instance with the following specifications:\n\tMachines:\t\t" << machines_quantity << "\n\tProcesses:\t\t" << processes_quantity << "\n\n";

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
	for (unsigned short int j = 0; j < processes_quantity - 1; j++) {
		std::getline (input_file_stream, str, ';');
		std::istringstream (str) >> duration;
		processes[j] = new Process (j + 1, duration);
	}
	std::getline (input_file_stream, str);
	std::istringstream (str) >> duration;
	processes[processes_quantity - 1] = new Process (processes_quantity, duration);
	
	// Close the file
	input_file_stream.close();
}

Problem::Problem (const Problem &problem):
	machines_quantity (problem.get_machines_quantity ()),
	processes_quantity (problem.get_processes_quantity ())
{
	// std::cout << "\nCreating new Problem instance with the following specifications:\n\tMachines:\t\t" << machines_quantity << "\n\tProcesses:\t\t" << processes_quantity << "\n\n";

	// Create Machines
	Machine **temp_machines = nullptr;
	temp_machines = problem.get_machines_pointer ();
	machines = new Machine* [machines_quantity];
	for (unsigned short int i = 0; i < machines_quantity; i++) {
		machines[i] = new Machine (*temp_machines[i]);
	}

	// Create Processes
	Process **temp_processes = nullptr;
	temp_processes = problem.get_processes_pointer ();
	processes = new Process* [processes_quantity];
	for (unsigned short int j = 0; j < processes_quantity; j++) {
		processes[j] = new Process (*temp_processes[j]);
	}
}


Problem::~Problem () {
	for (unsigned short int i = 0; i < machines_quantity; i++) {
		delete machines[i];
	}
	delete [] machines;
	
	for (unsigned short int j = 0; j < processes_quantity; j++) {
		delete processes[j];
	}
	delete [] processes;
}

void Problem::assign_process_to_machine_by_ids (unsigned short int pid, unsigned short int mid) {
	// std::cout << "\t  Assigning process " << processes[pid - 1]->get_id () << " with a duration of " << processes[pid - 1]->get_processing_time () << " to machine " << machines[mid - 1]->get_id () << "\n";
	machines[mid - 1]->assign_process_to_machine (processes[pid - 1]);
	processes[pid - 1]->set_assigned_machines_id (mid);
}


bool Problem::check_validity () {
	for (unsigned short int j = 0; j < processes_quantity; j++) {
		// Check if every Process is assigned to a Machine
		if (processes[j]->get_assigned_machines_id () == 0) {
			// std::cout << "\t  Process " << processes[j]->get_id () << " is not yet assigned => no feasible solution.\n";
			return false;
		}
		// Check if all Processes are assigned to valid Machines
		if (processes[j]->get_assigned_machines_id () > machines_quantity) {
			// std::cout << "\t  Process " << processes[j]->get_id () << " is not assigned to a valid machine => no feasible solution.\n";
			return false;
		}
	}

	// std::cout << "\t  All Processes are assigned => feasible solution.\n";
	return true;
}

void Problem::flush () {
	// std::cout << "\nFlushing ...\n";

	// Flush the information stored in the Processes
	for (unsigned short int j = 0; j < processes_quantity; j++) {
		processes[j]->set_assigned_machines_id(0);
	}
	
	// Flush the information stored in the Machines
	for (unsigned short int i = 0; i < machines_quantity; i++) {
		machines[i]->flush ();
	}
}

void Problem::load_stored_solution () {
	// Load the data stored in temporary_storage and delete it afterwards
	// Load the assignments of the Machines
	temporary_storage->load_temporarily_stored_solution (machines, processes);
	delete temporary_storage;
	temporary_storage = nullptr;
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

unsigned short int Problem::query_lowest_workload_machines_id (unsigned short int placement, bool invert) {
	// Create a vector with all Machines which will be popped subsquently later
	std::vector<Machine*> pop_machines;
	pop_machines.reserve (machines_quantity);
	for (unsigned short int i = 0; i < machines_quantity; i++) {
		pop_machines.push_back (machines[i]);
		// std::cout << "pop_machines.push_back (Machine " << pop_machines.at (i)->get_id () << ")\n";
	}
	// Create the vector, where the popped Machines' ids will be stored
	std::vector<unsigned short int> push_machine_ids;
	push_machine_ids.reserve (machines_quantity);

	// Pop the Machine with the lowest or highest workload until 'pop_machines' is empty
	while (!pop_machines.empty ()) {
		// Take the first Machine in 'pop_machines' as start value
		unsigned int best_workload = pop_machines.at (0)->get_completion_time ();
		// std::cout << "best_workload " << best_workload << "\n";
		unsigned short int best_workload_machine = 0;
		// std::cout << "pop_machines.size () = " << pop_machines.size () << "\n";
		// Search over the complete array starting by index 1, since 0 will never become true
		for (unsigned short int i = 0; i < pop_machines.size (); i++) {
			// Search for lowest workload Machine (default: invert == false)
			if (!invert) {
				// std::cout << "Checking for lowest workload Machine.\n";
				// Check if current Machine has a lower workload than the known one
				if (pop_machines.at (i)->get_completion_time () < best_workload) {
					// std::cout << "Superior!\n";
					best_workload = pop_machines.at (i)->get_completion_time ();
					best_workload_machine = i;
				}
			}
			// Search for highest workload Machine
			else {
				// std::cout << "Checking for highest workload Machine.\n";
				// Check if current Machine has a higher workload than the known one
				if (pop_machines.at (i)->get_completion_time () > best_workload) {
					// std::cout << "Superior!\n";
					best_workload = pop_machines.at (i)->get_completion_time ();
					best_workload_machine = i;
				}
			}
		}
		// std::cout << "best_workload_machine: " << best_workload_machine << "\n";
		// All but one Machines have been assigned, assign the last one
		// std::cout << "Machine " << pop_machines.at (best_workload_machine)->get_id () << "/" << pop_machines.at (best_workload_machine)->get_completion_time () << " added \n";
        push_machine_ids.push_back (pop_machines.at (best_workload_machine)->GetId());
		pop_machines.erase (pop_machines.begin () + (best_workload_machine));
	}

	/* for (auto it = push_machine_ids.cbegin (); it != push_machine_ids.cend (); ++it) {
		std::cout << "Machine " << *it << " with workload " << machines[*it - 1]->get_completion_time () << "\n";
	} */
	return (push_machine_ids.at (placement));
}

void Problem::query_state () {
	std::cout << "\nThe problem's state:\n";

	// Output information about Processes
	std::cout << "\t" << processes_quantity << " Processes\n";
	unsigned int overall_completion_time = 0;
	for (unsigned short int j = 0; j < processes_quantity; j++) {
        std::cout << "\t  Process " << processes[j]->GetId() << ",\tduration: " << processes[j]->get_processing_time () << "\n";
		overall_completion_time += processes[j]->get_processing_time ();
	}
	std::cout << "\t  =>With an over all completion time of " << overall_completion_time << "\n";

	// Output information about Machines
	std::cout << "\t" << machines_quantity << " Machines\n";
	// Query the Machine's Processes after having checked, if the current solution is feasible
	if (check_validity ()) {
		for (unsigned int i = 0; i < machines_quantity; i++) {
			std::vector <Process*> *vecptr = nullptr;
			vecptr = machines[i]->get_processes_copy ();
            std::cout << "\t    Processes assigned to Machine " << machines[i]->GetId() << "\n";

			// Iterate over all assigned Processes and output the ids and processing times
			for (std::vector<Process*>::const_iterator cit = vecptr->cbegin (); cit != vecptr->cend (); ++cit) {
                std::cout << "\t\t" << (*cit)->GetId() << "=>" << (*cit)->get_processing_time () << "\n";
			}

			// Output the complete completion time of the Machine
            std::cout << "\t\t=>Machine " << machines[i]->GetId() << " completion time: " << machines[i]->get_completion_time () << "\n";

			delete vecptr;
		}

		std::cout << "\tThe current target solution value is " << query_lowest_completion_time () << "\n";
	}

}

void Problem::store_current_solution () {
	// Checks if the current solution is valid
	if (check_validity ()) {
		// Stores the current solution to temporary_storage
		// std::cout << "\nThe solution is valid and will be temporarily stored.\n";
		temporary_storage = new TemporaryStorage (query_lowest_completion_time (), machines_quantity, machines);
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
	output_file_stream << "# Machines\n" << machines_quantity << "\n# Processes\n" << processes_quantity << "\n";

	// Store the Process durations
	output_file_stream << "# Process durations\n";
	for (unsigned short int j = 0; j < processes_quantity; j++) {
		if (j > 0)
			output_file_stream << ";";
		output_file_stream << processes[j]->get_processing_time ();
	}

	// Check if the current solution is valid and store Machine assignemts if so
	if (check_validity ()) {
		output_file_stream << "\n# Machine assignments\n";
		for (unsigned short int i = 0; i < machines_quantity; i++) {
			std::vector <Process*> *vecptr = nullptr;
            output_file_stream << "# Machine " << machines[i]->GetId() << "\n";
			vecptr = machines[i]->get_processes_copy ();
			for (std::vector<Process*>::const_iterator cit = vecptr->cbegin (); cit != vecptr->cend (); ++cit) {
				if (cit != vecptr->cbegin ())
					output_file_stream << ";";
                output_file_stream << (*cit)->GetId();
			}
			delete vecptr;
			output_file_stream << "\n";
		}
	}
	else
		output_file_stream << "\n";
	output_file_stream.close ();
}
