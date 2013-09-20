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

	// Create a pointer to the problem to be solved
	Problem *problem = nullptr;
	problem = new Problem (argv[1]);
}
