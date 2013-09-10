#include "machine.hpp"

Machine::Machine (unsigned short int mid, unsigned short int processes):
	changed (false),
	Object (mid),
	machine_completion_time (0)
{
	v_processes.reserve (processes);
	std::cout << "\tCreating Machine " << mid << " able to contain " << processes << " Processes\n";
}

Machine::Machine (const Machine &machine):
	changed (true),
	Object (machine.get_id ()),
	machine_completion_time (0)
{
	std::vector<Process*> *vecptr = nullptr;
	vecptr = machine.get_processes ();
	v_processes = *vecptr;
	delete vecptr;
	compute_completion_time ();
}

void Machine::assign_process_to_machine (Process *process) {
	std::cout << "Assigning Process " << process->get_id () << " to Machine " << this->get_id () << ".\n";
	v_processes.push_back (process);
	changed = true;
}

void Machine::compute_completion_time () {
	// Reset machine completion time
	machine_completion_time = 0;

	// Recompute it
	for (std::vector<Process*>::iterator it = v_processes.begin (); it != v_processes.end (); ++it) {
		machine_completion_time += (*it)->get_processing_time ();
	}

	changed = false;
}

void Machine::delete_process_from_machine (Process *process) {
	for (auto it = v_processes.begin (); it != v_processes.end (); ++it) {
		if (*it == process) {
			std::cout << "Deleting Process " << (*it)->get_id () << "/" << process->get_id () << " from Machine " << this->get_id () << ".\n";
			v_processes.erase (it);
			break;
		}
	}
	changed = true;
}

void Machine::flush () {
	v_processes.clear ();
	machine_completion_time = 0;
	changed = true;
}

unsigned int Machine::get_completion_time () {
	if (changed == false) {
		return machine_completion_time;
	}
	else {
		compute_completion_time ();
		return machine_completion_time;
	}
}

std::vector<Process*> *Machine::get_processes () const {
	std::vector<Process*> *vecptr = new std::vector<Process*> (v_processes);

	return vecptr;
}

void Machine::set_assigned_processes_vector (std::vector<Process*> processes_vector) {
	v_processes = processes_vector;
}
