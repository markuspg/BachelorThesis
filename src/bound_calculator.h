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

#ifndef BOUND_CALC_H
#define BOUND_CALC_H

#include "enums.h"
#include "problem.h"

namespace bct {

/*! An object used to calculate bounds */
class BoundCalc : public Problem {
public:
    BoundCalc(const Problem &argProblem);
    unsigned int ComputeUpperBound(const UPPER_BOUND_ALGOS argAlgo);
    unsigned int GetCumulatedProcessingTimes() const noexcept;

private:
    unsigned int ApplyLDMAlgorithm();
    unsigned int ApplyNAIVEAlgorithm();
    unsigned int ApplySIMPLEAlgorithm();
    unsigned int ApplySIMPLE_LINEAR_TIMEAlgorithm();
    unsigned int ConvertPCmaxLowerBoundToPCminUpperBound(
            const unsigned int argPCmaxLowerBound);
    unsigned short GetLB_BPP_BP_LowerBound(const unsigned int argCapacity);

    //! The processing time of all Processes of the Problem
    unsigned int cumulatedProcessingTimes = 0;
    //! The processing time of the longest Process of the Problem
    unsigned int longestProcessingTime = 0;
    //! The processing time of the shortest Process of the Problem
    unsigned int shortestProcessingTime = 0;
};

inline unsigned int BoundCalc::GetCumulatedProcessingTimes () const noexcept {
    return cumulatedProcessingTimes;
}

} // namespace bct

#endif // BOUND_CALC_H
