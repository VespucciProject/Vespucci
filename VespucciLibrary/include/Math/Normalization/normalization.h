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
#ifndef NORMALIZATION_H
#define NORMALIZATION_H

#if !defined( SWIG )
    // SWIG should not see #inlcude<armadillo> as it can not handle it
    #include "Math/VespucciMath.h"
    #include "libvespucci.h"
#endif

namespace Vespucci
{
    namespace Math
    {
    /// \namespace Vespucci::Math::Normalization
    /// \brief A namespace for math functions relating to normalization.
    ///
    ///
        namespace Normalization
        {
            VESPUCCI_EXPORT arma::vec StandardScore(const arma::vec &X);
            VESPUCCI_EXPORT arma::mat StandardScoreMat(const arma::mat &X);

            VESPUCCI_EXPORT arma::mat SNVNorm(const arma::mat &X, const double offset, bool center);
        }
    }
}
#endif //NORMALIZATION_H
