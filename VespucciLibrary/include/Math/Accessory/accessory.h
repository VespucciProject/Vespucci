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
#ifndef ACCESSORY_H
#define ACCESSORY_H
#include <mlpack/core.hpp>
#include "Math/Accessory/accessory_impl.h"
#include "libvespucci.h"
namespace Vespucci
{
    namespace Math
    {
        //general Math functions

        VESPUCCI_EXPORT arma::uword LocalMaximum(const arma::vec &X, double &value);
        VESPUCCI_EXPORT arma::uword LocalMaximum(const arma::vec &dX, const arma::vec &d2X, double &value);
        VESPUCCI_EXPORT arma::uword LocalMaximum(const arma::vec &X, const arma::vec &dX, const arma::vec &d2X, double &value);

        VESPUCCI_EXPORT arma::uword LocalMinimum(const arma::mat &X, double &value);
        VESPUCCI_EXPORT arma::uword LocalMinimum(const arma::vec &dX, const arma::vec &d2X, double &value);
        VESPUCCI_EXPORT arma::uword LocalMinimum(const arma::vec &X, const arma::vec &dX, const arma::mat &d2X, double &value);


        VESPUCCI_EXPORT arma::sp_mat LocalMaxima(const arma::mat &X);
        VESPUCCI_EXPORT arma::sp_mat LocalMaxima(const arma::mat &X, const arma::mat &dX, const arma::mat &d2X);

        VESPUCCI_EXPORT arma::sp_mat LocalMinima(const arma::mat &X);
        VESPUCCI_EXPORT arma::sp_mat LocalMinima(const arma::mat &X, const arma::mat &dX, const arma::mat &d2X);

        VESPUCCI_EXPORT arma::sp_mat LocalMinimaWindow(const arma::mat &X, const int window_size);
        VESPUCCI_EXPORT arma::sp_mat LocalMaximaWindow(const arma::mat &X, const int window_size);

        VESPUCCI_EXPORT arma::sp_mat LocalMinimaCWT(arma::mat coefs, arma::uvec scales, arma::uword min_window_size, double amplitude_threshold);
        VESPUCCI_EXPORT arma::sp_mat LocalMaximaCWT(arma::mat coefs, arma::uvec scales, arma::uword min_window_size);

        VESPUCCI_EXPORT double quantile(arma::vec &data, double probs);

        VESPUCCI_EXPORT arma::vec ExtendToNextPow(arma::vec X, arma::uword n);
        VESPUCCI_EXPORT arma::uword NextPow(arma::uword number, arma::uword power);

        VESPUCCI_EXPORT arma::mat spdiags(arma::mat B, arma::ivec d, arma::uword m, arma::uword n);
        VESPUCCI_EXPORT arma::mat orth(arma::mat X); //this is implemented better in mlpack


        VESPUCCI_EXPORT arma::uword min(arma::uword a, arma::uword b);
        VESPUCCI_EXPORT arma::uword max(arma::uword a, arma::uword b);

        VESPUCCI_EXPORT void position(arma::uword index,
                                      arma::uword n_rows, arma::uword n_cols,
                                      arma::uword &i, arma::uword &j);
        VESPUCCI_EXPORT arma::umat to_row_column(arma::uvec indices, arma::uword n_rows, arma::uword n_cols);

        VESPUCCI_EXPORT double RecalculateAverage(double new_value, double old_average, double old_count);
        VESPUCCI_EXPORT double RecalculateStdDev(double new_value, double old_mean, double old_stddev, double old_count);

        VESPUCCI_EXPORT arma::umat GetClosestValues(arma::vec query, arma::vec target, const arma::uword k=5);

        VESPUCCI_EXPORT double CalcPoly(const double x, const arma::vec &coefs);

        VESPUCCI_EXPORT arma::vec RepresentativeSpectrum(const arma::mat &spectra, arma::uword &index, std::string metric_name="euclidean", std::string center="centroid");


        //Abscissa transforms
        VESPUCCI_EXPORT arma::vec WavelengthToFrequency(const arma::vec &x, double freq_factor, double wl_factor);
        VESPUCCI_EXPORT arma::vec FrequencyToWavelength(const arma::vec &x, double wl_factor, double freq_factor);
        VESPUCCI_EXPORT arma::vec FrequencyToEnergy(const arma::vec &x, double energy_factor, double freq_factor);
        VESPUCCI_EXPORT arma::vec EnergyToFrequency(const arma::vec &x, double freq_factor, double energy_factor);
        VESPUCCI_EXPORT arma::vec WavenumberToFrequency(const arma::vec &x, double freq_factor, double wn_factor);
        VESPUCCI_EXPORT arma::vec FrequencyToWavenumber(const arma::vec &x, double wn_factor, double freq_factor);
        VESPUCCI_EXPORT arma::vec WavenumberToWavelength(const arma::vec &x, double wn_factor, double wl_factor);
        VESPUCCI_EXPORT arma::vec WavelengthToWavenumber(const arma::vec &x, double wl_factor, double wn_factor);
        VESPUCCI_EXPORT arma::vec WavelengthToEnergy(const arma::vec &x, double energy_factor, double wl_factor);
        VESPUCCI_EXPORT arma::vec EnergyToWavelength(const arma::vec &x, double wl_factor, double energy_factor);
        VESPUCCI_EXPORT arma::vec EnergyToWavenumber(const arma::vec &x, double wn_factor, double energy_factor);
        VESPUCCI_EXPORT arma::vec WavenumberToEnergy(const arma::vec &x, double energy_factor, double wn_factor);


        VESPUCCI_EXPORT bool AreEqual(const arma::vec &a, const arma::vec &b);

        VESPUCCI_EXPORT bool IsMonotonic(const arma::vec &x);
        VESPUCCI_EXPORT bool IsIncreasing(const arma::vec &x);

        VESPUCCI_EXPORT arma::cx_vec cx_zeros(arma::uword n);
        VESPUCCI_EXPORT arma::cx_mat cx_zeros(arma::uword m, arma::uword n);

        VESPUCCI_EXPORT arma::uword ClosestIndex(double value, const arma::vec &vector);
        VESPUCCI_EXPORT arma::uvec Intersection(arma::uvec &x, arma::uvec &y);

        VESPUCCI_EXPORT double CalculateRSquared(const arma::vec &data,
                                                 const arma::vec &fit,
                                                 arma::vec &residuals);
        VESPUCCI_EXPORT arma::mat SafeRows(const arma::mat &x,
                                           arma::uword a,
                                           arma::uword b);
    }
}


#endif //ACCESSORY_H
