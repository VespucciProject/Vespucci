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
#ifndef DENOISE_H
#define DENOISE_H
#include "libvespucci.h"
#include <mlpack/core.hpp>
namespace Vespucci{
    namespace Math{
        namespace Smoothing{
            VESPUCCI_EXPORT arma::mat SVDDenoise(const arma::mat &X, arma::uword k, arma::mat &U, arma::vec &s, arma::mat &V);
            VESPUCCI_EXPORT arma::mat QUICSVDDenoise(const arma::mat &X, double epsilon, arma::mat &U, arma::vec &s, arma::mat &V, arma::uword &rank);
        }
    }
}
#endif // DENOISE_H
