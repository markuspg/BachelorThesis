#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

// enums representing the available algorithms
enum IMPROVEMENT_ALGOS {PAIRWISE};
enum UPPER_BOUND_ALGOS {NAIVE};
enum START_SOLUTION {STUPID, iLPT, rLPT, sLPT};

#include "object.cpp"
#include "process.cpp"
#include "machine.cpp"
#include "tempstorage.cpp"
#include "problem.cpp"
#include "bound_calculator.cpp"
#include "scheduler.cpp"
#include "improver.cpp"

int main (int argc, char *argv[]) {
	std::cout << "<----- PC_min solver ----->\n";

	// Initalize the random number generator
	srand (time (NULL));

	// Create a pointer to the problem to be solved
	Problem *problem = nullptr;
	// Declare a BoundCalc object to compute boundaries
	BoundCalc *boundcalc = nullptr;

	if (argc > 1) {
		problem = new Problem (argv[1]);
	}
	else {
		// Initialize the random number generator
		srand (time (NULL));

		unsigned short int machines = 0, processes = 0;
		unsigned int interval = 0;
		std::cout << "\nEnter the quantity of machines: ";
		std::cin >> machines;
		std::cout << "Enter the quantity of processes: ";
		std::cin >> processes;
		std::cout << "Enter the upper bound of the process duration interval: ";
		std::cin >> interval;

		problem = new Problem (machines, processes, interval);
		problem->save_instance (nullptr);
		boundcalc = new BoundCalc (*problem);
	}

	// Query the Problem's state
	problem->query_state ();

	// Creating the start solution using the Scheduler class
	Scheduler *scheduler = new Scheduler (*problem);
	scheduler->create_start_solution (iLPT);

	scheduler->query_state ();
	scheduler->save_instance (nullptr);

	// Improve the start solution using the Improver class
	Improver *improver = new Improver (*scheduler);
	improver->query_state ();
}
