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
#ifndef DIMENSION_REDUCTION_H
#define DIMENSION_REDUCTION_H
#if !defined( SWIG )
    // SWIG should not see #inlcude<armadillo> as it can not handle it
    #include <mlpack/core.hpp>
    #include "libvespucci.h"
#endif
namespace Vespucci
{
    namespace Math
    {
        /// \namespace Vespucci::Math::DimensionReduction
        /// \brief A namespace for math functions relating to dimension reduction.
        ///
        ///
        namespace DimensionReduction
        {
            VESPUCCI_EXPORT bool plsregress(arma::mat X,
                                            arma::mat Y,
                                            int components,
                                            arma::mat &X_loadings,
                                            arma::mat &Y_loadings,
                                            arma::mat &X_scores,
                                            arma::mat &Y_scores,
                                            arma::mat &coefficients,
                                            arma::mat &percent_variance,
                                            arma::mat &fitted);
            VESPUCCI_EXPORT bool VCA(const arma::mat &R,
                                     arma::uword p,
                                     arma::uvec &indices,
                                     arma::mat &endmember_spectra,
                                     arma::mat &projected_data,
                                     arma::mat &fractional_abundances);
            VESPUCCI_EXPORT double estimate_snr(const arma::mat &R,
                                                arma::vec r_m,
                                                arma::mat x);
            VESPUCCI_EXPORT size_t HySime(const arma::mat &y,
                                          const arma::mat &n,
                                          arma::mat &Rn,
                                          arma::mat &Ek);
            VESPUCCI_EXPORT size_t HySime(const arma::mat &y,
                                          arma::mat &subspace);
            VESPUCCI_EXPORT void EstimateAdditiveNoise(arma::mat &noise,
                                                       arma::mat &noise_correlation,
                                                       const arma::mat &sample);
            VESPUCCI_EXPORT bool svds(const arma::mat &X,
                                      arma::uword k,
                                      arma::mat &U,
                                      arma::vec &s,
                                      arma::mat &V);
        }
    }
}
#endif //DIMENSION_REDUCTION_H
