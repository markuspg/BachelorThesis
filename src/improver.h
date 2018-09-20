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

#ifndef IMPROVER_H
#define IMPROVER_H

#include "problem.h"

namespace bct {

/*! A class to improve given starting solutions */
class Improver : public Problem {
public:
    enum class EImprovementAlgos{
        gPAIRWISE,
        PAIRWISE,
    };

    Improver(const Problem &argroblem);
    void Apply_PAIRWISE_algorithm(const bool argGreedy = false);
    void ImproveStartSolution(const EImprovementAlgos argAlgo,
                              const unsigned short argIterations = 50);
};

} // namespace bct

#endif // IMPROVER_H
