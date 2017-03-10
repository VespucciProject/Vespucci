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
#include "Math/Smoothing/denoise.h"
#include "mlpack/methods/quic_svd/quic_svd.hpp"
arma::mat Vespucci::Math::Smoothing::SVDDenoise(const arma::mat &X, arma::uword k, arma::mat &U, arma::vec &s, arma::mat &V)
{
    arma::svds(U, s, V, arma::sp_mat(X), k);
    return -1 * U * diagmat(s) * V.t();
}

arma::mat Vespucci::Math::Smoothing::QUICSVDDenoise(const arma::mat &X, double epsilon, arma::mat &U, arma::vec &s, arma::mat &V, arma::uword &rank)
{
    arma::mat s_mat;
    mlpack::svd::QUIC_SVD(X, U, V, s_mat, epsilon, 0.1);
    rank = U.n_cols;
    s = s_mat.diag(0);
    return U * s_mat * V.t();
}
