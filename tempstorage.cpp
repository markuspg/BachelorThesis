#include "tempstorage.hpp"

TemporaryStorage::TemporaryStorage (unsigned int current_solution, unsigned short int n_processes, unsigned short int m_machines, Machine **p_machines):
	machines_quantity (m_machines),
	solution_value (current_solution),
	v_processes (nullptr)
{
	std::cout << "\tCreating a temporary storage containing " << machines_quantity << " Machine's assignments.\n";
	v_processes = new std::vector<Process*> *[m_machines];
	for (unsigned short int i = 0; i < m_machines; i++) {
		v_processes[i] = p_machines[i]->get_processes ();
		std::cout << "\t  Added a vector containing " << v_processes[i]->size () << " elements representing Machine " << p_machines[i]->get_id () << ".\n";
	}
}

TemporaryStorage::~TemporaryStorage () {
	for (unsigned short int i = 0; i < machines_quantity; i++) {
		delete v_processes[i];
	}

	delete v_processes;
}

void TemporaryStorage::load_temporarily_stored_solution (Machine **p_machines) {
	std::cout << "\nLoading the temporarily stored solution.\n";

	for (unsigned short int i = 0; i < machines_quantity; i++) {
		std::cout << "\tFlushing Machine " << p_machines[i]->get_id () << ".\n";
		p_machines[i]->flush ();
		std::cout << "\tAssigning old solution vector.\n";
		p_machines[i]->set_assigned_processes_vector (*v_processes[i]);
	}
}
