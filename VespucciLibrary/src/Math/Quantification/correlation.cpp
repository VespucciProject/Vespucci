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

#include <Math/Quantification/correlation.h>
arma::mat Vespucci::Math::Quantification::CorrelationMat(const arma::mat &X, const arma::mat &control)
{
    arma::mat results;
    results.set_size(X.n_cols, control.n_cols);

    for (arma::uword i = 0; i < X.n_cols; ++i)
        for (arma::uword j = 0; j < control.n_cols; ++j)
            results(i, j) = arma::as_scalar(arma::cor(control.col(i), X.col(j)));
    return results;
}


