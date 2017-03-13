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
#ifndef FFT_H
#define FFT_H
#if !defined( SWIG )
    // SWIG should not see #inlcude<armadillo> as it can not handle it
    #include "Math/VespucciMath.h"
    #include "libvespucci.h"
#endif
namespace Vespucci{
    namespace Math{
        namespace Transform{
            VESPUCCI_EXPORT void fft_mat(const arma::mat &t_signal,
                                        const arma::vec &t_abscissa,
                                        arma::cx_mat &f_signal,
                                        arma::vec &f_abscissa,
                                        arma::uword n);
            VESPUCCI_EXPORT void ifft_mat(const arma::cx_mat &f_signal,
                                        const arma::vec f_abscissa,
                                        arma::cx_mat &t_signal,
                                        arma::vec &t_abscissa,
                                        arma::uword n);
            VESPUCCI_EXPORT arma::mat ApplyWeights(const arma::mat &signal,
                                                   const arma::vec &abscissa,
                                                   const std::string &weight,
                                                   const double &param);
            VESPUCCI_EXPORT arma::mat ApplySBWeights(const arma::mat &signal,
                                                     const arma::vec &abscissa,
                                                     const double &starting_offset,
                                                     const double &ending_offset,
                                                     const double &power);
    }
}
}
#endif // FFT_H
