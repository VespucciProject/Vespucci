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
#ifndef KERNELPEAKFINDING_H
#define KERNELPEAKFINDING_H
#if !defined( SWIG )
    // SWIG should not see #inlcude<armadillo> as it can not handle it
    #include "Math/VespucciMath.h"
    #include "libvespucci.h"
#endif
#include "mlpack/core.hpp"
namespace Vespucci{
    namespace Math{
        namespace PeakFinding{
            VESPUCCI_EXPORT arma::uvec FindPeakCenters(const arma::vec &signal,
                                                       const arma::vec &abscissa,
                                                       std::list<arma::umat> &ridges,
                                                       arma::uword first_scale,
                                                       arma::uword last_scale,
                                                       arma::uword search_window,
                                                       arma::uword min_length,
                                                       arma::uword max_gap,
                                                       const std::string &max_method);
            VESPUCCI_EXPORT arma::vec DerivGaussKernel(double abscissa_step,
                                                             arma::uword window_size,
                                                             arma::uword width,
                                                             double height);
            VESPUCCI_EXPORT arma::vec MexicanHatKernel(double abscissa_step,
                                                             arma::uword window_size,
                                                             arma::uword width,
                                                             double height);
            VESPUCCI_EXPORT arma::uvec FindIntercepts(const arma::vec &signal);
        }
    }
}

#endif // KERNELPEAKFINDING_H
