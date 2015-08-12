/*******************************************************************************
    Copyright (C) 2015 Wright State University - All Rights Reserved
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
/// \brief Vespucci::Math::Quantification::IntegratePeak
/// \param X
/// \param min_index
/// \param max_index
/// \param abscissa_step
/// \param baseline
/// \return
/// Takes a Riemann sum under a peak defined by certain indices
double Vespucci::Math::Quantification::IntegratePeak(const arma::vec &X, arma::uword min_index, arma::uword max_index, double abscissa_step, arma::vec &baseline)
{
    arma::vec region = X.subvec(min_index, max_index);

    double start = X(min_index);
    double end = X(max_index);

    baseline = arma::linspace(start, end, region.n_elem);

    double baseline_area = sum(baseline) / abscissa_step;
    double region_area = sum(region) / abscissa_step;

    return region_area - baseline_area;
}

///
/// \brief Vespucci::Math::Quantification::IntegratePeakMat
/// \param X
/// \param abscissa
/// \param min
/// \param max
/// \param baselines
/// \return
/// Finds the index of specified start and end values, then calls IntegratePeak
/// on each column of the arma::matrix
arma::vec Vespucci::Math::Quantification::IntegratePeakMat(const arma::mat &X, arma::vec abscissa, double &min, double &max, arma::mat &baselines, arma::uvec &boundaries)
{
    double delta = std::abs(abscissa(1) - abscissa(0));
    arma::uvec left_bound = find(((min-delta) <= abscissa) && (abscissa <= (min+delta)));
    arma::uvec right_bound = find(((max-delta) <= abscissa) && (abscissa <= (max+delta)));

    arma::uword min_index = left_bound(0);
    arma::uword max_index = right_bound(0);
    min = abscissa(min_index);
    max = abscissa(max_index);
    boundaries << min_index << arma::endr << max_index;
    arma::vec results(X.n_cols);
    baselines.set_size(X.col(0).subvec(min_index, max_index).n_elem, X.n_cols);
    arma::vec baseline(baselines.n_cols);
    for (arma::uword i = 0; i < X.n_cols; ++i){
        results(i) = IntegratePeak(X.col(i), min_index, max_index, delta, baseline);
        baselines.col(i) = baseline;
    }

    return results;
}

///
/// \brief Vespucci::Math::Quantification::IntegratePeaksMat
/// \param X
/// \param abscissa
/// \param first_min
/// \param first_max
/// \param second_min
/// \param second_max
/// \param first_baselines
/// \param second_baselines
/// \return
/// Performs two peak integrations
arma::mat Vespucci::Math::Quantification::IntegratePeaksMat(const arma::mat &X, arma::vec abscissa, double &first_min, double &first_max, double &second_min, double &second_max, arma::mat &first_baselines, arma::mat &second_baselines, arma::uvec &boundaries)
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

    boundaries.set_size(4);
    boundaries(0) = first_min_index;
    boundaries(1) = first_max_index;
    boundaries(2) = second_min_index;
    boundaries(3) = second_max_index;
    //The << operator doesn't seem to work on arma::uvecs...
    /*
    boundaries << first_min_index << arma::endr << first_max_index <<
                  second_min_index << arma::endr << second_max_index;
    */
    first_baselines.set_size(X.col(0).subvec(first_min_index, first_max_index).n_elem, X.n_cols);
    arma::vec first_baseline(first_baselines.n_cols);
    second_baselines.set_size(X.col(0).subvec(second_min_index, second_max_index).n_elem, X.n_cols);
    arma::vec second_baseline(second_baselines.n_cols);

    arma::mat results (X.n_cols, 2);
    for (arma::uword i = 0; i < X.n_cols; ++i){
        results(i, 0) = IntegratePeak(X.col(i), first_min_index, first_max_index, delta, first_baseline);
        first_baselines.col(i) = first_baseline;
    }
    for (arma::uword i = 0; i < X.n_cols; ++i){
        results(i, 1) = IntegratePeak(X.col(i), second_min_index, second_max_index, delta, second_baseline);
        second_baselines.col(i) = second_baseline;
    }

    return results;
}
