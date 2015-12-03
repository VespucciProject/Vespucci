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
#include "Math/Quantification/integration.h"
#include "Math/Accessory/accessory.h"
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
arma::vec Vespucci::Math::Quantification::IntegratePeakMat(const arma::mat &X, const arma::vec abscissa, double &min, double &max, arma::field<arma::mat> &baselines, arma::uvec &boundaries)
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
    baselines.set_size(X.n_cols);
    arma::mat current_baseline;
    arma::vec baseline(abscissa.rows(min_index, max_index).n_rows);
    for (arma::uword i = 0; i < X.n_cols; ++i){
        results(i) = IntegratePeak(X.col(i), min_index, max_index, delta, baseline);
        current_baseline = arma::join_horiz(abscissa.rows(min_index, max_index),
                                            baseline);
        baselines(i) = current_baseline;
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
arma::mat Vespucci::Math::Quantification::IntegratePeaksMat(const arma::mat &X, const arma::vec &abscissa, double &first_min, double &first_max, double &second_min, double &second_max, arma::field<arma::mat> &first_baselines, arma::field<arma::mat> &second_baselines, arma::uvec &boundaries)
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

    arma::mat current_first_baseline;
    arma::mat current_second_baseline;

    first_baselines.set_size(X.n_cols);
    arma::vec first_baseline(abscissa.rows(first_min_index, first_max_index).n_rows);
    second_baselines.set_size(X.n_cols);
    arma::vec second_baseline(abscissa.rows(second_min_index, second_max_index).n_rows);

    arma::mat results (X.n_cols, 2);
    for (arma::uword i = 0; i < X.n_cols; ++i){
        results(i, 0) = IntegratePeak(X.col(i), first_min_index, first_max_index, delta, first_baseline);
        current_first_baseline = arma::join_horiz(abscissa.rows(first_min_index, first_max_index),
                                                  first_baseline);
        first_baselines(i) = current_first_baseline;
    }
    for (arma::uword i = 0; i < X.n_cols; ++i){
        results(i, 1) = IntegratePeak(X.col(i), second_min_index, second_max_index, delta, second_baseline);
        current_second_baseline = arma::join_horiz(abscissa.rows(second_min_index, second_max_index),
                                                   second_baseline);
        second_baselines(i) = current_second_baseline;
    }
    return results;
}

///
/// \brief Vespucci::Math::Quantification::IntegratePeakMat
/// \param X
/// \param abscissa
/// \param min
/// \param max
/// \param baselines
/// \param boundaries
/// \param bound_window
/// \return
///  Performs single peak Riemann sum integration. If find_boundaries is true, then will estimate the actual boundaries
arma::vec Vespucci::Math::Quantification::IntegratePeakMat(const arma::mat &X, const arma::vec &abscissa,
                                                           double &min, double &max,
                                                           arma::field<arma::mat> &baselines,
                                                           arma::uvec &boundaries, arma::uword bound_window)
{
    arma::vec results(X.n_cols);
    double delta = std::abs(abscissa(1) - abscissa(0)); //assumes monotonic to some degree of precision
    arma::uvec left_bound = find(((min-delta) <= abscissa) && (abscissa <= (min+delta)));
    arma::uvec right_bound = find(((max-delta) <= abscissa) && (abscissa <= (max+delta)));

    //initial centers
    arma::uword min_index = left_bound(0);
    arma::uword max_index = right_bound(0);
    min = abscissa(min_index);
    max = abscissa(max_index);

    arma::uword min_start = min_index - bound_window;
    arma::uword min_end = min_index + bound_window;
    min_end = (min_end >= abscissa.n_rows ? abscissa.n_rows - 1 : min_end);

    arma::uword max_start = max_index - bound_window;
    arma::uword max_end = max_index + bound_window;
    max_end = (max_end >= abscissa.n_rows ? abscissa.n_rows - 1 : max_end);
    arma::mat current_baseline;
    baselines.set_size(X.n_cols);
    for (arma::uword i = 0; i < X.n_cols; ++i){
        arma::vec spectrum = X.col(i);
        arma::vec min_window = spectrum.rows(min_start, min_end);
        arma::vec max_window = spectrum.rows(max_start, max_end);
        min_index = Vespucci::Math::LocalMinimum(min_window, min);
        max_index = Vespucci::Math::LocalMinimum(max_window, max);
        arma::vec baseline(spectrum.rows(min_index, max_index).n_rows);
        results(i) = IntegratePeak(spectrum, min_index, max_index, delta, baseline);
        current_baseline = arma::join_horiz(abscissa.rows(min_index, max_index),
                                            baseline);
        baselines(i) = current_baseline;
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
/// \param boundaries
/// \param bound_window
/// \return
///
arma::mat Vespucci::Math::Quantification::IntegratePeaksMat(const arma::mat &X, const arma::vec &abscissa,
                                                            double &first_min, double &first_max,
                                                            double &second_min, double &second_max,
                                                            arma::field<arma::mat> &first_baselines, arma::field<arma::mat> &second_baselines,
                                                            arma::uvec &boundaries, arma::uword bound_window)
{
    arma::mat results(X.n_cols, 2);
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

    arma::uword first_min_start = first_min_index - bound_window;
    arma::uword first_min_end = first_min_index + bound_window;
    first_min_end = (first_min_end >= abscissa.n_rows ? abscissa.n_rows - 1 : first_min_end);
    arma::uword second_min_start = second_min_index - bound_window;
    arma::uword second_min_end = second_min_index + bound_window;
    second_min_end = (second_min_end >= abscissa.n_rows ? abscissa.n_rows - 1 : second_min_end);

    arma::uword first_max_start = first_max_index - bound_window;
    arma::uword first_max_end = first_max_index + bound_window;
    first_max_end = (first_max_end >= abscissa.n_rows ? abscissa.n_rows - 1 : first_max_end);
    arma::uword second_max_start = second_max_index - bound_window;
    arma::uword second_max_end = second_max_index + bound_window;
    second_max_end = (second_max_end >= abscissa.n_rows ? abscissa.n_rows - 1 : second_max_end);

    first_baselines.set_size(X.n_cols);
    second_baselines.set_size(X.n_cols);

    arma::mat current_first_baseline;
    arma::mat current_second_baseline;
    for (arma::uword i = 0; i < X.n_cols; ++i){
        arma::vec spectrum = X.col(i);

        arma::vec first_min_window = spectrum.rows(first_min_start, first_min_end);
        arma::vec first_max_window = spectrum.rows(first_max_start, first_max_end);
        first_min_index = Vespucci::Math::LocalMinimum(first_min_window, first_min);
        first_max_index = Vespucci::Math::LocalMinimum(first_max_window, first_max);
        arma::vec first_baseline(spectrum.rows(first_min_index, first_max_index).n_rows);
        results(i, 0) = IntegratePeak(spectrum, first_min_index, first_max_index, delta, first_baseline);
        current_first_baseline = arma::join_horiz(abscissa.rows(first_min_index, first_max_index),
                                                  first_baseline);
        first_baselines(i) = current_first_baseline;

        arma::vec second_min_window = spectrum.rows(second_min_start, second_min_end);
        arma::vec second_max_window = spectrum.rows(second_max_start, second_max_end);
        second_min_index = Vespucci::Math::LocalMinimum(second_min_window, second_min);
        second_max_index = Vespucci::Math::LocalMinimum(second_max_window, second_max);
        arma::vec second_baseline(spectrum.rows(second_min_index, second_max_index).n_rows);
        results(i, 1) = IntegratePeak(spectrum, second_min_index, second_max_index, delta, second_baseline);
        current_second_baseline = arma::join_horiz(abscissa.rows(second_min_index, second_max_index),
                                                   second_baseline);
        second_baselines(i) = current_second_baseline;
    }

    boundaries.set_size(4);
    boundaries(0) = first_min_index;
    boundaries(1) = first_max_index;
    boundaries(2) = second_min_index;
    boundaries(3) = second_max_index;

    return results;
}
