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
#include <Math/Fitting/linleastsq.h>
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
arma::rowvec Vespucci::Math::Quantification::QuantifyPeak(const arma::vec &spectrum,
                                                          const arma::vec &abscissa,
                                                          double &min,
                                                          double &max,
                                                          arma::uword bound_window,
                                                          arma::mat &total_baseline,
                                                          arma::mat &inflection_baseline)
{
    //performs analysis of peak shape and magnitude
    arma::rowvec results(8);

    arma::uword min_index = Vespucci::Math::ClosestIndex(min, abscissa);
    arma::uword max_index = Vespucci::Math::ClosestIndex(max, abscissa);

    arma::vec abscissa_part = abscissa.rows(min_index, max_index);
    arma::vec spectrum_part = spectrum.rows(min_index, max_index);

    min = abscissa(min_index);
    max = abscissa(max_index);

    arma::uword min_start = min_index - bound_window;
    arma::uword min_end = min_index + bound_window;
    min_end = (min_end >= abscissa.n_rows ? abscissa.n_rows - 1 : min_end);

    arma::uword max_start = max_index - bound_window;
    arma::uword max_end = max_index + bound_window;
    max_end = (max_end >= abscissa.n_rows ? abscissa.n_rows - 1 : max_end);

    double total_area = arma::as_scalar(arma::trapz(abscissa_part, spectrum_part));

    arma::uword window_size = max_index - min_index + 1;
    total_baseline = arma::linspace(spectrum(min_index), spectrum(max_index), window_size);
    arma::vec total_abscissa = abscissa.rows(min_index, max_index);
    arma::uvec positive_indices = arma::find(spectrum_part > total_baseline);
    double positive_area = arma::as_scalar(arma::trapz(abscissa_part.rows(positive_indices), spectrum_part.rows(positive_indices)));
    double baseline_area = arma::as_scalar(arma::trapz(abscissa_part.rows(positive_indices), total_baseline.rows(positive_indices)));
    total_baseline = arma::join_horiz(total_abscissa, total_baseline);



    arma::vec min_window = spectrum.rows(min_start, min_end);
    arma::vec max_window = spectrum.rows(max_start, max_end);
    arma::uword inflection_min_index = Vespucci::Math::LocalMinimum(min_window, min) + min_start;
    arma::uword inflection_max_index = Vespucci::Math::LocalMinimum(max_window, max) + max_start;

    abscissa_part = abscissa.rows(inflection_min_index, inflection_max_index);
    spectrum_part = spectrum.rows(inflection_min_index, inflection_max_index);

    double area_between_inflection;
    try{
        area_between_inflection = arma::as_scalar(arma::trapz(abscissa_part, spectrum_part));
    }catch(std::exception e){
        std::cout << "Exception: invalid inflection points found" << std::endl;
        area_between_inflection = std::nan("");
        inflection_min_index = min_index;
        inflection_max_index = max_index;
    }

    window_size = inflection_max_index - inflection_min_index + 1;
    inflection_baseline = arma::linspace(spectrum(inflection_min_index), spectrum(inflection_max_index), window_size);
    double inf_baseline_area = arma::as_scalar(arma::trapz(abscissa_part, inflection_baseline));
    inflection_baseline = join_horiz(abscissa.rows(inflection_min_index, inflection_max_index), inflection_baseline);



    //we need to make sure the peak center is within the originally specified range
    arma::uword search_min_index = (inflection_min_index < min_index ? min_index : inflection_min_index);
    arma::uword search_max_index = (inflection_max_index > max_index ? max_index : inflection_max_index);
    arma::vec region = spectrum.rows(search_min_index, search_max_index);
    arma::vec region_abscissa = abscissa.rows(search_min_index, search_max_index);

    double maximum = region.max();
    arma::uword max_pos = region.index_max();
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
    results(4) = positive_area - baseline_area;
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
    //total_baselines.set_size(spectra.n_cols, )
    inflection_baselines.set_size(spectra.n_cols);
    arma::field<arma::mat> total_baselines_tmp(spectra.n_cols);

#ifdef _WIN32
  #pragma omp parallel for default(none) \
      shared(spectra, inflection_baselines, total_baselines_tmp, results, min, max, bound_window, abscissa)
     for (intmax_t i = 0; i < (intmax_t) spectra.n_cols; ++i)
#else
  #pragma omp parallel for default(none) \
      shared(spectra, inflection_baselines, total_baselines_tmp, results, min, max, bound_window, abscissa)
    for (size_t i = 0; i < spectra.n_cols; ++ i)
#endif
    {
        double temp_min = min;
        double temp_max = max;
        arma::mat total_baseline;
        arma::mat inflection_baseline;
        results.row(i) = Vespucci::Math::Quantification::QuantifyPeak(spectra.col(i), abscissa, temp_min, temp_max, bound_window, total_baseline, inflection_baseline);
        inflection_baselines(i) = inflection_baseline;
        total_baselines_tmp(i) = total_baseline;
    }

    for (size_t i = 0; i < total_baselines_tmp.n_rows; ++i){
        if (total_baselines.n_rows){
            total_baselines = arma::join_horiz(total_baselines, total_baselines_tmp(i).col(1));
        }
        else{
            total_baselines = total_baselines_tmp(i);
        }
    }
    return results;
}

///
/// \brief Vespucci::Math::Quantification::ConvertInflectionBaselines
/// \param inflection_baselines
/// \return
/// Create a matrix containing inflection point baselines. If a baseline does not contain a value for a particular abscissa point, the value is NaN
/// first column will be the abscissa
arma::mat Vespucci::Math::Quantification::ConvertInflectionBaselines(const arma::field<arma::mat> &inflection_baselines)
{
    arma::vec all_abscissa = inflection_baselines(0).col(0);

    for (arma::uword i = 0; i < inflection_baselines.n_elem; ++i){
        arma::vec current_baseline = inflection_baselines(i).col(0);
        all_abscissa = arma::join_vert(all_abscissa, current_baseline);
    }
    arma::vec abscissa = arma::unique(all_abscissa);
    abscissa = arma::sort(abscissa);
    arma::mat baseline_matrix(abscissa.n_rows, inflection_baselines.n_elem);

    for (arma::uword j = 0; j < inflection_baselines.n_elem; ++j){
        arma::vec current_baseline = inflection_baselines(j).col(1);
        arma::vec current_abscissa = inflection_baselines(j).col(0);
        for (arma::uword i = 0; i < abscissa.n_rows; ++i){
            arma::uvec indices = arma::find(current_abscissa == abscissa(i));
            baseline_matrix(i, j) = (indices.n_rows > 0 ?
                                         current_baseline(indices(0)) :
                                         std::nan(""));
        }
    }
    baseline_matrix = arma::join_horiz(abscissa, baseline_matrix);
    return baseline_matrix;
}

///
/// \brief Vespucci::Math::Quantification::FitGaussianPeak
/// \param spectrum A spectrum
/// \param abscissa The spectral abscissa
/// \param min Abscissa value of the left bound of the window
/// This parameter will change to the value closest to the specified value
/// \param max Abscissa value of the right bound of the window
/// This parameter will change to the vlaue closest to the specified value
/// \param iterations Number of iterations
/// \param baseline The linear baseline drawn between spectral points at min and max
/// \param gauss_params The parameters of the gaussian peak
/// \param quad_params The parameters of the quadratic function used to fit gaussian
/// \param gauss_error The uncertainties in the parameters in gauss_params
/// \param quad_error The uncertainties in the parameters in quad_params
/// \return
///
/// Fit a guassian peak through iterative weighted least squares as described by
/// Guo in IEEE Signal Processing Magazine, DOI: 10.1109/MSP.2011.941846
arma::rowvec Vespucci::Math::Quantification::FitGaussianPeak(const arma::vec &spectrum,
                                                             const arma::vec &abscissa,
                                                             double &min,
                                                             double &max,
                                                             arma::uword iterations,
                                                             arma::mat &baseline,
                                                             arma::mat &fit,
                                                             arma::rowvec &gauss_params,
                                                             arma::rowvec &quad_params,
                                                             arma::mat residuals)
{

    arma::rowvec results(5);

    arma::uword min_index = Vespucci::Math::ClosestIndex(min, abscissa);
    arma::uword max_index = Vespucci::Math::ClosestIndex(max, abscissa);

    arma::vec abscissa_part = abscissa.rows(min_index, max_index);
    arma::vec spectrum_part = spectrum.rows(min_index, max_index);

    min = abscissa(min_index);
    max = abscissa(max_index);
    arma::uword window_size = max_index - min_index;

    baseline = arma::linspace(spectrum(min_index), spectrum(max_index), window_size);
    spectrum_part -= baseline;

    arma::vec xsquared = arma::pow(abscissa_part, 2);
    arma::vec params;
    for (arma::uword i = 0; i < iterations; ++i){
        arma::mat X = Vespucci::Math::Quantification::GuoX(fit, abscissa_part);
        arma::mat Y = Vespucci::Math::Quantification::GuoY(fit, abscissa_part);
        params = Vespucci::Math::LinLeastSq::OrdinaryLeastSquares(X, Y);
        fit = arma::exp(params(0,0) * arma::ones(spectrum_part.n_rows)
                        + params(1,0) * abscissa_part
                        + params(2,0) * xsquared);
    }
    quad_params = params.t();
    residuals = spectrum_part - fit;
    arma::vec centered = spectrum_part - arma::as_scalar(arma::mean(spectrum_part));
    double residual_sumsq = arma::as_scalar(arma::sum(arma::pow(residuals, 2.0)));
    double total_sumsq = arma::as_scalar(arma::sum(arma::pow(centered, 2.0)));
    double R_squared = 1.0 - (residual_sumsq/total_sumsq);

    double A = std::exp(quad_params(0) - (0.25*std::pow(quad_params(1), 2)/quad_params(3)));
    double sigma = (quad_params(2) < 0 ? std::sqrt(-0.5/quad_params(2)) : std::nan(""));
    double mu = -0.5*quad_params(1)/quad_params(2);
    gauss_params = arma::rowvec({A, mu, sigma});

    results(0) = mu; //center
    results(1) = A; //intensity
    results(2) = arma::datum::sqrt2 * std::abs(sigma) * std::sqrt(arma::datum::pi); //area
    results(3) = 2 * std::sqrt(2 * std::log(2)) * sigma; //fwhm
    results(4) = R_squared; //R-squared
    fit += baseline;
    baseline = arma::join_horiz(abscissa_part, baseline);
    residuals = arma::join_horiz(abscissa_part, residuals);
    fit = arma::join_horiz(abscissa_part, fit);

    return results;
}

arma::mat Vespucci::Math::Quantification::GuoX(const arma::vec &x, const arma::vec &y)
{
    arma::vec ysquared = arma::pow(y, 2);
    arma::vec xsquared = arma::pow(x, 2);
    arma::mat X(3, 3);
    X(0,0) = arma::sum(ysquared);
    X(0,1) = arma::sum(x % ysquared);
    X(0,2) = arma::sum(ysquared % xsquared);
    X(1,0) = arma::sum(x % ysquared);
    X(1,1) = x(0,2);
    X(1,2) = arma::sum(xsquared % x % ysquared);
    X(2,0) = x(0,2);
    X(2,1) = x(1,2);
    X(2,2) = arma::sum(xsquared % xsquared % ysquared);
    return X;
}

arma::mat Vespucci::Math::Quantification::GuoY(const arma::vec &x, const arma::vec &y)
{
    arma::mat Y(3,1);
    arma::vec ysquared = arma::pow(y, 2);
    arma::vec xsquared = arma::pow(x, 2);
    arma::vec logy = arma::log(y);
    Y(0,0) = arma::sum(ysquared % logy);
    Y(1,0) = arma::sum(x % ysquared % logy);
    Y(2,0) = arma::sum(xsquared % ysquared % logy);
    return Y;
}

arma::mat Vespucci::Math::Quantification::FitGaussianPeakMat(const arma::mat &spectra,
                                                             const arma::vec &abscissa,
                                                             double &min,
                                                             double &max,
                                                             arma::uword iterations,
                                                             arma::mat &baselines,
                                                             arma::mat &fits,
                                                             arma::mat &gauss_params,
                                                             arma::mat &quad_params,
                                                             arma::mat &residuals)
{
    arma::mat results(spectra.n_cols, 5);
    gauss_params.clear();
    quad_params.clear();
    fits.clear();
    residuals.clear();
    baselines.clear();
    arma::field<arma::mat> baselines_tmp(spectra.n_cols);
    arma::field<arma::mat> fits_tmp(spectra.n_cols);
    arma::field<arma::mat> residuals_tmp(spectra.n_cols);
    gauss_params.set_size(spectra.n_cols, 3);
    quad_params.set_size(spectra.n_cols, 3);

#ifdef _WIN32
  #pragma omp parallel for default(none) \
      shared(spectra, inflection_baselines, total_baselines_tmp, results, min, max, bound_window, abscissa)
     for (intmax_t i = 0; i < (intmax_t) spectra.n_cols; ++i)
#else
  #pragma omp parallel for default(none) \
      shared(spectra, inflection_baselines, total_baselines_tmp, results, min, max, bound_window, abscissa)
    for (size_t i = 0; i < spectra.n_cols; ++ i)
#endif
    {
        double temp_min = min;
        double temp_max = max;
        arma::vec baseline;
        arma::rowvec gauss_params_tmp;
        arma::rowvec quad_params_tmp;
        arma::vec current_residuals;
        arma::vec fit;
        results.row(i) = Vespucci::Math::Quantification::FitGaussianPeak(spectra.col(i),
                                                                         abscissa,
                                                                         temp_min,
                                                                         temp_max,
                                                                         iterations,
                                                                         baseline,
                                                                         fit,
                                                                         gauss_params_tmp,
                                                                         quad_params_tmp,
                                                                         current_residuals);
        baselines_tmp(i) = baseline;
        gauss_params.row(i) = gauss_params_tmp;
        quad_params.row(i) = quad_params_tmp;
        fits_tmp(i) = fit;
        residuals_tmp(i) = current_residuals;
    }

    for (size_t i = 0; i < baselines_tmp.n_rows; ++i){
        if (baselines.n_rows)
            baselines = arma::join_horiz(baselines, baselines_tmp(i).col(1));
        else
            baselines = baselines_tmp(i);
        if (fits.n_rows)
            fits = arma::join_horiz(fits, fits_tmp(i).col(1));
        else
            fits = fits_tmp(i);
        if (residuals.n_rows)
            residuals = arma::join_horiz(residuals, residuals_tmp(i).col(1));
        else
            residuals = residuals_tmp(i);

    }
    return results;
}
