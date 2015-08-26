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
#include "Math/Transform/fft.h"
///
/// \brief Vespucci::Math::Transform::fft_mat
/// \param t_signal
/// \param t_abscissa
/// \param f_signal
/// \param f_abscissa
/// \param n
/// Perform fourier transform on the time-domain signal t_signal with size n
void Vespucci::Math::Transform::fft_mat(const arma::mat &t_signal,
                                        const arma::vec &t_abscissa,
                                        arma::cx_mat &f_signal,
                                        arma::vec &f_abscissa,
                                        arma::uword n)
{
    if (!Vespucci::Math::IsMonotonic(t_abscissa)){
        throw std::invalid_argument("Time-domain abscissa is not monotonic!");
    }

    double dt = t_abscissa(1) - t_abscissa(0);
    double df = 1 / (n * dt);
    f_abscissa = arma::linspace(1, n+1, n) * df;
    f_signal = arma::fft(t_signal, n);
}

///
/// \brief Vespucci::Math::Transform::ifft_mat
/// \param f_signal
/// \param f_abscissa
/// \param t_signal
/// \param t_abscissa
/// \param n
/// Perform the inverse fourier transform on the frequency-domain signal f_signal
void Vespucci::Math::Transform::ifft_mat(const arma::cx_mat &f_signal,
                                         const arma::vec f_abscissa,
                                         arma::cx_mat &t_signal,
                                         arma::vec &t_abscissa,
                                         arma::uword n)
{
    if (!Vespucci::Math::IsMonotonic(f_abscissa)){
        throw std::invalid_argument("Frequency-domain abscissa is not monotonic!");
    }
    double df = f_abscissa(1) - f_abscissa(0);
    double dt = 1 / (n * df);
    t_abscissa = arma::linspace(0, n+1, n) * dt;
    t_signal = arma::ifft(f_signal, n);
}
