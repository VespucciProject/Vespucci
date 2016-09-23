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
#include <lmcurve.h>
#include "Math/Accessory/Faddeeva.h"
///
/// \brief Gaussian
/// \param t values of t or x...
/// \param p parameters A, b, c, in that order
/// \return
///
double GaussianFn(double t, const double *p)
{
    return p[0]*std::exp(-0.5*std::pow(t-p[1], 2)/std::pow(p[2], 2));
}

///
/// \brief Lorentzian
/// \param t
/// \param p parameters I, gamma and x0, in that order
/// \return
///
double LorentzianFn(double t, const double *p)
{
    return p[0] * std::pow(p[1], 2)/(std::pow(t - p[2], 2) + std::pow(p[1], 2));
}

///
/// \brief VoigtFn
/// \param t
/// \param p amplitude, center, sigma, gamma
/// \return
///
double VoigtFn(double t, const double *p)
{
    const std::complex<double> i(0.0, 1.0);
    std::complex<double> z = (t - p[1] + i*p[3]) / (p[2] * arma::datum::sqrt2);
    return p[0] * Faddeeva::w(z).real() / (p[2] * arma::datum::sqrt2);
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

///
/// \brief Vespucci::Math::NonLinLeastSq::FitGaussianPeak
/// \param x
/// \param y
/// \return
/// Returns parameters A, intensity, b, the center, and c, the RMS width
arma::vec Vespucci::Math::NonLinLeastSq::FitGaussian(arma::vec x, arma::vec y)
{
    if (x.n_rows != y.n_rows) throw(std::invalid_argument("FitGaussian: x and y must be same size!"));
    arma::vec params(3);
    lm_control_struct control = lm_control_double;
    lm_status_struct status;
    control.verbosity = 7;
    params = Vespucci::Math::NonLinLeastSq::EstimateGaussParams(x, y);
    lmcurve(3, params.memptr(), x.n_rows, x.memptr(), y.memptr(), GaussianFn, &control, &status);
    return params;
}

arma::vec Vespucci::Math::NonLinLeastSq::FitLorentzian(arma::vec x, arma::vec y)
{
    if (x.n_rows != y.n_rows) throw(std::invalid_argument("FittLorentzian: x and y must be same size!"));
    arma::vec params(2);
    double I, x0, gamma;
    return arma::vec({I, x0, gamma});

}

arma::vec Vespucci::Math::NonLinLeastSq::EstimateLorentzParams(arma::vec x, arma::vec y)
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
    arma::vec invy = y.transform([](const double t){return 1.0/t;});
    Y(0,0) = arma::sum(ysquared % invy);
    Y(1,0) = arma::sum(x % ysquared % invy);
    Y(2,0) = arma::sum(xsquared % ysquared % invy);
}
