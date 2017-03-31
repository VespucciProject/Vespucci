
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
#if !defined( SWIG )
    // SWIG should not see #inlcude<armadillo> as it can not handle it
    #include "Math/VespucciMath.h"
    #include "libvespucci.h"
#endif
#ifndef FIR_H
#define FIR_H

namespace Vespucci{
    namespace Math{
        namespace Smoothing{
        VESPUCCI_EXPORT arma::mat sgolay(arma::uword poly_order, arma::uword window_size, arma::uword deriv_order, arma::uword scaling_factor);
        VESPUCCI_EXPORT arma::mat sgolayfilt(const arma::mat &x, arma::uword poly_order, arma::uword window_size, arma::uword deriv_order, arma::uword scaling_factor);
        VESPUCCI_EXPORT arma::vec ApplyFilter(const arma::vec &x, arma::mat coefficients, arma::uword window_size);
        VESPUCCI_EXPORT arma::vec ApplyFilter(const arma::vec &x, arma::vec filter);
        VESPUCCI_EXPORT arma::vec CreateMovingAverageFilter(arma::uword window_size);
        VESPUCCI_EXPORT arma::vec MedianFilter(const arma::vec &X, arma::uword window_size);
        VESPUCCI_EXPORT arma::mat MedianFilterMat(const arma::mat &X, arma::uword window_size);
        VESPUCCI_EXPORT arma::vec WhittakerSmooth(const arma::vec &x, double lambda, arma::uword penalty_order);
        VESPUCCI_EXPORT arma::mat InterpolateToNewAbscissa(const arma::mat &spectra,
                                           const arma::vec &old_abscissa,
                                           const arma::vec &new_abscissa,
                                           const int window_size,
                                           const int order);
        VESPUCCI_EXPORT arma::mat InterpolateToNewAbscissa(const arma::mat &spectra,
                                                           const arma::vec &old_abscissa,
                                                           const arma::vec &new_abscissa);
        }
    }
}


#endif // FIR_H
