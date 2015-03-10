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

#include <Math/Quantification/quantification.h>
arma::vec Vespucci::Math::Quantification::CorrelationMat(arma::mat X, arma::vec control)
{
    arma::vec results;
    results.set_size(X.n_cols);
    for(arma::uword i = 0; i < X.n_cols; ++i){
        results(i) = as_scalar(cor(control, X.col(i)));
    }
    return results;
}


