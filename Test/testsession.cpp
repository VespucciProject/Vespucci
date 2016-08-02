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
#include "testsession.h"

TestSession::TestSession()
{

}

///
/// \brief TestSession::GenerateMockData
/// \param spectra
/// \param abscissa
/// \param x
/// \param y
/// Generate mock spectra
/// This is a matrix containing 100 spectra in columns.
/// The abscissa will consist of 100 to 4000 cm-1 with 4096 elements
/// 50 spectra will have Gaussian peaks
/// 50 spectra will have Lorentzian peaks
/// For each peak type, there will be 5 unique spectra, each with 5 different baselines
/// Peak centers will be uniformly distributed from 200 to 3800 cm-1
/// Peak widths will be uniformly distributed from 5 to 100 cm-1
/// Peak intensities will be uniformly distributed from 1000 to 50000 a.u.
/// Basline types:
///     * None
///     * Typical Raman baseline (generated from linear regression of a quadratic
///       function to 100 Si Raman signals)
///     * Fluorescent Raman baseline (generated from linear regression of a quadratic
///       function to 100 highly-fluorescent Rhodamine-6G Raman signals)
///     * Average of typical and fluorescent baseline
///     * Linear baseline from 0 to 25000 a.u.
///
/// Mock spectra for IR and MS signals will be created in the future
void TestSession::GenerateMockData(mat &spectra, vec &abscissa, vec &x, vec &y,
                                   mat &peak_data, mat &baselines)
{
    double raman_a0 = 1;
    double raman_a1 = 1;
    double raman_a2 = 1;
    double fluor_a0 = 1;
    double fluor_a1 = 1;
    double fluor_a2 = 1;
    abscissa = linspace(100, 4000, 4096);
    spectra = zeros(4096, 100);
    vec coords = linspace(0, 9, 10);
    y = coords;
    while (y.n_rows < 100)
       y = join_vert(y, coords);

    x = zeros(10);
    uword x_row = 0;
    while (x.n_rows < 100)
        x = join_vert(x, (x_row++) * ones(10));
    mat unique_baselines(4096, 5);
    unique_baselines.col(0) = zeros(4096);
    unique_baselines.col(1) = GenerateRamanBaseline(raman_a0, raman_a1, raman_a2, abscissa);
    unique_baselines.col(2) = GenerateRamanBaseline(fluor_a0, fluor_a1, fluor_a2, abscissa);
    unique_baselines.col(3) = 0.5 * (unique_baselines.col(1), unique_baselines.col(2));
    unique_baselines.col(4) = linspace(0, 25000, 4096);
    vec peak_centers = (3600 * randu<vec>(4096)) + (200 * ones(4096));
    vec peak_intensities = (49000 * randu<vec>(4096)) + (1000 * ones(4096));
    vec peak_fwhm = (95 * randu<vec>(4096)) + (5 * ones(4096));
    for (uword i = 0; i < 50; ++i){
        for (uword j = 0; j < 5; ++j){
            vec peak_center = peak_centers.col(j);
            vec peak_intensity = peak_intensities.col(j);
            vec peak_width = peak_fwhm.col(j);
            for (uword k = 0; k < 5; ++j){
                vec baseline = baselines.col(k);
                baselines.col(i) = baseline;
            }
        }
    }

}

arma::vec TestSession::GenerateLinearBaseline(double min, double max, uword size)
{
    return linspace(min, max, size);
}

arma::vec TestSession::GenerateRamanBaseline(double a0, double a1, double a2, const vec &abscissa)
{
    return a0 + a1*abscissa + a2*arma::pow(abscissa, 2);
}

vec TestSession::GenerateGaussian(vec abscissa, vec centers, vec fwhm, vec intensities)
{

}
