/*******************************************************************************
    Copyright (C) 2014-2017 Wright State University - All Rights Reserved
    Daniel P. Foose - Maintainer/Lead Developer

    This file is part of Vespucci.

    Vespucci is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Vespucci is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Vespucci.  If not, see <http://www.gnu.org/licenses/>.
*******************************************************************************/
#include <mlpack/core.hpp>
#include <boost/math/distributions.hpp>
#include "Math/Stats/confidenceinterval.h"

///
/// \brief Vespucci::Math::Stats::TInterval
/// \param alpha
/// \param stddev
/// \param n
/// \return Width of the TInterval
///
double Vespucci::Math::Stats::TInterval(double alpha, double stddev, unsigned int n)
{
    using namespace boost::math;
    using namespace std;
    students_t dist(n - 1);
    double T = quantile(complement(dist, alpha/2));
    return T*stddev / sqrt(double(n));
}

///
/// \brief Vespucci::Math::Stats::ChiSquaredInterval
/// \param alpha
/// \param stddev
/// \param n
/// \return
/// First value is lower limit, second value is upper limit
std::pair<double, double> Vespucci::Math::Stats::ChiSquaredInterval(double alpha, double stddev, unsigned int n)
{
    using namespace boost::math;
    using namespace std;
    chi_squared dist(n - 1);
    double lower_limit = sqrt((n - 1) * stddev*stddev / quantile(complement(dist, alpha/2)));
    double upper_limit = sqrt((n -1 ) * stddev*stddev / quantile(dist, alpha/2));
    return pair<double, double>(lower_limit, upper_limit);
}
