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
#include "Math/Accessory/distancemetricwrapper.h"

///
/// \brief Vespucci::Math::DistanceMetricWrapper::DistanceMetricWrapper
/// \param metric_description
/// Will throw invalid_argument for unsupported metric_description
Vespucci::Math::DistanceMetricWrapper::DistanceMetricWrapper(std::string metric_description)
    : metric_description_(metric_description)
{
    std::set<std::string> valid_types = {"euclidean",
                                         "squaredeuclidean",
                                         "manhattan",
                                         "chebyshev",
                                         "cosine",
                                         "correlation"};
    if (!valid_types.count(metric_description))
        throw std::invalid_argument("Unsupported metric type " + metric_description);
}

///
/// \brief Vespucci::Math::DistanceMetricWrapper::Evaluate
/// \param first
/// \param second
/// \return
/// Evaluate the distance between first and second
/// Throws std::runtime_error for invalid type. This should be impossible because
/// the constructor should have thrown an exception...
double Vespucci::Math::DistanceMetricWrapper::Evaluate(arma::vec &first, arma::vec &second)
{
    if (metric_description_ == "squaredeuclidean")
        return squared_euclidean_.Evaluate(first, second);
    if (metric_description_ == "manhattan")
        return manhattan_.Evaluate(first, second);
    if (metric_description_ == "chebyshev")
        return chebyshev_.Evaluate(first, second);
    if (metric_description_ == "cosine")
        return 1 - arma::norm_dot(first, second);
    if (metric_description_ == "correlation")
        return 1 - arma::as_scalar(arma::cor(first, second));
    if (metric_description_ == "euclidean")
        return euclidean_.Evaluate(first, second);
    throw std::runtime_error("Unsupported metric type");
}
