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

#include <Math/Quantification/quantification.h>

///
/// \brief Vespucci::Math::Quantification::FindPeakMax
/// \param X
/// \param min_index
/// \param max_index
/// \param position
/// \return
/// Finds the maximum of a peak bound by min_index and max_index
double Vespucci::Math::Quantification::FindPeakMax(const arma::vec &X, arma::uword min_index, arma::uword max_index, arma::uword &position)
{
    arma::vec region = X.subvec(min_index, max_index);
    double max = region.max();
    arma::uvec positions = find(region == max);
    position = min_index + positions(0);
    return max;
}

///
/// \brief Vespucci::Math::Quantification::FindPeakMaxMat
/// \param X
/// \param abscissa
/// \param min
/// \param max
/// \param positions
/// \return
/// Iterates FindPeakMat over the columns of a arma::matrix. Finds the indices of specified
/// min and max inputs
arma::vec Vespucci::Math::Quantification::FindPeakMaxMat(const arma::mat &X, arma::vec abscissa, double &min, double &max, arma::vec &positions)
{
    double delta = std::abs(abscissa(1) - abscissa(0));
    arma::uvec left_bound = find(((min-delta) <= abscissa) && (abscissa <= (min+delta)));
    arma::uvec right_bound = find(((max-delta) <= abscissa) && (abscissa <= (max+delta)));

    arma::uword min_index = left_bound(0);
    arma::uword max_index = right_bound(0);

    min = abscissa(min_index);
    max = abscissa(max_index);

    arma::vec results(X.n_cols);
    arma::uword position;
    positions.set_size(X.n_cols);
    for (arma::uword i = 0; i < X.n_cols; ++i){
        results(i) = FindPeakMax(X.col(i), min_index, max_index, position);
        positions(i) = abscissa(position);
    }

    return results;
}

///
/// \brief Vespucci::Math::Quantification::FindPeakMaxesMat
/// \param X
/// \param abscissa
/// \param first_min
/// \param first_max
/// \param second_min
/// \param second_max
/// \param positions
/// \return
/// Finds two peaks in the manner of FindPeakMaxMat
arma::mat Vespucci::Math::Quantification::FindPeakMaxesMat(const arma::mat &X, arma::vec abscissa, double &first_min, double &first_max, double &second_min, double &second_max, arma::mat positions)
{
    double delta = std::abs(abscissa(1) - abscissa(0));
    arma::uvec first_left_bound = find(((first_min-delta) <= abscissa) && (abscissa <= (first_min+delta)));
    arma::uvec first_right_bound = find(((first_max-delta) <= abscissa) && (abscissa <= (first_max+delta)));
    arma::uvec second_left_bound = find(((second_min-delta) <= abscissa) && (abscissa <= (second_min+delta)));
    arma::uvec second_right_bound = find(((second_max-delta) <= abscissa) && (abscissa <= (second_max+delta)));

    arma::uword first_min_index = first_left_bound(0);
    arma::uword first_max_index = first_right_bound(0);
    arma::uword second_min_index = second_left_bound(0);
    arma::uword second_max_index = second_right_bound(0);

    first_min = abscissa(first_min_index);
    first_max = abscissa(first_max_index);
    second_min = abscissa(second_min_index);
    second_max = abscissa(second_max_index);

    positions.set_size(X.n_cols, 2);
    arma::mat results(X.n_cols, 2);
    arma::uword first_position;
    arma::uword second_position;
    for (arma::uword i = 0; i < X.n_cols; ++i){
        results(i, 0) = FindPeakMax(X.col(i), first_min_index, first_max_index, first_position);
        results(i, 1) = FindPeakMax(X.col(i), second_min_index, second_max_index, second_position);
        positions(i, 0) = abscissa(first_position);
        positions(i, 1) = abscissa(second_position);
    }

    return results;

}
