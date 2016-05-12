/*******************************************************************************
    Copyright (C) 2014-2016 Wright State University - All Rights Reserved
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
arma::rowvec Vespucci::Math::Quantification::QuantifyPeak(const arma::vec &spectrum, const arma::vec &abscissa, double &min, double &max, arma::uword bound_window, arma::mat &total_baseline, arma::mat &inflection_baseline)
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
    arma::vec total_abscissa = abscissa.rows(min_index, max_index);
    double baseline_area = arma::accu(total_baseline) / (abscissa(max_index) - abscissa(min_index));
    total_baseline = arma::join_horiz(total_abscissa, total_baseline);

    arma::vec min_window = spectrum.rows(min_start, min_end);
    arma::vec max_window = spectrum.rows(max_start, max_end);
    arma::uword inflection_min_index = Vespucci::Math::LocalMinimum(min_window, min) + min_start;
    arma::uword inflection_max_index = Vespucci::Math::LocalMinimum(max_window, max) + max_start;

    double area_between_inflection;
    try{
        area_between_inflection = arma::sum(spectrum.rows(min_index, max_index));
    }catch(std::exception e){
        std::cout << "Exception: invalid inflection points found" << std::endl;
        area_between_inflection = std::nan("");
        inflection_min_index = min_index;
        inflection_max_index = max_index;
    }

    area_between_inflection = area_between_inflection / (abscissa(inflection_max_index) - abscissa(inflection_min_index));
    window_size = inflection_max_index - inflection_min_index + 1;
    inflection_baseline = arma::linspace(spectrum(inflection_min_index), spectrum(inflection_max_index), window_size);
    double inf_baseline_area = arma::accu(inflection_baseline) / (abscissa(inflection_min_index) - abscissa(inflection_max_index));



    //we need to make sure the peak center is within the originally specified range
    arma::uword search_min_index = (inflection_min_index < min_index ? min_index : inflection_min_index);
    arma::uword search_max_index = (inflection_max_index > max_index ? max_index : inflection_max_index);
    arma::vec region = spectrum.rows(search_min_index, search_max_index);
    arma::vec region_abscissa = abscissa.rows(search_min_index, search_max_index);
    inflection_baseline = join_horiz(region_abscissa, region);
    double maximum = region.max();
    arma::uvec max_loc = arma::find(region == maximum);
    arma::uword max_pos = max_loc(0);
    arma::vec search_baseline = arma::linspace(region(0), region(region.n_rows - 1), region.n_rows);
    double peak_center = region_abscissa(max_pos);
    double adj_maximum = maximum - search_baseline(max_pos);

    double half_maximum = adj_maximum / 2.0;
    //find left inflection points

    arma::vec bandwidth_region = spectrum.rows(search_min_index, search_max_index);
    arma::vec bandwidth_baseline = arma::linspace(bandwidth_region(0), bandwidth_region(bandwidth_region.n_rows - 1), bandwidth_region.n_rows);
    bandwidth_region = bandwidth_region - bandwidth_baseline;
    arma::vec bandwidth_abscissa = abscissa.rows(search_min_index, search_max_index);
    bandwidth_region = bandwidth_region - half_maximum * arma::ones(bandwidth_region.n_rows);
    arma::uword i = 0;
    //search for first positive point, point before is inflection
    while (i < bandwidth_region.n_rows && bandwidth_region(i) < 0){++i;}
    arma::uword left = (i >= bandwidth_region.n_rows ? bandwidth_region.n_rows - 1 : i);

    //search for first negative point after first infleciton
    while (i < bandwidth_region.n_rows && bandwidth_region(i) >= 0){++i;}
    arma::uword right = (i >= bandwidth_region.n_rows ? bandwidth_region.n_rows - 1 : i);

    if (bandwidth_region.n_rows > left)
        left = std::fabs(bandwidth_region(left)) < std::fabs(bandwidth_region(left + 1)) ? left : left + 1;
    if (right > 0)
        right = std::fabs(bandwidth_region(right)) < std::fabs(bandwidth_region(right - 1)) ? right : right - 1;


    double fwhm = bandwidth_abscissa(right) - bandwidth_abscissa(left);

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

arma::mat Vespucci::Math::Quantification::QuantifyPeakMat(const arma::mat &spectra, const arma::vec &abscissa, double &min, double &max, arma::uword bound_window, arma::mat &total_baselines, arma::field<arma::mat> &inflection_baselines)
{
    arma::mat results(spectra.n_cols, 8);
    inflection_baselines.clear();
    total_baselines.clear();
    inflection_baselines.set_size(spectra.n_cols);
    for (arma::uword i = 0; i < spectra.n_cols; ++i){
        double temp_min = min;
        double temp_max = max;
        arma::mat total_baseline;
        arma::mat inflection_baseline;
        results.row(i) = Vespucci::Math::Quantification::QuantifyPeak(spectra.col(i), abscissa, temp_min, temp_max, bound_window, total_baseline, inflection_baseline);
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

