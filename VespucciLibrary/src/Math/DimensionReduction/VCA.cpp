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
#include "Math/DimensionReduction/dimensionreduction.h"
#include "Math/Accessory/accessory.h"


///
/// \brief Vespucci::Math::DimensionReduction::VCA
/// Vertex Component Analysis
/// \param R The dataset
/// \param endmembers Number of endmembers to compute
/// \param indices Row indices of pure components.
/// \param endmember_spectra Spectra of pure components (note that these are in
/// columns, not rows as in spectra_)
/// \param projected_data Projected data
/// \param fractional_abundances Purity of a given spectrum relative to endmember
/// \return Convergeance (no actual test implemented...)
///
bool Vespucci::Math::DimensionReduction::VCA(const arma::mat &R, arma::uword p,
         arma::uvec &indices, arma::mat &endmember_spectra,
         arma::mat &projected_data, arma::mat &fractional_abundances)
{
//Initializations
    arma::uword L = R.n_rows;
    arma::uword N = R.n_cols;
    if (L == 0 || N == 0){
        std::cerr << "No data!" << std::endl;
        return false;
    }

    if (p > L){
        std::cerr << "wrong number of endmembers (" << p << ")!"<< std::endl;
        std::cerr << "set to 5 or one less than number of spectra" << std::endl;
        p = (L < 5? 5: L-1);
    }
//mat of SNR
    arma::mat r_m = mean(R, 1);
    arma::mat R_m = arma::repmat(r_m, 1, N); //the mean of each spectral band
    arma::mat R_o = R - R_m; //mean-center the data
    arma::mat Ud;
    arma::vec Sd;
    arma::mat Vd;
    //arma::svds(Ud, Sd, Vd, arma::sp_mat(R_o * R_o.t()/N), p);
    Vespucci::Math::DimensionReduction::svds(R_o*R_o.t()/N, p, Ud, Sd, Vd);
    arma::mat x_p;
    try{
    x_p = Ud.t() * R_o;
    }catch(std::exception e){
        std::cout << "Ud.t() * R_o" << std::endl;
    }

    double SNR = Vespucci::Math::DimensionReduction::estimate_snr(R, r_m, x_p);
    double SNR_th = 15 + 10*log10(p);

//Choose projective projection or projection to p-1 subspace
    arma::mat y;
    if (SNR < SNR_th){
        arma::uword d = p - 1;
        Ud = Ud.cols(0, d-1);
        projected_data = Ud * x_p.rows(0, d-1) + R_m; //in dimension L
        arma::mat x = x_p.rows(0, d-1);//x_p = trans(Ud)*R_o, p-dimensional subspace
        //following three lines are one in arma::matlab...
        arma::mat sum_squares = sum(pow(x, 2));
        double c = sum_squares.max();
        c = std::sqrt(c);
        y = arma::join_vert(x, c*arma::ones(1, N));
      }
    else{
        arma::uword d = p;
        Vespucci::Math::DimensionReduction::svds(R*R.t()/N, p, Ud, Sd, Vd);
        arma::svds(Ud, Sd, Vd, arma::sp_mat(R*R.t()/N), d);//R_o is a mean centered version...
        x_p = Ud.t() * R;
        projected_data = Ud * x_p.rows(0, d-1);
        arma::mat x = Ud.t() * R;
        arma::mat u = arma::mean(x, 1);
        y = x / arma::repmat(sum(x % arma::repmat(u, 1, N)), d, 1);
    }


    // The VCA algorithm
    arma::vec w;
    w.set_size(p);
    arma::vec f;
    arma::rowvec v;
    indices.set_size(p);
    //there are no fill functions for arma::uvecs
    for (arma::uword i = 0; i < p; ++i)
        indices(i) = 0;

    arma::mat A = arma::zeros(p, p);
    double v_max;
    double sum_squares;
    arma::uvec q1;
    A(p-1, 0) = 1;
    for (arma::uword i = 0; i < p; ++i){
        w.randu();
        f = w - A*arma::pinv(A)*w;
        sum_squares = sqrt(sum(square(f)));
        f /= sum_squares;
        v = f.t() * y;
        v_max = arma::max(abs(v));
        q1 = arma::find(abs(v) == v_max, 1);
        indices(i) = q1(0);
        A.col(i) = y.col(indices(i)); //same as x.col(indices(i));
    }
    endmember_spectra = projected_data.cols(indices);
    fractional_abundances = arma::trans(pinv(endmember_spectra) * projected_data);
    return true;
}

///
/// \brief Vespucci::Math Estimates Signal-to-Noise ratio.
/// \cite Nascimento2005
/// \param R Input
/// \param r_m
/// \param x
/// \return
///
double Vespucci::Math::DimensionReduction::estimate_snr(const arma::mat &R, arma::vec r_m, arma::mat x)
{
    arma::uword L = R.n_rows;
    arma::uword N = R.n_cols;
    arma::uword p = x.n_rows;
    if (x.n_cols != N){
        std::cerr << "invaliid input to Estimate_snr" << std::endl;
        return 0;
    }

    double P_y = accu(arma::square(R))/N;
    double P_x = accu(arma::square(x))/N + dot(r_m, r_m);
    return 10*log10( (P_x - p/L*P_y)/(P_y - P_x) );
}

///
/// \brief Vespucci::Math::DimensionReduction::HySime
/// \param y
/// \param n
/// \param Rn
/// \param Ek
/// \return
/// Performs the HySime algorithm to predict the rank of y
int Vespucci::Math::DimensionReduction::HySime(arma::mat y,
                     arma::mat n,
                     arma::mat Rn,
                     arma::mat &Ek)
{
    std::cout << "Vespucci::Math::DimensionReduction::HySime()" << std::endl;
    if (n.n_rows != y.n_rows || n.n_cols  != y.n_cols){
        std::cerr << "HySime: Empty noise arma::matrix or its size does not agree with size of y" << std::endl;
        throw(std::runtime_error("HySime: Empty noise arma::matrix or does not agree with size of y"));
    }

    if (Rn.n_rows != Rn.n_cols || Rn.n_rows != y.n_rows){
        std::cerr << "Bad noise correlation arma::matrix" << std::endl;
        Rn = n*n.t() / y.n_cols;
    }

    arma::mat x = y - n;

    arma::mat Ry = y*y.t() / y.n_cols;
    arma::mat Rx = x*x.t() / y.n_cols;
    arma::mat E, D, V;
    arma::vec dx;
    svd(E, dx, V, Rx);
    D = arma::diagmat(dx);

    Rn = Rn + sum(Rx.diag())/y.n_rows/10000*arma::eye(y.n_rows, y.n_rows);

    arma::mat P = E.t() * Ry * E;
    arma::vec Py = P.diag();

    P = E.t() * Rn * E;
    arma::vec Pn = P.diag();

    arma::vec cost_F = -Py + 2 * Pn;
    arma::uvec negatives = find (cost_F < 0);
    int kf = negatives.n_elem;
    arma::uvec sorted_cols = sort_index(cost_F);
    Ek = E.cols(sorted_cols);
    return kf;

}

///
/// \brief Vespucci::Math::DimensionReduction::EstimateAdditiveNoise
/// \param noise
/// \param noise_correlation
/// \param sample
/// The slow additive noise mat::mator from the HySime paper. I'm looking into
/// faster ones.
void Vespucci::Math::DimensionReduction::EstimateAdditiveNoise(arma::mat &noise, arma::mat &noise_correlation, const arma::mat &sample)
{
    double little = 1e-6;
    noise = arma::zeros(sample.n_rows, sample.n_cols);
    arma::mat RR = sample * sample.t();
    arma::mat RRi = arma::inv(RR + little*arma::eye(sample.n_rows, sample.n_rows));
    arma::mat XX, RRa, beta;
    for (arma::uword i = 0; i < sample.n_rows; ++i){
        XX = RRi - (RRi.col(i) * RRi.row(i))/RRi(i, i);
        RRa = RR.col(i);
        RRa(i) = 0;
        beta = XX * RRa;
        beta(i) = 0;
        noise.row(i) = sample.row(i) - beta.t() * sample;
    }
    arma::mat nn = noise * noise.t() / sample.n_rows;
    noise_correlation = arma::diagmat(nn.diag());
}
