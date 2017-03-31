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
#ifndef LINLEASTSQ_H
#define LINLEASTSQ_H
#if !defined( SWIG )
    // SWIG should not see #inlcude<armadillo> as it can not handle it
    #include "Math/VespucciMath.h"
    #include "libvespucci.h"
#endif
namespace Vespucci{
    namespace Math{
        /// \namespace Vespucci::Math:::LinLeastSq
        /// \brief A namespace for math functions relating to linear
        /// least squares regression.
        ///
        ///
        namespace LinLeastSq{
            VESPUCCI_EXPORT arma::uword IModPoly(const arma::vec &spectrum, const arma::vec &abscissa,
                           arma::vec &baseline, arma::vec &corrected,
                           double &err,
                           const arma::uword poly_order,
                           const arma::uword max_it,
                           const double threshold);

            double CalcDev(const arma::vec &spectrum, const arma::vec &fit);
            arma::uvec NonPeakInd(const arma::vec &spectrum, const double dev);
            arma::vec CalcPoly(const arma::vec &coefs, const arma::vec &x);
            VESPUCCI_EXPORT arma::mat OrdinaryLeastSquares(const arma::mat &X, const arma::mat &y);
            VESPUCCI_EXPORT arma::mat OrdinaryLeastSquares(const arma::mat &X, const arma::mat &y, arma::mat &coef_errors, arma::mat &predicted);
            VESPUCCI_EXPORT arma::vec OrdinaryLeastSquares(const arma::mat &X, const arma::vec &y, arma::vec &fit, std::map<std::string, double> &stats);
            VESPUCCI_EXPORT arma::mat Vandermonde(const arma::vec &x, const int poly_order);
            double CalcErr(const double &dev, const double &prev_dev);
            VESPUCCI_EXPORT arma::vec FitGaussian(const arma::vec &x, const arma::vec &y, arma::vec &fit, std::map<std::string, double> &stats);


        }
    }

}


#endif // LINLEASTSQ_H
