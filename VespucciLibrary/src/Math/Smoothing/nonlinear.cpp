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
#include <Math/Smoothing/nonlinear.h>
#include <Math/Accessory/accessory.h>

///
/// \brief Vespucci::Math::Smoothing::MedianFilter
/// \param X
/// \param window_size
/// \return
/// Performs median filtering on a signal X. This just ignores the edges becuause
/// they will be fairly small and not very interesting.
arma::vec Vespucci::Math::Smoothing::MedianFilter(const arma::vec &X, arma::uword window_size)
{
    arma::uword k = (window_size - 1) / 2;

    arma::vec filtered = X; //copy the whole thing, then add in the bit we actually filter
    arma::vec buffer;
    try{
        // middle part of spectrum
        for (arma::uword i = k; i < (X.n_rows - k); ++i){
            buffer = X.subvec(i-k, i+k);
            filtered(i) = arma::median(buffer);
        }
    }catch(std::exception e){
        std::cout << e.what();
    }

    return filtered;
}

///
/// \brief Vespucci::Math::Smoothing::MedianFilterMat
/// \param X
/// \param window_size
/// \return
/// Performs Median Filter over a arma::matrix with spectra in columns
arma::mat Vespucci::Math::Smoothing::MedianFilterMat(const arma::mat &X, arma::uword window_size)
{
    arma::mat filtered;
    filtered.set_size(X.n_rows, X.n_cols);

#ifdef _WIN32
  #pragma omp parallel for default(none) \
      shared(X, window_size, filtered)
     for (intmax_t i = 0; i < (intmax_t) X.n_cols; ++i)
#else
  #pragma omp parallel for default(none) \
      shared(X, window_size, filtered)
    for (size_t i = 0; i < X.n_cols; ++i)
#endif
    {
        filtered.col(i) = MedianFilter(X.col(i), window_size);
    }

    return filtered;
}
