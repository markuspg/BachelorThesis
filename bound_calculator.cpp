#include "bound_calculator.hpp"

BoundCalc::BoundCalc (const Problem &problem):
	Problem (problem)
{
	std::cout << "\nCreating a new BoundCalc instance with the following specifications:\n\tMachines:\t\t" << machines_quantity << "\n\tProcesses:\t\t" << processes_quantity << "\n\tUpper interval bound:\t" << process_interval << "\n\n";
}
