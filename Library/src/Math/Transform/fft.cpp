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
#include <Math/Transform/fft.h>
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
    //we transform dt into df
    arma::vec df = Vespucci::Math::diff(t_abscissa, 1);
    arma::rowvec new_row = {df(0)};
    df.insert_rows(0, new_row);

    //df = 1.0/(n*dt)
    df = n*df;
    df.transform([](double val){return 1.0/val;});

    //abs(k) = k*df
    f_abscissa = arma::linspace(1, df.n_rows+1, df.n_rows) % df;

    //truncate or extend abscissa so it has size n
    if (f_abscissa.n_rows > n)
        f_abscissa = f_abscissa.rows(0, n-1);
    if (f_abscissa.n_rows < n){
        double last_df = df(df.n_rows - 1);
        double last_abs = f_abscissa(f_abscissa.n_rows - 1);
        arma::uword rows_to_add = n - f_abscissa.n_rows;
        arma::vec abs_end = arma::linspace(last_abs + last_df,
                                           last_abs + (rows_to_add*last_df),
                                           rows_to_add);
        f_abscissa = arma::join_vert(f_abscissa, abs_end);
    }


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

    //we transform df into dt
    arma::vec dt = Vespucci::Math::diff(f_abscissa, 1); //==df
    arma::rowvec new_row = {dt(0)}; //insert so we have dt for the first value
    dt.insert_rows(0, new_row);

    //dt = 1.0/(n*df)
    dt = n * dt;
    dt.transform([](double val){return 1.0/val;});

    //abs(k) = k*dt
    t_abscissa = arma::linspace(1, dt.n_rows+1, dt.n_rows) % dt;



    //truncate or pad abscissa to match size of spectra
    if (t_abscissa.n_rows > n)
        t_abscissa = t_abscissa.rows(0, n-1);
    if (t_abscissa.n_rows < n){
        double last_dt = dt(dt.n_rows - 1);
        double last_abs = t_abscissa(t_abscissa.n_rows - 1);
        arma::uword rows_to_add = n - t_abscissa.n_rows;
        arma::vec abs_end = arma::linspace(last_abs + last_dt,
                                           last_abs + (rows_to_add*last_dt),
                                           rows_to_add);
        t_abscissa = arma::join_vert(t_abscissa, abs_end);
    }
    t_signal = arma::ifft(f_signal, n);



}

///
/// \brief Vespucci::Math::Transform::ApplyWeights
/// \param signal
/// \param abscissa
/// \param weight
/// \param param
/// \return
/// Apply weights to the time-domain signal signal
arma::mat Vespucci::Math::Transform::ApplyWeights(const arma::mat &signal,
                                                  const arma::vec &abscissa,
                                                  const std::string &weight,
                                                  const double &param)
{
    arma::vec weights;
    if (weight == "gaus"){
        double coef = -1 * std::pow(param, 2.0);
        weights = (coef * arma::pow(abscissa, 2.0)) / 2;
    }
    else if (weight == "exp"){
        weights = arma::exp(-1*param*abscissa);
    }
    else{
        throw std::runtime_error("Unsupported value of weight");
    }
    arma::mat weighted_signal(signal.n_rows, signal.n_cols);
    for (arma::uword i = 0; i < signal.n_cols; ++i)
        weighted_signal.col(i) = signal.col(i) % weights;
    return weighted_signal;
}

///
/// \brief Vespucci::Math::Transform::ApplyWeights
/// \param signal
/// \param abscissa
/// \param weight
/// \param param
/// \param cut_off
/// \return
///
arma::mat Vespucci::Math::Transform::ApplySBWeights(const arma::mat &signal,
                                                    const arma::vec &abscissa,
                                                    const double &starting_offset,
                                                    const double &ending_offset,
                                                    const double &power)
{
    arma::vec weights;

    //what is in front of the equation (one over the denominator)
    double out_coef = 1.0/(abscissa.n_rows - 1);

    //what the time is multiplied by
    double coef = arma::datum::pi * (starting_offset - ending_offset);

    //what is added to coef*time
    arma::vec offset = arma::datum::pi * starting_offset * arma::ones(abscissa.n_rows);
    weights = arma::pow(arma::sin(out_coef*(offset+(coef*abscissa))), power);

    arma::mat weighted_signal(signal.n_rows, signal.n_cols);
    for (arma::uword i = 0; i < signal.n_cols; ++i)
        weighted_signal.col(i) = signal.col(i) % weights;
    return weighted_signal;
}
