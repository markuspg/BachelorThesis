#include "bound_calculator.h"

#include <chrono>
#include <fstream>
#include <iostream>

int main(int argc, char *argv[]) {
  std::cout << "<----- PC_min solver ----->\n";
  std::ofstream outputFileStream;
  std::ofstream timingsStream;
  outputFileStream.open("bound_calculator-results.csv",
                        std::ofstream::out | std::ofstream::trunc);
  timingsStream.open("bound_calculator-timings.csv",
                     std::ofstream::out | std::ofstream::trunc);

  for (int t = 1; t < argc; t++) {
    std::string longFilename{argv[t]};
    std::string shortFilename;
    std::string result;
    auto found = longFilename.rfind('/');
    if (found != std::string::npos) {
      shortFilename = longFilename.substr(found + 1);
    } else {
      shortFilename = longFilename;
    }

    outputFileStream << shortFilename << ",";
    timingsStream << shortFilename << ",";

    // Create a pointer to the problem to be solved
    bct::Problem problem{longFilename};

    {
      bct::BoundCalc boundCalculator{problem};
      auto t1 = std::chrono::steady_clock::now();
      outputFileStream << std::to_string(boundCalculator.ComputeUpperBound(
                              bct::BoundCalc::EUpperBoundAlgos::NAIVE))
                       << ",";
      auto t2 = std::chrono::steady_clock::now();
      auto time_span =
          std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
      timingsStream << time_span.count() << ",";
    }

    {
      bct::BoundCalc boundCalculator{problem};
      auto t1 = std::chrono::steady_clock::now();
      outputFileStream << std::to_string(boundCalculator.ComputeUpperBound(
                              bct::BoundCalc::EUpperBoundAlgos::SIMPLE))
                       << ",";
      auto t2 = std::chrono::steady_clock::now();
      auto time_span =
          std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
      timingsStream << time_span.count() << ",";
    }

    {
      bct::BoundCalc boundCalculator{problem};
      auto t1 = std::chrono::steady_clock::now();
      outputFileStream
          << std::to_string(boundCalculator.ComputeUpperBound(
                 bct::BoundCalc::EUpperBoundAlgos::SIMPLE_LINEAR_TIME))
          << ",";
      auto t2 = std::chrono::steady_clock::now();
      auto time_span =
          std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
      timingsStream << time_span.count() << ",";
    }

    {
      bct::BoundCalc boundalculator{problem};
      auto t1 = std::chrono::steady_clock::now();
      outputFileStream << std::to_string(boundalculator.ComputeUpperBound(
          bct::BoundCalc::EUpperBoundAlgos::LDM));
      auto t2 = std::chrono::steady_clock::now();
      auto time_span =
          std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
      timingsStream << time_span.count() << ",";
    }

    outputFileStream << "\n";
    timingsStream << "\n";
  }
}
