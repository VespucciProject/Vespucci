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

#ifndef SMOOTHING_H
#define SMOOTHING_H
#include <Math/Accessory/accessory.h>

namespace Vespucci
{
    namespace Math
    {
    /// \namespace Vespucci::Math::Smoothing
    /// \brief A namespace for math functions relating to filtering and smoothing signals.
    ///
    ///
        namespace Smoothing
        {
            arma::mat sgolay(arma::uword poly_order, arma::uword window_size, arma::uword deriv_order, arma::uword scaling_factor);
            arma::mat sgolayfilt(const arma::mat &x, arma::uword poly_order, arma::uword window_size, arma::uword deriv_order, arma::uword scaling_factor);
            arma::vec ApplyFilter(const arma::vec &x, arma::mat coefficients, arma::uword window_size);
            arma::vec ApplyFilter(const arma::vec &x, arma::vec filter);
            arma::vec CreateMovingAverageFilter(arma::uword window_size);
            arma::vec MedianFilter(const arma::vec &X, arma::uword window_size);
            arma::mat MedianFilterMat(const arma::mat &X, arma::uword window_size);
            arma::vec WhittakerSmooth(const arma::vec &x, double lambda, arma::uword penalty_order);
            arma::mat InterpolateToNewAbscissa(const arma::mat &spectra,
                                               const arma::vec &old_abscissa,
                                               const arma::vec &new_abscissa,
                                               const int window_size=2,
                                               const int order=1);
        }
    }
}
#endif //SMOOTHING_H
