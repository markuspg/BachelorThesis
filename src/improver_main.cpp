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

#include "improver.h"
#include "scheduler.h"

#include <chrono>
#include <fstream>
#include <iostream>

int main(int argc, char *argv[]) {
  std::cout << "<----- PC_min solver ----->\n";
  std::ofstream outputFileStream;
  std::ofstream timingsStream;
  outputFileStream.open("improver-results.csv",
                        std::ofstream::out | std::ofstream::trunc);
  timingsStream.open("improver-timings.csv",
                     std::ofstream::out | std::ofstream::trunc);

  for (int t = 1; t < argc; t++) {

    std::string long_filename(argv[t]);
    std::string short_filename;
    auto found = long_filename.rfind('/');
    if (found != std::string::npos) {
      short_filename = long_filename.substr(found + 1);
    } else {
      short_filename = long_filename;
    }

    outputFileStream << short_filename << ",";
    timingsStream << short_filename << ",";

    // Create a pointer to the problem to be solved
    bct::Problem problem{long_filename};

    {
      bct::Scheduler scheduler{problem};
      scheduler.CreateStartSolution(bct::Scheduler::EStartSolution::sLPT);
      outputFileStream << scheduler.QueryLowestCompletionTime() << ",";

      {
        bct::Improver improver{scheduler};
        auto t1 = std::chrono::steady_clock::now();
        outputFileStream << improver.ImproveStartSolution(
                                bct::Improver::EImprovementAlgos::gPAIRWISE)
                         << ",";
        auto t2 = std::chrono::steady_clock::now();
        auto time_span =
            std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
        outputFileStream << improver.QueryLowestCompletionTime() << ",";
        timingsStream << time_span.count() << ",";
      }

      {
        bct::Improver improver{scheduler};
        auto t1 = std::chrono::steady_clock::now();
        outputFileStream << improver.ImproveStartSolution(
                                bct::Improver::EImprovementAlgos::PAIRWISE,
                                5000)
                         << ",";
        auto t2 = std::chrono::steady_clock::now();
        auto time_span =
            std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
        outputFileStream << improver.QueryLowestCompletionTime() << ",";
        timingsStream << time_span.count() << ",";
      }
    }
    {
      bct::Scheduler scheduler{problem};
      scheduler.CreateStartSolution(bct::Scheduler::EStartSolution::iLPT);
      outputFileStream << scheduler.QueryLowestCompletionTime() << ",";

      {
        bct::Improver improver{scheduler};
        auto t1 = std::chrono::steady_clock::now();
        outputFileStream << improver.ImproveStartSolution(
                                bct::Improver::EImprovementAlgos::gPAIRWISE)
                         << ",";
        auto t2 = std::chrono::steady_clock::now();
        auto time_span =
            std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
        outputFileStream << improver.QueryLowestCompletionTime() << ",";
        timingsStream << time_span.count() << ",";
      }

      {
        bct::Improver improver{scheduler};
        auto t1 = std::chrono::steady_clock::now();
        outputFileStream << improver.ImproveStartSolution(
                                bct::Improver::EImprovementAlgos::PAIRWISE,
                                5000)
                         << ",";
        auto t2 = std::chrono::steady_clock::now();
        auto time_span =
            std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
        outputFileStream << improver.QueryLowestCompletionTime() << ",";
        timingsStream << time_span.count() << ",";
      }
    }

    outputFileStream << "\n";
    timingsStream << "\n";
  }
}
