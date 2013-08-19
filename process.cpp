#include "process.hpp"

Process::Process (unsigned short int pid, unsigned int time):
	Object (pid),
	assigned_machine (0),
	processing_time (time)
{
	std::cout << "\tCreated process " << id << " with a duration of " << processing_time << "\n";
}

void Process::set_assigned_machine (unsigned int mid) {
	assigned_machine = mid;
}
