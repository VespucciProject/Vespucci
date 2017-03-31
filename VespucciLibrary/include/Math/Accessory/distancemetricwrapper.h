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
#ifndef DISTANCEMETRICWRAPPER_H
#define DISTANCEMETRICWRAPPER_H
#include "libvespucci.h"
#include <mlpack/core.hpp>

namespace Vespucci{
    namespace Math{
        class VESPUCCI_EXPORT DistanceMetricWrapper
        {
            public:
                DistanceMetricWrapper(std::string metric_description);
                double Evaluate(arma::vec &first, arma::vec &second);
            private:
                std::string metric_description_;
                mlpack::metric::ChebyshevDistance chebyshev_;
                mlpack::metric::EuclideanDistance euclidean_;
                mlpack::metric::ManhattanDistance manhattan_;
                mlpack::metric::SquaredEuclideanDistance squared_euclidean_;
        };
    }
}


#endif // DISTANCEMETRICWRAPPER_H
