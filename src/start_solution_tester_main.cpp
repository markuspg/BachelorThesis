/*
 * Copyright 2013, 2018 Markus Prasser
 *
 * This file is part of bct.
 *
 *  bct is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  bct is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with bct.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "scheduler.h"

#include <chrono>
#include <fstream>
#include <iostream>

int main(int argc, char *argv[]) {
  std::cout << "<----- PC_min solver ----->\n";
  std::ofstream outputFileStream;
  std::ofstream timingsStream;
  outputFileStream.open("start_solution_tester-results.csv",
                        std::ofstream::out | std::ofstream::trunc);
  timingsStream.open("start_solution_tester-timings.csv",
                     std::ofstream::out | std::ofstream::trunc);

  for (int t = 1; t < argc; t++) {
    std::string longFilename(argv[t]);
    std::string shortFilename;
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
      bct::Scheduler scheduler{problem};
      auto t1 = std::chrono::steady_clock::now();
      scheduler.CreateStartSolution(bct::Scheduler::EStartSolution::sLPT);
      auto t2 = std::chrono::steady_clock::now();
      outputFileStream << std::to_string(scheduler.QueryLowestCompletionTime())
                       << ",";
      auto timeSpan =
          std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
      timingsStream << timeSpan.count() << ",";
    }

    {
      bct::Scheduler scheduler{problem};
      auto t1 = std::chrono::steady_clock::now();
      scheduler.CreateStartSolution(bct::Scheduler::EStartSolution::iLPT);
      auto t2 = std::chrono::steady_clock::now();
      outputFileStream << std::to_string(scheduler.QueryLowestCompletionTime())
                       << ",";
      auto timeSpan =
          std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
      timingsStream << timeSpan.count() << ",";
    }

    {
      bct::Scheduler scheduler{problem};
      auto t1 = std::chrono::steady_clock::now();
      scheduler.CreateStartSolution(bct::Scheduler::EStartSolution::rLPT);
      auto t2 = std::chrono::steady_clock::now();
      outputFileStream << std::to_string(scheduler.QueryLowestCompletionTime())
                       << ",";
      auto timeSpan =
          std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
      timingsStream << timeSpan.count() << ",";
    }

    {
      bct::Scheduler scheduler{problem};
      auto t1 = std::chrono::steady_clock::now();
      scheduler.CreateStartSolution(bct::Scheduler::EStartSolution::SI);
      auto t2 = std::chrono::steady_clock::now();
      outputFileStream << std::to_string(scheduler.QueryLowestCompletionTime());
      auto timeSpan =
          std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
      timingsStream << timeSpan.count();
    }

    outputFileStream << "\n";
    timingsStream << "\n";
  }
}
