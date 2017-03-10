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
#include "Math/Fitting/nonlinleastsq.h"
#include "Math/Fitting/linleastsq.h"
//#include <lmcurve.h>
#include "Math/Accessory/Faddeeva.h"
#include <limits>
///
/// \brief Gaussian
/// \param t values of t or x...
/// \param p parameters A, b (mu), c (sigma), in that order
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

///
/// \brief Vespucci::Math::NonLinLeastSq::EstimateGaussParams
/// \param x
/// \param y
/// \return
/// When estimated sigma parameter is nonreal, returns sigma as the standard deviation of y
arma::vec Vespucci::Math::NonLinLeastSq::EstimateGaussParams(arma::vec x, arma::vec y)
{
    arma::vec logy = arma::log(y);
    arma::mat vdm = Vespucci::Math::LinLeastSq::Vandermonde(x, 2);
    arma::vec quad_params = Vespucci::Math::LinLeastSq::OrdinaryLeastSquares(vdm, logy);

    double A = std::exp(quad_params(0) - (0.25*std::pow(quad_params(1), 2)/quad_params(2)));
    double sigma = (quad_params(2) < 0 ? std::sqrt(-0.5/quad_params(2)) : arma::stddev(y));
    double mu = -0.5*quad_params(1)/quad_params(2);

    return arma::vec({A, mu, sigma});
}

arma::vec Vespucci::Math::NonLinLeastSq::EstimateLorentzParams(arma::vec x, arma::vec y)
{
    arma::vec invy = y.transform([](const double t){return (t != 0 ? 1.0/t : std::numeric_limits<double>::max());});
    arma::mat vdm = Vespucci::Math::LinLeastSq::Vandermonde(x, 2);
    arma::vec quad_params = Vespucci::Math::LinLeastSq::OrdinaryLeastSquares(vdm, invy);
    double x0 = -0.5 * quad_params(1) / quad_params(0);
    double inside_sqrt = (quad_params(2) - std::pow(x0, 1)) / quad_params(0);
    double gamma = (inside_sqrt >= 0 ? std::sqrt(inside_sqrt) : arma::stddev(y));
    double I = 1.0 / (quad_params(0) * gamma);
    return arma::vec({I, gamma, x0});
}
///
/// \brief Vespucci::Math::NonLinLeastSq::FitGaussian
/// \param x
/// \param y
/// \return
/// Returns parameters A, intensity, b, the center, and c, the RMS width
arma::vec Vespucci::Math::NonLinLeastSq::FitGaussian(arma::vec x, arma::vec y)
{
    /*
    if (x.n_rows != y.n_rows) throw(std::invalid_argument("FitGaussian: x and y must be same size!"));
    lm_control_struct control = lm_control_double;
    lm_status_struct status;
    control.verbosity = 7;
    arma::vec params = Vespucci::Math::NonLinLeastSq::EstimateGaussParams(x, y);
    //lmcurve(3, params.memptr(), x.n_rows, x.memptr(), y.memptr(), GaussianFn, &control, &status);
    return params;
*/  return arma::vec();
}

arma::vec Vespucci::Math::NonLinLeastSq::FitLorentzian(arma::vec x, arma::vec y)
{
/*
    if (x.n_rows != y.n_rows) throw(std::invalid_argument("FittLorentzian: x and y must be same size!"));
    lm_control_struct control = lm_control_double;
    lm_status_struct status;
    control.verbosity = 7;
    arma::vec params = Vespucci::Math::NonLinLeastSq::EstimateLorentzParams(x, y);
    lmcurve(3, params.memptr(), x.n_rows, x.memptr(), y.memptr(), LorentzianFn, &control, &status);
    return params;
  */return arma::vec();
}



arma::vec Vespucci::Math::NonLinLeastSq::FitVoigt(arma::vec x, arma::vec y)
{
    /*
    if (x.n_rows != y.n_rows) throw(std::invalid_argument("FittLorentzian: x and y must be same size!"));
    lm_control_struct control = lm_control_double;
    lm_status_struct status;
    control.verbosity = 7;
    arma::vec lorentz_params = Vespucci::Math::NonLinLeastSq::EstimateLorentzParams(x, y);
    arma::vec gauss_params = Vespucci::Math::NonLinLeastSq::EstimateGaussParams(x, y);
    arma::vec params({gauss_params(0), gauss_params(1), gauss_params(2), lorentz_params(1)});
    lmcurve(4, params.memptr(), x.n_rows, x.memptr(), y.memptr(), VoigtFn, &control, &status);
    return params;
  */return arma::vec();
}
