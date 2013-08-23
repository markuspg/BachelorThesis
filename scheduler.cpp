#include "scheduler.hpp"

Scheduler::Scheduler (const Problem &problem):
	Problem (problem)
{
}

void Scheduler::apply_iLPT_algorithm () {
	std::cout << "\nApplying intelligent LPT algorihm\n";
	for (unsigned short int i = 0; i < processes_quantity; i++) {
		unsigned short int lowest_workload_machine = query_lowest_workload_machines_id ();
		std::cout << "\tMachine with lowest workload: " << machines[lowest_workload_machine - 1]->get_id ();
		machines[lowest_workload_machine - 1]->assign_process_to_machine (processes[i]);
		processes[i]->set_assigned_machines_id (machines[lowest_workload_machine - 1]->get_id ());
		std::cout << "\n\t  Assigning process " << processes[i]->get_id () << " with a duration of " << processes[i]->get_processing_time () << " to machine " << machines[lowest_workload_machine - 1]->get_id () << "\n";
	}
}

void Scheduler::apply_rLPT_algorithm (unsigned short int iterations) {
	std::cout << "\nApplying randomized LPT algorihm\n";
}

void Scheduler::apply_sLPT_algorithm () {
	std::cout << "\nApplying simple LPT algorihm\n";
	for (unsigned int i = 0, j = 0; i < processes_quantity; i++, j++) {
		machines[j]->assign_process_to_machine (processes[i]);
		std::cout << "Assigning process " << processes[i]->get_id () << " with a duration of " << processes[i]->get_processing_time () << " to machine " << machines[j]->get_id () << "\n";
		processes[i]->set_assigned_machines_id (machines[j]->get_id ());
		if (j == (machines_quantity - 1))
			j = -1;
	}
}

void Scheduler::apply_STUPID_algorithm () {
	std::cout << "\nApplying stupid algorithm\n";
	for (unsigned short int i = 0; i < processes_quantity; i++) {
		machines[0]->assign_process_to_machine (processes[i]);
		std::cout << "\tAssigning process " << processes[i]->get_id () << " with a duration of " << processes[i]->get_processing_time () << " to machine " << machines[0]->get_id () << "\n";
		processes[i]->set_assigned_machines_id (machines[0]->get_id ());
	}
}

void Scheduler::create_start_solution (unsigned int choice, unsigned short int iterations) {
	switch (choice) {
		case STUPID:
			apply_STUPID_algorithm ();
			break;
		case iLPT:
			apply_iLPT_algorithm ();
			break;
		case rLPT:
			apply_rLPT_algorithm (iterations);
			break;
		case sLPT:
			apply_sLPT_algorithm ();
			break;
		default:
			std::cerr << "ERROR: Invalid start solution algorithm\n";
	}
}
