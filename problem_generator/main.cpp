#include <chrono>
#include <fstream>
#include <iostream>
#include <random>

enum DISTRIBUTIONS {EXPONENTIAL, NORMAL, UNIFORM};

#include "process_creator.cpp"

int main (int argc, char *argv[]) {
	std::cout << "<----- PC_min problem generator ----->\n";

	unsigned short int choice = 0, processes = 0;
	unsigned int interval = 0;

	// Get the base problem parameters
	std::cout << "Enter the quantity of processes: ";
	std::cin >> processes;
	std::cout << "Enter the upper bound of the process duration interval: ";
	std::cin >> interval;

	// Get the filename to write to
	std::string *filename = nullptr;
	filename = new std::string ();
	std::cout << "\nPlease enter the file name where the problem's instance shall be saved: ";
	std::cin >> *filename;
	std::cout << "\nThe generated problem will be saved in " << *filename << "\n";

	// Ask for the distribution to use
	bool valid_choice = false;
	while (!valid_choice) {
		std::cout << "\nEnter the distribution to use:\n\t(1) Exponential distribution (lambda = 2.0)\n\t(2) Normal distribution\n\t(3) Uniform distribution\n\t=> ";
		std::cin >> choice;
	}

	ProcessCreator creator (UNIFORM, processes, interval);

	// Store the problem's base data in the file
	std::ofstream output_file_stream (*filename);
	output_file_stream << "\n# Processes\n" << processes << "\n# Processing times upper bound\n" << interval << "\n";

	std::cout << "\nFinished\n";
}
