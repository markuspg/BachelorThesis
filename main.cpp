#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <list>
#include <random>
#include <sstream>
#include <vector>

#include <boost/numeric/conversion/converter.hpp>

// enums representing the available algorithms
enum IMPROVEMENT_ALGOS {gPAIRWISE, PAIRWISE};
enum UPPER_BOUND_ALGOS {NAIVE, SIMPLE, SIMPLE_LINEAR_TIME};
enum START_SOLUTION {iLPT, rLPT, SI, sLPT, STUPID};

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

	// Create a pointer to the problem to be solved
	Problem *problem = nullptr;

	// if (argc > 1) {
		problem = new Problem (argv[1]);
	// }
	/* else {
		unsigned short int machines = 0, processes = 0;
		unsigned int interval = 0;
		bool valid_choice = false;
		unsigned short int choice = 0;
		
		std::default_random_engine engine{};
		std::uniform_int_distribution<unsigned short int> generator{};
		std::uniform_int_distribution<unsigned int> interval_generator{};

		while (!valid_choice) {
			std::cout << "\nDo you want to enter a problem (1) or generate one by random (2)? => ";
			std::cout << "\nChoose the mode you want to run:\n\t(1) Enter a problem\n\t(2) Randomly generate a problem\n\t(3) Use default values (4;10;100)\n\t=> ";
			std::cin >> choice;
		
			switch (choice) {
				case 1:
					valid_choice = true;
					std::cout << "\nEnter the quantity of machines: ";
					std::cin >> machines;
					std::cout << "Enter the quantity of processes: ";
					std::cin >> processes;
					std::cout << "Enter the upper bound of the process duration interval: ";
					std::cin >> interval;

					problem = new Problem (machines, processes, interval);
					break;
				case 2:
					valid_choice = true;
					machines = generator (engine, std::uniform_int_distribution<unsigned short int>::param_type{2,10});
					processes = generator (engine, std::uniform_int_distribution<unsigned short int>::param_type{static_cast<unsigned short int>(machines*1.5),static_cast<unsigned short int>(machines*4)});
					interval = interval_generator (engine, std::uniform_int_distribution<unsigned int>::param_type{1,100});
					
					problem = new Problem (machines, processes, interval);
					break;
				case 3:
					valid_choice = true;
					problem = new Problem (4, 10, 100);
					break;
				default:
					std::cerr << "\nERROR: Invalid choice: Mode " << choice << " does not exist\n";
			}
		}

	}*/

	// Query the Problem's state
	problem->query_state ();

	// Creating the start solution using the Scheduler class
	Scheduler *scheduler = new Scheduler (*problem);
	scheduler->create_start_solution (sLPT);

	scheduler->query_state ();
	// scheduler->save_instance (nullptr);

	// Improve the start solution using the Improver class
	Improver *improver = new Improver (*scheduler);
	improver->improve_start_solution (PAIRWISE);
	// improver->query_state ();
}
