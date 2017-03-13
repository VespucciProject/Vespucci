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


#ifndef BANDWIDTH_H
#define BANDWIDTH_H

#if !defined( SWIG )
    // SWIG should not see #inlcude<armadillo> as it can not handle it
    #include "Math/VespucciMath.h"
    #include "libvespucci.h"
#endif

namespace Vespucci{
    namespace Math{
        namespace Quantification {
            VESPUCCI_EXPORT double FindBandwidth(const arma::vec &X, arma::uword min_index, arma::uword max_index, arma::vec &midline, arma::vec &baseline, double abscissa_step);
            VESPUCCI_EXPORT arma::vec FindBandwidthMat(const arma::mat &X, arma::vec abscissa, double &min, double &max, arma::mat &midlines, arma::mat &baselines, arma::uvec &boundaries);
        }
    }
}
#endif // BANDWIDTH_H
