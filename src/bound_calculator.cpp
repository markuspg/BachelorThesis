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

#include "bound_calculator.h"
#include "process.h"

#include <cmath>
#include <iostream>
#include <vector>

/*!
  * \brief BoundCalc's constructor
  * \param[in] argProblem A Problem instance whose bounds shall be calculated
  */
bct::BoundCalc::BoundCalc(const Problem &argProblem) :
    Problem{argProblem}
{
    for (unsigned short j = 0; j < processesQuantity; ++j) {
        const auto processingTime = processes[j]->GetProcessingTime();
        cumulatedProcessingTimes += processingTime;
        if (processingTime > longestProcessingTime)
            longestProcessingTime = processingTime;
        if (processingTime < shortestProcessingTime)
            shortestProcessingTime = processingTime;
    }
    // std::cout << "\nCreating a new BoundCalc instance with the following"
    //              " specifications:\n\tMachines:\t\t" << machinesQuantity
    //           << "\n\tProcesses:\t\t" << processesQuantity
    //           << "\n\tCumulated processing times:\t" << cumulatedProcessingTimes
    //           << "\n\tLongest processing time:\t" << longestProcessingTime
    //           << "\n\nShortest processing time:\t:" << shortestProcessingTime
    //           << "\n\n";

}

/*!
 * \brief Applies the L_DM lower bound algorithm from Haouari and Jemmali 2008
 * \return A valid PC_max lower bound
 */
unsigned int bct::BoundCalc::ApplyLDMAlgorithm() {
    const auto existingBound = ApplySIMPLE_LINEAR_TIMEAlgorithm ();
    auto testCapacity = static_cast<unsigned int>(std::max(
                                                      static_cast<double>(existingBound
                                                                          - (processes[0]->GetProcessingTime() / 2)),
                                                  static_cast<double>(existingBound / 2)));
    // Iteratively test different bin capacities beginning with an already known lower bound
    for ( ; testCapacity < existingBound + processes[0]->GetProcessingTime();
          ++testCapacity) {
        const auto test = GetLB_BPP_BP_LowerBound(testCapacity);
        if ((test > 0) && (test  <= machinesQuantity))
            break;
    }

    return testCapacity;
}

/*!
 * \brief Applies the NAIVE algorithm to calculate an PC_max lower bound bound from Ghomi and Ghazvini 1998
 * \return The maximum of the cumulated processing times divided by the quantity of machines and the longest process
 */
unsigned int bct::BoundCalc::ApplyNAIVEAlgorithm() {
    // std::cout << "\nApplying NAIVE algorihm\n";

    // Not using the processesQuantity like in the paper, because it doesn't make any sense
    // The result can be truncated, because it should be rounded down anyway
    unsigned int averageMachineRuntime
            = ((static_cast<float>(cumulatedProcessingTimes)
                / static_cast<float>(machinesQuantity))
               > static_cast<unsigned int>(cumulatedProcessingTimes
                                           / machinesQuantity))
            ? static_cast<unsigned int>((cumulatedProcessingTimes
                                         / machinesQuantity) + 1)
            : (cumulatedProcessingTimes / machinesQuantity);

    return (averageMachineRuntime > longestProcessingTime)
            ? averageMachineRuntime : longestProcessingTime;
}

/*!
 * \brief Applies the U_0 upper bound from Haouari and Jemmali 2008
 * \return The average processing time over all machines in the case of perfect balanced distribution
 */
unsigned int bct::BoundCalc::ApplySIMPLEAlgorithm() {
    // std::cout << "\nApplying SIMPLE algorihm\n";

    return static_cast<unsigned int>(cumulatedProcessingTimes / machinesQuantity);
}

/*!
 * \brief Applies the simple linear-time lower bound algorithm of Dell'Amico and Martello 1995 (L_1 in Haouari and Jemmali)
 * \return The simple linear-time lower bound
 */
unsigned int bct::BoundCalc::ApplySIMPLE_LINEAR_TIMEAlgorithm() {
    // std::cout << "\nApplying SIMPLE_LINEAR_TIME algorithm\n";

    // Calculate the three needed quantities
    // std::cout << "Longest processing time:\t" << longestProcessingTime << "\n";
    auto pmpm1 = processes[machinesQuantity - 1]->GetProcessingTime()
            + processes[machinesQuantity]->GetProcessingTime();
    // std::cout << "pmpm1:\t\t\t\t" << pmpm1 << "\n";
    unsigned int averageRuntime
            = ((static_cast<float>(cumulatedProcessingTimes)
                / static_cast<float>(machinesQuantity))
               > static_cast<unsigned int>(cumulatedProcessingTimes
                                           / machinesQuantity))
            ? static_cast<unsigned int>((cumulatedProcessingTimes
                                         / machinesQuantity) + 1)
            : (cumulatedProcessingTimes / machinesQuantity);
    // std::cout << "Average runtime:\t\t" << averageRuntime << "\n";

    if ((longestProcessingTime >= pmpm1) && (longestProcessingTime >= averageRuntime))
        return longestProcessingTime;
    if ((pmpm1 >= longestProcessingTime) && (pmpm1 >= averageRuntime))
        return pmpm1;
    else
        return averageRuntime;
}

/*!
 * \brief Compute lower and upper bounds
 * \param[in] algo
 * \return
 */
unsigned int bct::BoundCalc::ComputeUpperBound(const EUpperBoundAlgos argAlgo) {
    switch (argAlgo) {
    case EUpperBoundAlgos::LDM:
        return ConvertPCmaxLowerBoundToPCminUpperBound (ApplyLDMAlgorithm());
    case EUpperBoundAlgos::NAIVE:
        return ConvertPCmaxLowerBoundToPCminUpperBound (ApplyNAIVEAlgorithm());
    case EUpperBoundAlgos::SIMPLE:
        return ApplySIMPLEAlgorithm();
    case EUpperBoundAlgos::SIMPLE_LINEAR_TIME:
        return ConvertPCmaxLowerBoundToPCminUpperBound (ApplySIMPLE_LINEAR_TIMEAlgorithm());
    default:
        std::cerr << "\nERROR: Invalid bound calculation algorithm\n";
    }
}

/*!
 * \brief Converts a valid PC_max lower bound to a PC_min upper bound
 * \param[in] argPCmaxLowerBound The PC_max lower bound to convert
 * \return A valid PC_min upper bound
 */
unsigned int bct::BoundCalc::ConvertPCmaxLowerBoundToPCminUpperBound(
        const unsigned int argPCmaxLowerBound) {
    return static_cast<unsigned int>((cumulatedProcessingTimes - argPCmaxLowerBound)
                                     / (machinesQuantity - 1));
}

/*!
 * \brief Returns a valid BPP lower bound LB_BPP from Haouari and Jemmali 2008
 * \param[in] argCapacity The capacity the bound shall be calculated for
 * \return A valid BPP lower bound
 */
unsigned short int bct::BoundCalc::GetLB_BPP_BP_LowerBound (const unsigned int argCapacity) {
    // std::cout << "\nComputing valid BPP lower bound for a capacity of "
    //           << argCapacity << "\n";
    // Add all valid processing times to a vector
    std::vector<unsigned int> p;
    for (unsigned int j = machinesQuantity + 1; j < processesQuantity; ++j) {
        if (processes[j]->GetProcessingTime()
                > static_cast<unsigned int>(argCapacity / 2)) {
            continue;
        }
        p.emplace_back(processes[j]->GetProcessingTime());
        // std::cout << "Added Process " << processes[j]->GetId()
        //           << " with a duration of " << processes[j]->GetProcessingTime()
        //           << "\n";
    }

    // Create the BPPx vectors
    std::vector<unsigned short> BPP_1, BPP_2;

    // Iterate over the vector and create a new vector of possible bounds
    for (auto it = p.cbegin (); it != p.cend (); ++it) {
        // std::cout << "p = " << *it << "\n";

        // Create the three sets of Processes
        std::vector<unsigned int> J_1, J_2, J_3;
        for (unsigned int k = 0; k < processesQuantity; ++k) {
            if (processes[k]->GetProcessingTime() > argCapacity - *it)
                J_1.emplace_back(processes[k]->GetProcessingTime());
            if ((static_cast<unsigned int>(argCapacity / 2)
                 < processes[k]->GetProcessingTime())
                    && (processes[k]->GetProcessingTime() <= argCapacity - *it)) {
                J_2.emplace_back(processes[k]->GetProcessingTime());
            }
            if ((*it <= processes[k]->GetProcessingTime())
                    && (processes[k]->GetProcessingTime ()
                        <= static_cast<unsigned int>(argCapacity / 2))) {
                J_3.emplace_back(processes[k]->GetProcessingTime());
            }
        }

        // Output of the three vectors
        /* std::cout << "J_1 =";
        for (auto j1it = J_1.cbegin (); j1it != J_1.cend (); ++j1it) {
            std::cout << " " << *j1it;
        }
        std::cout << "\n";
        std::cout << "J_2 =";
        for (auto j2it = J_2.cbegin (); j2it != J_2.cend (); ++j2it) {
            std::cout << " " << *j2it;
        }
        std::cout << "\n";
        std::cout << "J_3 =";
        for (auto j3it = J_3.cbegin (); j3it != J_3.cend (); ++j3it) {
            std::cout << " " << *j3it;
        }
        std::cout << "\n"; */

        // Calculate support values
        unsigned int cumulatedProcessingTimesJ2 = 0;
        unsigned int cumulatedProcessingTimesJ3 = 0;
        unsigned int J2Rest = 0;
        for (auto j2it = J_2.cbegin(); j2it != J_2.cend(); ++j2it) {
            cumulatedProcessingTimesJ2 += *j2it;
            J2Rest += static_cast<unsigned int>((argCapacity - *j2it) / *it);
        }
        // std::cout << "cumulatedProcessingTimesJ2 = "
        //           << cumulatedProcessingTimesJ2 << "\n";
        for (auto j3it = J_3.cbegin (); j3it != J_3.cend (); ++j3it) {
            cumulatedProcessingTimesJ3 += *j3it;
        }
        // std::cout << "cumulatedProcessingTimesJ3 = "
        //           << cumulatedProcessingTimesJ3 << "\n";

        // Calculate and store BPP_1 and BPP_2
        BPP_1.emplace_back(static_cast<unsigned int>(J_1.size () + J_2.size () + fmax (0, ceil (static_cast<double> (cumulatedProcessingTimesJ3 - static_cast<double> (argCapacity * J_2.size ()) + cumulatedProcessingTimesJ2) / static_cast<double> (argCapacity)))));
        BPP_2.emplace_back(static_cast<unsigned int>(J_1.size () + J_2.size () + fmax (0, ceil (static_cast<double> (J_3.size () - static_cast<double> (J2Rest)) / static_cast<double> (static_cast<unsigned int> (argCapacity / *it))))));
    }

    /* // Display the BPPs
    std::cout << "BPP_1 =";
    for (auto BPP1it = BPP_1.cbegin (); BPP1it != BPP_1.cend (); ++BPP1it) {
        std::cout << " " << *BPP1it;
    }
    std::cout << "\n";
    std::cout << "BPP_2 =";
    for (auto BPP2it = BPP_2.cbegin (); BPP2it != BPP_2.cend (); ++BPP2it) {
        std::cout << " " << *BPP2it;
    }
    std::cout << "\n"; */

    unsigned short maxMinBinRequirement = 0;
    for (auto BPP1it = BPP_1.cbegin(), BPP2it = BPP_2.cbegin();
         BPP1it != BPP_1.cend(), BPP2it != BPP_2.cend();
         ++BPP1it, ++BPP2it) {
        if (*BPP1it > maxMinBinRequirement)
            maxMinBinRequirement = *BPP1it;
        if (*BPP2it > maxMinBinRequirement)
            maxMinBinRequirement = *BPP2it;
    }

    // std::cout << "The minimum bin requirement is " << maxMinBinRequirement << "\n";

    return maxMinBinRequirement;
}
