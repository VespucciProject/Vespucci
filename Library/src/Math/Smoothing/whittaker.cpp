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
#include <Math/Smoothing/whittaker.h>
#include <Math/Accessory/accessory.h>

///
/// \brief Vespucci::Math::WhittakerSmooth
/// \param x
/// \param lambda
/// \param penalty_order
/// \return
/// This may be re-written to use a sparse Cholesky decomposition if such a function
/// comes to exist in armadillo or MLPACK
arma::vec Vespucci::Math::Smoothing::WhittakerSmooth(const arma::vec &x, double lambda, arma::uword penalty_order)
{

    arma::uword m = x.n_elem;
    arma::mat E = arma::eye(m, m);
    arma::mat D = Vespucci::Math::diff(E, penalty_order);
    arma::vec filtered = solve(E + lambda*D.t()*D, x);

    //This is a version that would work if chol() could take a sparse input.
    /*
    arma::uword m = x.n_elem;
    arma::sp_mat E = speye(m, m);
    arma::sp_mat D = Vespucci::Math::diff(E, penalty_order);
    arma::sp_mat F = E + lambda*D.t()*D;
    arma::mat C = chol(F);
    arma::vec z = solve(C, solve(C, x));
    */
    return filtered;
}
