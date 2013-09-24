#include <algorithm>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <list>
#include <random>
#include <sstream>
#include <vector>

// enums representing the available algorithms
enum IMPROVEMENT_ALGOS {gPAIRWISE, PAIRWISE};
enum UPPER_BOUND_ALGOS {LDM, NAIVE, SIMPLE, SIMPLE_LINEAR_TIME};
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
	std::ofstream output_file_stream;
	output_file_stream.open ("bound_calculator-results.csv", std::ofstream::out | std::ofstream::trunc);

	for (int t = 1; t < argc; t++) {

		std::string long_filename (argv[t]);
		std::string short_filename, result;
		unsigned int found = long_filename.rfind('/');
		if (found != std::string::npos)
			short_filename = long_filename.substr (found + 1);
		else
			short_filename = long_filename;

		output_file_stream << short_filename << ",";

		// Create a pointer to the problem to be solved
		Problem *problem = nullptr;
		problem = new Problem (long_filename);

		delete problem;
	}

	output_file_stream.close ();
}
