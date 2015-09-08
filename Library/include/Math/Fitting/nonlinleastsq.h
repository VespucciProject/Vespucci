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
#ifndef NONLINLEASTSQ_H
#define NONLINLEASTSQ_H

#include "Math/Accessory/accessory.h"
#include "Math/Accessory/accessory_impl.h"

    //#include <levmar.h>
    //#include <compiler.h>
    //#include <misc.h>
#include<cminpack.h>
#include<minpack.h>
namespace Vespucci{
    namespace Math{
    /// \namespace Vespucci::Math::Fitting
    /// \brief A namespace for math functions relating to non-linear curve fitting
    ///
    ///
        namespace Fitting{
            arma::vec FitBaseline(arma::vec signal, std::string function, std::vector<double> initial_params, std::vector<double> lims);
            arma::vec FitGaussianPeak(arma::vec signal, int iterations, std::string &reason, std::map<std::string, std::pair<double, double> > &params, std::map<std::string, double> &stats, arma::vec &residuals);
            void Gaussian(double *p, double *x, int m, int n, void *data);
            void GaussianJacobian(double *p, double *jac, int m, int n, void *data);
            void Lorentzian(double *p, double *x, int m, int n, void *data);
            void LorentzianJacobian(double *p, double *jac, int m, int n, void *data);
            void PseudoVoigt(double *p, double *x, int m, int n, void *data);
            void PseudoVoigtJacobian(double *p, double *jac, int m, int n, void *data);
            
            

        }
    }
}

#endif // NONLINLEASTSQ_H
