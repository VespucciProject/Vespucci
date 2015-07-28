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
arma::vec Vespucci::Math::Normalization::StandardScore(const arma::vec &X)
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
arma::mat Vespucci::Math::Normalization::StandardScoreMat(const arma::mat &X)
{
    arma::mat normalized = X;
    arma::rowvec means = mean(X);
    arma::rowvec std_devs = arma::stddev(X);
    normalized.each_row() -= means;
    std_devs.transform( [](double val){return 1.0 / val;}); //probably faster than finding inverse of diagonal matrix?
    return normalized * arma::diagmat(std_devs);
}


arma::mat Vespucci::Math::Normalization::SNVNorm(const arma::mat &X, const double offset)
{
    arma::rowvec weights = arma::stddev(X, 0) + offset*arma::ones(1, X.n_cols);
    weights.transform( [](double val){return 1.0 / val;});
    return X * arma::diagmat(weights);
}
