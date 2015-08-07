/*******************************************************************************
    Copyright (C) 2014 Wright State University - All Rights Reserved
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
#include "linleastsq.h"
///
/// \brief Vespucci::Math::LinLeastSq::IModPoly Perform the Vancouver Raman Algorithm to correct baseline
/// \param spectrum A vector containing the signal to be corrected
/// \param abscissa The x-values of spectrum
/// \param baseline Will contain the fitted baseline
/// \param corrected Will contain the corrected spectrum
/// \param poly_order Polynomial order for the baseline fits
/// \param max_it Maximum number of iterations to perform. if set to 0 there is no maximum
/// \param threshold A value specifying the upper limit of the error criterion
/// \param err The final error criterion
/// \return The number of iterations
///
arma::uword Vespucci::Math::LinLeastSq::IModPoly(const arma::vec &spectrum,
                             const arma::vec &abscissa,
                             arma::vec &baseline,
                             arma::vec &corrected,
                             double &err,
                             const arma::uword poly_order,
                             const arma::uword max_it,
                             const double threshold)
{
    if (poly_order == 0)
        throw std::invalid_argument("Polynomial order must be 1 (linear) or greater.");
    if (threshold >= 1 || threshold <= 0)
        throw std::invalid_argument("Threshold value must be between 0 and 1.");
    if (spectrum.n_rows != abscissa.n_rows)
        throw std::invalid_argument("Spectrum and abscissa must be the same size.");

    arma::uword i = 1;
    bool no_max_it = (max_it == 0);
    arma::mat X;
    arma::vec coefs, fit;
    double dev;

    //perform first regresion (on spectrum without removed major peaks)
    X = Vandermonde(abscissa, poly_order);
    coefs = Vespucci::Math::LinLeastSq::OrdinaryLeastSquares(X, spectrum);
    fit = Vespucci::Math::LinLeastSq::CalcPoly(coefs, abscissa);
    dev = Vespucci::Math::LinLeastSq::CalcDev(spectrum, fit);

    double prev_dev = dev; //used in while loop critera

    //arma::arma::find major peak areas to remove and remove them
    arma::uvec non_peak_ind = NonPeakInd(spectrum, dev);
    arma::vec new_abscissa = abscissa(non_peak_ind);

    arma::vec prev_fit = spectrum(non_peak_ind);; //not a fit here, but in the loop.

    X = Vespucci::Math::LinLeastSq::Vandermonde(new_abscissa, poly_order);

    arma::uword rows = new_abscissa.n_rows;
    do{ //always perform at least one regression on the "shrunken" spectrum
        //Polynomial Fitting
        coefs = Vespucci::Math::LinLeastSq::OrdinaryLeastSquares(X, prev_fit);
        fit = Vespucci::Math::LinLeastSq::CalcPoly(coefs, new_abscissa);
        //Residual and dev calc (residual calculted inside CalcDev)
        dev = Vespucci::Math::LinLeastSq::CalcDev(prev_fit, fit);
        //error criteria
        err = Vespucci::Math::LinLeastSq::CalcErr(dev, prev_dev);
        //Reconstruction of model input
        fit += dev * arma::ones(rows);
        //if a value in the previous fit is lower than this fit, take the previous
        arma::uvec ind = arma::find (prev_fit < fit);
        fit(ind) = prev_fit(ind);
        prev_fit = fit;
        prev_dev = dev;
        ++i;
    }while (err > threshold && (no_max_it || i <= max_it));
    //calculate fit for all values in original abscissa
    baseline = Vespucci::Math::LinLeastSq::CalcPoly(coefs, abscissa);
    corrected = spectrum - baseline;
    return i;
}

///
/// \brief Vespucci::Math::LinLeastSq::CalcDev
/// \param spectrum
/// \param fit
/// \return
///
double Vespucci::Math::LinLeastSq::CalcDev(const arma::vec &spectrum, const arma::vec &fit)
{
    using namespace arma;
    arma::vec R = spectrum - fit;
    double R_avg = mean(R);
    arma::vec centered = R - R_avg*arma::ones(R.n_rows);
    centered = arma::pow(centered, 2.0);
    return std::sqrt(sum(centered)/centered.n_rows);
}

///
/// \brief Vespucci::Math::LinLeastSq::NonPeakInd
/// \param spectrum
/// \param dev
/// \return
///
arma::uvec Vespucci::Math::LinLeastSq::NonPeakInd(const arma::vec &spectrum, const double dev)
{
    using namespace arma;
    arma::vec SUM = spectrum + dev * arma::ones(spectrum.n_rows);
    return arma::find(spectrum <= SUM);
}

///
/// \brief Vespucci::Math::LinLeastSq::CalcPoly Calculate the values of a polynomial
/// \param coefs The polynomial coefficients ordered from 0th order to nth order
/// \param x The values for which the polynomial is to be calculated
/// \return The calculated values
///
arma::vec Vespucci::Math::LinLeastSq::CalcPoly(const arma::vec &coefs, const arma::vec &x)
{
    arma::vec y = coefs(0) + x*coefs(1); //0th and 1st power of x
    //this loop only used for powers where pow(x, power) needs to be calculated
    if (coefs.n_rows > 1){
        for (arma::uword i = 2; i < coefs.n_rows; ++i)
            y += coefs(i) * arma::pow(x, i);
    }
    return y;

}

///
/// \brief Vespucci::Math::LinLeastSq::OrdinaryLeastSquares Perform Ordinary Least Squares
/// \param X The design matrix of the regression
/// \param y The response vector
/// \return
///
arma::mat Vespucci::Math::LinLeastSq::OrdinaryLeastSquares(const arma::mat &X, const arma::mat &y)
{
    arma::mat Q, R;
    arma::qr(Q, R, X);
    return arma::solve(R, Q.t()) * y;
}

///
/// \brief Vespucci::Math::LinLeastSq::Vandermonde Build a Vandermonde matrix for OLS
/// \param x A vector containing a signal
/// \param poly_order The polynomial order
/// \return A Vandermonde matrix of x for the polynomial of order poly_order
///
arma::mat Vespucci::Math::LinLeastSq::Vandermonde(const arma::vec &x, const int poly_order)
{
    arma::mat X(x.n_rows, poly_order + 1);
    X.col(0) = arma::ones(x.n_rows); //faster than pow(X, 0)
    X.col(1) = x;
    for (arma::uword i = 2; i < X.n_cols; ++i)
        X.col(i) = arma::pow(x, i);
    return X;
}

///
/// \brief Vespucci::Math::LinLeastSq::CalcErr Calculate the error criterion
/// \param dev
/// \param prev_dev
/// \return
///
double Vespucci::Math::LinLeastSq::CalcErr(const double &dev, const double &prev_dev)
{
    return std::abs( (dev - prev_dev) / dev );
}


arma::vec Vespucci::Math::LinLeastSq::OrdinaryLeastSquares(const arma::mat &X,
                                                           const arma::vec &y,
                                                           arma::vec &fit,
                                                           std::map<std::string, double> stats)
{
    double n = X.n_rows;
    double dof = n - 2; //for linear fit
    double y_bar = mean(y);

    arma::vec coefs = OrdinaryLeastSquares(X, y);
    fit = Vespucci::Math::LinLeastSq::CalcPoly(coefs, X.col(1));

    arma::vec residuals = y - fit;

    arma::vec centered = y - y_bar;
    double residual_sumsq = arma::as_scalar(arma::sum(arma::pow(residuals, 2.0)));
    double total_sumsq = arma::as_scalar(arma::sum(arma::pow(centered, 2.0)));
    double regression_sumsq = arma::as_scalar(arma::sum(arma::pow((fit - y_bar), 2.0)));
    double R_squared = 1.0 - (residual_sumsq/total_sumsq);
    double adj_R_squared = 1 - (1 - R_squared)*(n - 1)/dof; //p==1 for deg-1 polynomial

    arma::mat var_hat = (residual_sumsq / dof) * inv(X.t() * X);

    std::string param_name;
    for(arma::uword i = 0; i < coefs.n_rows; ++i){
        param_name = "a" + std::to_string(i);
        stats[param_name] = coefs(i);
        stats["s_" + param_name] = var_hat(i,i);
    }
    stats["R squared"] = R_squared;
    stats["Adjusted R squared"] = adj_R_squared;
    stats["s(y)"] = std::sqrt(regression_sumsq / n);
    stats["F"] =(regression_sumsq) / (residual_sumsq/dof);
    stats["Degrees of Freedom"] = dof;
    stats["SSreg"] = regression_sumsq;
    stats["SSres"] = residual_sumsq;
    stats["Norm of residuals"] = std::sqrt(residual_sumsq);
    return coefs;
}

/*
arma::mat Vespucci::Math::LinLeastSq::OrdinaryLeastSquares(const arma::mat &X, const arma::mat &y, arma::mat &coef_errors, arma::mat &predicted)
{

}
*/
