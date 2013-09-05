#include "process_creator.hpp"

ProcessCreator::ProcessCreator (unsigned int chosen_distribution, unsigned short int process_quantity, unsigned int processing_times_interval_ub):
	distribution (chosen_distribution),
	engine (nullptr),
	interval (processing_times_interval_ub),
	processes (process_quantity),
	processing_times (nullptr)
{
	seed = std::chrono::system_clock::now().time_since_epoch().count();
	engine = new std::default_random_engine (seed);
}

unsigned int *ProcessCreator::return_processing_times () {
	switch (distribution) {
		case EXPONENTIAL: {
					  std::exponential_distribution<float> generator (2.0);
					  break;
				  }
		case NORMAL: {
				     // normal_generator = new std::normal_distribution<float> ();
				     std::normal_distribution<float> generator (100, 1.0);
				     break;
			     }
		case UNIFORM: {
				      // uniform_generator = new uniform_int_distribution<unsigned int> ();
				      std::uniform_int_distribution<unsigned int> generator (1, interval);
				      break;
			      }
		default:
			std::cerr << "\nERROR: Invalid probability distribution\n";
	}
}
