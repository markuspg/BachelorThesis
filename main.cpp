#include <algorithm>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <list>
#include <random>
#include <sstream>
#include <vector>

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

	for (int t = 1; t < argc; t++) {
		// Create a pointer to the problem to be solved
		Problem *problem = nullptr;
		problem = new Problem (argv[1]);

		delete problem;
	}
}
