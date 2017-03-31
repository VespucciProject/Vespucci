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
#ifndef CWT_H
#define CWT_H
#if !defined( SWIG )
    // SWIG should not see #inlcude<armadillo> as it can not handle it
    #include "Math/VespucciMath.h"
    #include "libvespucci.h"
#endif
namespace Vespucci
{
    namespace Math
    {
    /// \namespace Vespucci::Math::DimensionReduction
    /// \brief A namespace for math functions relating to transforms
    ///
    ///
        namespace Transform
        {
            VESPUCCI_EXPORT arma::mat cwt(arma::vec X, std::string wavelet, arma::uvec scales);
            VESPUCCI_EXPORT arma::mat cwt_mat(arma::mat X, std::string wavelet, arma::uword qscale);
            VESPUCCI_EXPORT arma::vec cwt_spdbc(const arma::vec &X, std::string wavelet, arma::uword qscale, double threshold, std::string threshold_method, arma::uword window_size, arma::umat &peak_extrema, arma::vec &baseline);
            VESPUCCI_EXPORT arma::mat cwt_spdbc_mat(const arma::mat &X, std::string wavelet, arma::uword qscale, double threshold, std::string threshold_method, arma::uword window_size, arma::field<arma::umat> &peak_positions, arma::mat &baselines);
            VESPUCCI_EXPORT arma::field<arma::mat> cwt_multi_mat(arma::mat X, std::string wavelet, arma::uvec scales);
            VESPUCCI_EXPORT arma::field<arma::mat> cwt_multi(const arma::mat &X, std::string wavelet, arma::uvec scales);
            VESPUCCI_EXPORT arma::mat cwtPeakAnalysis(const arma::mat &X,
                                      std::string wavelet, arma::uword qscale,
                                      double threshold, std::string threshold_method,
                                      arma::mat &transform);
            VESPUCCI_EXPORT arma::mat FindRidges(arma::mat coefs);
        }
    }
}
#endif //CWT_H
