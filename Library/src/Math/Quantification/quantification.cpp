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
/// \brief Vespucci::Math::Quantification::QuantifyPeak
/// \param spectrum
/// \param abscissa
/// \param min
/// \param max
/// \param bound_window
/// \param baseline
/// \return  row vector with following quantities:
/// Peak Center
/// Intensity
/// Corrected Intensity
/// Riemann Sum
/// Corrected Riemann Sum
/// Riemann Sum between inflection points
/// Corrected Riemann Sum between inflextion points
/// Empirical Full-width at half maximum.
///
arma::rowvec Vespucci::Math::Quantification::QuantifyPeak(const arma::vec &spectrum, const arma::vec &abscissa, double &min, double &max, arma::uword bound_window, arma::vec &total_baseline, arma::vec &inflection_baseline)
{
    //performs analysis of peak shape and magnitude
    arma::rowvec results(8);
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


    double total_area = arma::sum(spectrum.rows(min_index, max_index));
    total_area = total_area / (abscissa(max_index) - abscissa(min_index));

    arma::uword window_size = max_index - min_index + 1;
    total_baseline = arma::linspace(spectrum(min_index), spectrum(max_index), window_size);
    double baseline_area = arma::sum(total_baseline) / (abscissa(max_index) - abscissa(min_index));

    arma::vec min_window = spectrum.rows(min_start, min_end);
    arma::vec max_window = spectrum.rows(max_start, max_end);
    min_index = Vespucci::Math::LocalMinimum(min_window, min);
    max_index = Vespucci::Math::LocalMinimum(max_window, max);

    double area_between_inflection = arma::sum(spectrum.rows(min_index, max_index));
    area_between_inflection = area_between_inflection / (abscissa(max_index) - abscissa(min_index));
    window_size = max_index - min_index + 1;
    inflection_baseline = arma::linspace(spectrum(min_index), spectrum(max_index), window_size);
    double inf_baseline_area = arma::sum(inflection_baseline) / (abscissa(min_index) - abscissa(max_index));


    double maximum = spectrum.rows(min_index, max_index).max();

    arma::uvec max_loc = arma::find(spectrum.rows(min_index, max_index) == maximum);

    arma::uword max_pos = min_index + max_loc(0);
    double peak_center = abscissa(max_pos);
    double adj_maximum = maximum - inflection_baseline(max_pos);


    double half_maximum = adj_maximum / 2.0;
    //find left inflection points
    arma::vec region = spectrum.rows(min_index, max_index);
    region = region - inflection_baseline;
    region = region - half_maximum * arma::ones(region.n_rows);
    arma::uword i = 0;
    //search for first positive point, point before is inflection
    while (region(i) < 0){++i;}
    arma::uword left = i;
    //search for first negative point after first infleciton
    while (region(i) >= 0){++i;}
    arma::uword right = i;

    left = std::fabs(region(left)) < std::fabs(region(left + 1)) ? left : left + 1;
    right = std::fabs(region(right)) < std::fabs(region(right - 1)) ? right : right - 1;

    double fwhm = abscissa(right + min_index) - abscissa(left + min_index);

    results(0) = peak_center;
    results(1) = maximum;
    results(2) = adj_maximum;
    results(3) = total_area;
    results(4) = total_area - baseline_area;
    results(5) = area_between_inflection;
    results(6) = area_between_inflection - inf_baseline_area;
    results(7) = fwhm;

    return results;
}

arma::mat Vespucci::Math::Quantification::QuantifyPeakMat(const arma::mat &spectra, const arma::vec &abscissa, double &min, double &max, arma::uword bound_window, arma::mat &total_baselines, arma::field<arma::vec> &inflection_baselines)
{
    arma::mat results(spectra.n_cols, 7);
    inflection_baselines.clear();
    total_baselines.clear();
    inflection_baselines.set_size(spectra.n_cols);
    for (arma::uword i = 0; i < spectra.n_cols; ++i){
        arma::vec total_baseline;
        arma::vec inflection_baseline;
        results.row(i) = Vespucci::Math::Quantification::QuantifyPeak(spectra.col(i), abscissa, min, max, bound_window, total_baseline, inflection_baseline);
        inflection_baselines(i) = inflection_baseline;
        if (total_baselines.n_rows){
            total_baselines = arma::join_vert(total_baselines, total_baseline.t());
        }
        else{
            total_baselines = total_baseline.t();
        }

    }
    return results;
}

