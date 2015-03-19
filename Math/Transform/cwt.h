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
#ifndef CWT_H
#define CWT_H
#include <Math/Accessory/accessory.h>
namespace Vespucci
{
    namespace Math
    {
        namespace Transform
        {
            arma::mat cwt(arma::vec X, std::string wavelet, arma::uvec scales);
            arma::mat cwt_mat(arma::mat X, std::string wavelet, arma::uword qscale);
            arma::vec cwt_spdbc(arma::vec X, std::string wavelet, arma::uword qscale, double threshold, std::string threshold_method, arma::uword window_size, arma::umat &peak_extrema, arma::vec &baseline);
            arma::mat cwt_spdbc_mat(arma::mat X, std::string wavelet, arma::uword qscale, double threshold, std::string threshold_method, arma::uword window_size, arma::field<arma::umat> &peak_positions, arma::mat &baselines);
            arma::field<arma::mat> cwt_multi_mat(arma::mat X, std::string wavelet, arma::uvec scales);
            arma::field<arma::mat> cwt_multi(arma::mat X, std::string wavelet, arma::uvec scales);
            arma::mat cwtPeakAnalysis(arma::mat X,
                                      std::string wavelet, arma::uword qscale,
                                      double threshold, std::string threshold_method,
                                      arma::mat &transform);
            arma::mat FindRidges(arma::mat coefs);
        }
    }
}
#endif //CWT_H
