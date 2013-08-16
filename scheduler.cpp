#include "scheduler.hpp"

Scheduler::Scheduler (const Problem &problem):
	Problem (problem)
{}

void Scheduler::apply_iLPT_algorithm () {
	std::cout << "Applying intelligent LPT algorihm\n";
	for (unsigned short int i = 0; i < processes_quantity; i++) {
		unsigned short int lowest_workload_machine = query_lowest_workload_machine ();
		std::cout << "Machine with lowest workload: " << machines[lowest_workload_machine]->get_id ();
		machines[lowest_workload_machine]->assign_process_to_machine (processes[i]);
		std::cout << "Assigning process " << processes[i]->get_id () << " with a duration of " << processes[i]->get_processing_time () << " to machine " << machines[lowest_workload_machine]->get_id () << "\n";
	}
}

void Scheduler::apply_sLPT_algorithm () {
	std::cout << "Applying simple LPT algorihm\n";
	for (unsigned short int i = 0, unsigned short int j = 0; i < processes_quantity; i++, j++) {
		machines[j]->assign_process_to_machine (processes[i]);
		std::cout << "Assigning process " << processes[i]->get_id () << " with a duration of " << processes[i]->get_processing_time () << " to machine " << machines[j]->get_id () << "\n";
		if (j = 7)
			j = 0;
	}
}

void Scheduler::apply_STUPID_algorithm () {
	std::cout << "Applying stupid algorithm\n";
	for (unsigned short int i = 0; i < processes_quantity; i++) {
		machines[0]->assign_process_to_machine (processes[i]);
		std::cout << "Assigning process " << processes[i]->get_id () << " with a duration of " << processes[i]->get_processing_time () << " to machine " << machines[0]->get_id () << "\n";
	}
}

void Scheduler::create_start_solution (unsigned int choice) {
	switch (choice) {
		case STUPID:
			apply_STUPID_algorithm ();
			break;
		case iLPT:
			apply_iLPT_algorithm ();
			break;
		case sLPT:
			apply_sLPT_algorithm ();
			break;
		default:
			std::cerr << "ERROR: Invalid start solution algorithm\n";
	}
}
