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
#ifndef FFT_H
#define FFT_H
#include "Math/VespucciMath.h"
#include "Global/libvespucci.h"
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
        }
    }
}

#endif // FFT_H
