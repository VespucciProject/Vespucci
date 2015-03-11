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

#include <Math/Transform/cwt.h>
#include <Math/PeakFinding/peakfinding.h>
///
/// \brief Vespucci::Math::Transform::cwt
/// \param X
/// \param wavelet
/// \param scales A vector of arma::uwords containing the scales to calculate
/// \return A arma::matrix containing vectors of CWT coefficients as columns (this is
/// the transpose of the result produced by arma::matlab/octave and Zhang's R code).
///
/// Calculate the CWT coefficients for the scales in scales for the vector X.
/// This code is translated from the R package baselineWavelet by Zhang.

/// Zhang, Z.-M., Chen, S., Liang, Y.-Z., Liu, Z.-X., Zhang, Q.-M., Ding, L.-X.,
/// Ye, F. and Zhou, H. (2010),
/// An intelligent background-correction algorithm for highly fluorescent samples
/// in Raman spectroscopy.
/// J. Raman Spectrosc., 41: 659–669. doi: 10.1002/jrs.2500
arma::mat Vespucci::Math::Transform::cwt(arma::vec X, std::string wavelet, arma::uvec scales)
{
    arma::mat wcoeffs(X.n_rows, scales.n_elem);
    arma::vec psi_xval(1024);
    arma::vec psi(1024);


    //calculate the wavelet:
    if (wavelet == "mexh"){
        psi_xval = arma::linspace(-8, 8, 1024);
        psi = (2/std::sqrt(3.0) * std::pow(arma::datum::pi, -0.25)) * (arma::ones(1024) - arma::pow(psi_xval, 2)) % arma::exp(-arma::pow(psi_xval, 2)/2);
    }
    else if (wavelet == "haar"){
        psi_xval = arma::linspace(0, 1, 1024);
        psi(0) = 0;
        psi(1023) = 0;
        psi.rows(1, 511) = arma::ones(511);
        psi.rows(512, 1022) = -1*arma::ones(511);
    }

    psi_xval = psi_xval - arma::ones(psi_xval.n_elem)*psi_xval(0);
    double dxval = psi_xval(1);
    double xmax = psi_xval(psi_xval.n_elem - 1);

    arma::vec f, j, w;
    arma::uvec j_u;
    arma::uword i, scale, next_p2, vector_size;
    arma::cx_vec X_hat, f_hat, schur_product;

    //calculate coefficents for all scales
    for (i = 0; i < scales.n_elem; ++i){
        scale = scales(i);
        f = arma::zeros(X.n_elem);
        j = arma::floor(arma::linspace(0, scale*xmax - 1, scale*xmax)/(scale*dxval)) + arma::ones(scale*xmax);
        j_u.set_size(j.n_elem);
        for (arma::uword k = 0; k < j_u.n_elem; ++k){
            j_u(k) = j(k);
        }

        f.rows(0, j.n_elem-1) = flipud(psi.elem(j_u)) - mean(psi.elem(j_u));
        if (f.n_rows != X.n_rows){
            std::cerr << "scale too large!" << std::endl;
        }
        next_p2 = Vespucci::Math::NextPow(X.n_elem, 2);
        vector_size = std::pow(2, next_p2);
        X_hat = fft(X, vector_size);
        f_hat = fft(f, vector_size);
        schur_product = X_hat % f_hat;
        w = arma::real( arma::ifft(schur_product, vector_size) );

        //if signal had to be padded, remove padding
        if (w.n_rows > X.n_rows){
            w.shed_rows(X.n_elem, w.n_rows - 1); //remove zero padding
        }

        wcoeffs.col(i) = w * (1/std::sqrt(scale));
    }

    return wcoeffs; //return real part of the ifft

}






arma::vec Vespucci::Math::Transform::cwt_spdbc(arma::vec X, std::string wavelet, arma::uword qscale, double threshold, std::string threshold_method, arma::uword window_size, arma::umat &peak_extrema, arma::vec &baseline)
{
    arma::umat peaks;
    arma::vec peak_magnitudes;
    arma::uvec scales(1);
    scales(0) = qscale;
    try{
        arma::vec X_transform = Vespucci::Math::Transform::cwt(X, wavelet, scales);
        arma::vec dX_transform = Vespucci::Math::diff(X, 1);
        dX_transform.insert_rows(0, 1, true); //buffer so that X and dX have same
        //number of elements and dX(i) is the derivative of X at i.
        peak_extrema = Vespucci::Math::PeakFinding::FindPeakPositions(X_transform, dX_transform,
                                                   threshold,
                                                   threshold_method,
                                                   peak_magnitudes);
        baseline = Vespucci::Math::PeakFinding::EstimateBaseline(X, peak_extrema, window_size);
    }catch(std::exception e){
        std::cerr << std::endl << "exception! cwt_spdbc" << std::endl;
        std::cerr << e.what();
        throw(e);
    }

    return X - baseline;

}


arma::mat Vespucci::Math::Transform::cwt_spdbc_mat(arma::mat X, std::string wavelet, arma::uword qscale,
                            double threshold, std::string threshold_method,
                            arma::uword window_size, arma::field<arma::umat> &peak_positions,
                            arma::mat &baselines)
{
    baselines.set_size(X.n_rows, X.n_cols);
    arma::vec baseline;
    arma::vec spectrum;
    arma::vec current_corrected;
    arma::mat corrected(X.n_rows, X.n_cols);
    arma::umat current_peakpos;
    peak_positions.set_size(X.n_cols);
    arma::uword i;
    try{
        for (i = 0; i < X.n_cols; ++i){
            spectrum = X.col(i);
            current_corrected = Vespucci::Math::Transform::cwt_spdbc(spectrum, wavelet,
                                                   qscale, threshold,
                                                   threshold_method, window_size,
                                                   current_peakpos, baseline);

            peak_positions(i) = current_peakpos;
            baselines.col(i) = baseline;
            corrected.col(i) = current_corrected;
        }
    }catch(std::exception e){
        std::cerr << std::endl << "exception! cwt_spdbc_mat" << std::endl;
        std::cerr << "i = " << i << std::endl;
        std::cerr << e.what();
        throw(e);
    }

    return corrected;
}




arma::mat Vespucci::Math::Transform::cwtPeakAnalysis(arma::mat X,
                              std::string wavelet, arma::uword qscale,
                              double threshold, std::string threshold_method,
                              arma::mat &transform)
{
    transform.set_size(X.n_rows, X.n_cols);
    arma::uvec scales(1);
    scales(0) = qscale;
    arma::uword i;
    arma::umat peak_positions;
    arma::mat peak_extrema(X.n_rows, X.n_cols);
    arma::vec spectrum, current_transform, dtransform, peak_magnitudes;
    try{
        for (i = 0; i < X.n_cols; ++i){
            spectrum = X.col(i);
            current_transform = Vespucci::Math::Transform::cwt(spectrum, wavelet, scales);
            transform.col(i) = current_transform;
            dtransform = Vespucci::Math::diff(transform, 1);
            dtransform.insert_rows(0, 1, true); //dtransform(i) is derivative at transform(i)
            peak_positions = Vespucci::Math::PeakFinding::FindPeakPositions(transform, dtransform,
                                                         threshold, threshold_method,
                                                         peak_magnitudes);
            peak_extrema.col(i) = Vespucci::Math::PeakFinding::PeakExtrema(X.n_elem, peak_positions);
        }
    }
    catch(std::exception e){
        std::cerr << "CWTPeakAnalysis" << std::endl;
        std::cerr << "i = " << i << std::endl;
        throw(e);
    }
    return peak_extrema;
}
///
/// \brief Vespucci::Math::Transform::cwt_multi
/// \param X
/// \param wavelet
/// \param scales
/// \return
/// This may be deprecated by putting this loop in cwt
arma::field<arma::mat> Vespucci::Math::Transform::cwt_multi(arma::mat X, std::string wavelet, arma::uvec scales)
{
    arma::field<arma::mat> results(X.n_cols);
    for (arma::uword i = 0; i < results.n_elem; ++i){
        results(i) = Vespucci::Math::Transform::cwt(X.col(i), wavelet, scales);
    }
    return results;
}


