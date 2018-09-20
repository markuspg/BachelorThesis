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

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "problem.h"

namespace bct {

/*! A class to generate and represent starting solutions */
class Scheduler : public Problem {
public:
    enum class EStartSolution{
        iLPT,
        rLPT,
        SI,
        sLPT,
        STUPID,
    };

    Scheduler(const Problem &argProblem);
    void Apply_iLPT_Algorithm();
    void Apply_rLPT_Algorithm(const unsigned short argIterations);
    void Apply_SI_Algorithm();
    void Apply_sLPT_Algorithm();
    void Apply_STUPID_Algorithm();
    void CreateStartSolution(const EStartSolution argChoice,
                             const unsigned short argIterations = 50);
};

} // namespace bct

#endif // SCHEDULER_H
