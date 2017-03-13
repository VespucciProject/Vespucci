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
#ifndef NONLINLEASTSQ_H
#define NONLINLEASTSQ_H
#include <mlpack/core.hpp>
#include "libvespucci.h"
extern "C"
{
    double GaussianFn(double t, const double *p);
    double LorentzianFn(double t, const double *p);
    double VoigtFn(double t, const double *p);
}
namespace Vespucci{
    namespace Math{
        namespace NonLinLeastSq{
            VESPUCCI_EXPORT arma::vec EstimateGaussParams(arma::vec x, arma::vec y);
            VESPUCCI_EXPORT arma::vec EstimateLorentzParams(arma::vec x, arma::vec y);
            VESPUCCI_EXPORT arma::vec FitGaussian(arma::vec x,
                                                  arma::vec y);
            VESPUCCI_EXPORT arma::vec FitLorentzian(arma::vec x,
                                                    arma::vec y);
            VESPUCCI_EXPORT arma::vec FitVoigt(arma::vec x,
                                               arma::vec y);
        }
    }
}


#endif // NONLINLEASTSQ_H
