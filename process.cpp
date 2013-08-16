#include "process.hpp"

Process::Process (unsigned short int pid, unsigned int time):
	Object (pid),
	processing_time (time)
{
	std::cout << "\tCreated process " << id << " with a duration of " << processing_time << "\n";
}
