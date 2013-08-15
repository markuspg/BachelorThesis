#include "machine.hpp"

Machine::Machine (unsigned short int mid, unsigned short int processes):
	changed (false),
	Object (mid),
	machine_completion_time (0)
{
	v_processes.reserve (processes);
}

void Machine::assign_process_to_machine (Process *process) {
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

unsigned int Machine::get_completion_time () {
	if (changed == false) {
		return machine_completion_time;
	}
	else {
		compute_completion_time ();
		return machine_completion_time;
	}
}

std::vector<Process*> *Machine::get_processes () {
	std::vector<Process*> *vecptr = new std::vector<Process*> (v_processes);

	return vecptr;
}
