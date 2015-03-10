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
#include <Math/Smoothing/smoothing.h>

///
/// \brief Vespucci::Math::Smoothing::MedianFilter
/// \param X
/// \param window_size
/// \return
/// Performs median filtering on a signal X. This just ignores the edges becuause
/// they will be fairly small and not very interesting.
arma::vec Vespucci::Math::Smoothing::MedianFilter(arma::vec X, arma::uword window_size)
{
    arma::uword k = (window_size - 1) / 2;

    arma::vec filtered = X; //copy the whole thing, then add in the bit we actually filter
    arma::vec buffer;
    arma::uvec sorted;
    //The armadillo median function results in a crash on some mingw compilers
    //This method might not be as fast, but it always works.
    try{
        //sort the window then pick the middle value
        for (arma::uword i = k; i < (X.n_rows - k); ++i){
            buffer = X.subarma::vec(i-k, i+k);
            sorted = stable_sort_index(buffer);
            filtered(i) = buffer(sorted(k));
            //filtered(i) = median(buffer);
        }

    }catch(std::exception e){
        cout << e.what();
    }

    return filtered;
}

///
/// \brief Vespucci::Math::Smoothing::MedianFilterMat
/// \param X
/// \param window_size
/// \return
/// Performs Median Filter over a arma::matrix with spectra in columns
arma::mat Vespucci::Math::Smoothing::MedianFilterMat(arma::mat X, arma::uword window_size)
{
    arma::mat filtered;
    filtered.set_size(X.n_rows, X.n_cols);
    for(arma::uword i = 0; i < X.n_cols; ++i)
        filtered.col(i) = MedianFilter(X.col(i), window_size);
    return filtered;
}
