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
#ifndef INTEGRATION_H
#define INTEGRATION_H
#if !defined( SWIG )
    // SWIG should not see #inlcude<armadillo> as it can not handle it
    #include "Math/VespucciMath.h"
    #include "libvespucci.h"
#endif
namespace Vespucci{
    namespace Math{
        namespace Quantification{
        VESPUCCI_EXPORT double IntegratePeak(const arma::vec &X, arma::uword min_index, arma::uword max_index, double abscissa_step, arma::vec &baseline, bool correct_baseline);
        VESPUCCI_EXPORT arma::vec IntegratePeakMat(const arma::mat &X, const arma::vec &abscissa, double &min, double &max, arma::mat &baselines, arma::uvec &boundaries, bool correct_baseline);

        VESPUCCI_EXPORT arma::mat IntegratePeaksMat(const arma::mat &X, const arma::vec &abscissa,
                              double &first_min, double &first_max,
                              double &second_min, double &second_max,
                              arma::mat &first_baselines, arma::mat &second_baselines, arma::uvec &boundaries);

        VESPUCCI_EXPORT arma::vec IntegratePeakMat(const arma::mat &X, const arma::vec &abscissa,
                                                   double &min, double &max,
                                                   arma::field<arma::vec> &baselines,
                                                   arma::uvec &boundaries, arma::uword bound_window, bool correct_baseline);


        VESPUCCI_EXPORT arma::mat IntegratePeaksMat(const arma::mat &X, const arma::vec &abscissa,
                                                    double &first_min, double &first_max,
                                                    double &second_min, double &second_max,
                                                    arma::field<arma::vec> &first_baselines, arma::field<arma::vec> &second_baselines,
                                                    arma::uvec &boundaries, arma::uword bound_window);

        }
    }
}
#endif // INTEGRATION_H
