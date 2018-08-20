#include "process.hpp"

Process::Process (unsigned short int pid, unsigned int time):
	Object (pid),
	assigned_machines_id (0),
	processing_time (time)
{
	// std::cout << "\tCreated Process " << id << " with a duration of " << processing_time << "\n";
}

Process::Process (const Process &process):
    Object{process.GetId()},
	assigned_machines_id (process.get_assigned_machines_id ()),
	processing_time (process.get_processing_time ())
{
	// std::cout << "\tCreated Process " << id << " with a duration of " << processing_time << "\n";
}

void Process::set_assigned_machines_id (unsigned int mid) {
	assigned_machines_id = mid;
}
