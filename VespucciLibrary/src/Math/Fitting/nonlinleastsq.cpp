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
#include "Math/Fitting/nonlinleastsq.h"
#include "Math/Fitting/linleastsq.h"

///
/// \brief Gaussian
/// \param t values of t or x...
/// \param p parameters A, b, c, in that order
/// \return
///
double Gaussian(double t, const double *p)
{
    return p[0]*std::exp(-0.5*std::pow(t-p[1], 2)/std::pow(p[2], 2));
}

///
/// \brief Lorentzian
/// \param t
/// \param p parameters gamma and x0, in that order
/// \return
///
double Lorentzian(double t, const double *p)
{
    return (1/(arma::datum::pi * p[0])) * std::pow(p[0], 2)/(std::pow(t - p[1], 2) + std::pow(p[0], 2));
}

///
/// \brief PseudoVoigt
/// \param t
/// \param p 6 params, eta first, then Lorentzian, then Gaussian
/// \return
///
double PseudoVoigt(double t, const double *p)
{
    double gauss_p[3];
    double lorentz_p[2];
    guass_p[0] = p[1];
    gauss_p[1] = p[2];
    gauss_p[2] = p[3];
    lorentz_p[0] = p[4];
    lorentz_p[1] = p[5];
    return p[0] * Lorentzian(t, lorentz_p) + (1-p[0]) * Gaussian(t, gauss_p);
}

arma::vec Vespucci::Math::NonLinLeastSq::EstimateGaussParams(arma::vec x, arma::vec y)
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

    arma::mat Y(3,1);
    arma::vec logy = arma::log(y);
    Y(0,0) = arma::sum(ysquared % logy);
    Y(1,0) = arma::sum(x % ysquared % logy);
    Y(2,0) = arma::sum(xsquared % ysquared % logy);

    arma::vec quad_params = Vespucci::Math::LinLeastSq::OrdinaryLeastSquares(X, Y);
    double A = std::exp(quad_params(0) - (0.25*std::pow(quad_params(1), 2)/quad_params(3)));
    double sigma = (quad_params(2) < 0 ? std::sqrt(-0.5/quad_params(2)) : std::nan(""));
    double mu = -0.5*quad_params(1)/quad_params(2);

    return arma::vec({A, mu, sigma});
}

