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
	std::ofstream timings_stream;
	output_file_stream.open ("start_solution_tester-results.csv", std::ofstream::out | std::ofstream::trunc);
	timings_stream.open ("start_solution_tester-timings.csv", std::ofstream::out | std::ofstream::trunc);

	for (int t = 1; t < argc; t++) {

		std::string long_filename (argv[t]);
		std::string short_filename, result;
		unsigned int found = long_filename.rfind('/');
		if (found != std::string::npos)
			short_filename = long_filename.substr (found + 1);
		else
			short_filename = long_filename;

		output_file_stream << short_filename << ",";
		timings_stream << short_filename << ",";

		// Create a pointer to the problem to be solved
		Problem *problem = nullptr;
		problem = new Problem (long_filename);

		Scheduler *scheduler = new Scheduler (*problem);
		std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();
		scheduler->create_start_solution (sLPT);
		std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();
		output_file_stream << std::to_string (scheduler->query_lowest_completion_time ()) << ",";
		std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
		timings_stream << time_span.count() << ",";
		delete scheduler;

		scheduler = new Scheduler (*problem);
		t1 = std::chrono::steady_clock::now();
		scheduler->create_start_solution (iLPT);
		t2 = std::chrono::steady_clock::now();
		output_file_stream << std::to_string (scheduler->query_lowest_completion_time ()) << ",";
		time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
		timings_stream << time_span.count() << ",";
		delete scheduler;

		scheduler = new Scheduler (*problem);
		t1 = std::chrono::steady_clock::now();
		scheduler->create_start_solution (rLPT);
		t2 = std::chrono::steady_clock::now();
		output_file_stream << std::to_string (scheduler->query_lowest_completion_time ()) << ",";
		time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
		timings_stream << time_span.count() << ",";
		delete scheduler;

		scheduler = new Scheduler (*problem);
		t1 = std::chrono::steady_clock::now();
		scheduler->create_start_solution (SI);
		t2 = std::chrono::steady_clock::now();
		output_file_stream << std::to_string (scheduler->query_lowest_completion_time ());
		time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
		timings_stream << time_span.count();
		delete scheduler;

		output_file_stream << "\n";
		timings_stream << "\n";

		delete problem;
	}

	output_file_stream.close ();
	timings_stream.close ();
}
