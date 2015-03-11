/*******************************************************************************
    Copyright (C) 2014 Wright State University - All Rights Reserved
    Daniel P. Foose - Author

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



#include <Math/Normalization/normalization.h>

///
/// \brief Vespucci::Math::Normalization::StandardScore
/// \param X A signal to be standardized
/// \return Standardized signal
///
arma::vec Vespucci::Math::Normalization::StandardScore(arma::vec X)
{
    arma::vec normalized = X;
    double mean = arma::mean(normalized);
    double std_dev = arma::stddev(X);
    normalized -= mean * arma::ones(normalized.n_elem);
    normalized /= std_dev;
    return normalized;
}

///
/// \brief Vespucci::Math::Normalization::StandardScore
/// \param X
/// \return A arma::matrix in which each column of X is replaced by its standard score.
///
arma::mat Vespucci::Math::Normalization::StandardScoreMat(arma::mat X)
{
    arma::mat normalized = X;
    for (arma::uword j = 0; j < normalized.n_cols; ++j)
        normalized.col(j) = StandardScore(normalized.col(j));
    return normalized;
}
