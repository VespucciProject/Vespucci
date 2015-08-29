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
#ifndef MAXIMUM_H
#define MAXIMUM_H
#include <Math/Accessory/accessory.h>
#include "Global/libvespucci.h"

namespace Vespucci
{
    namespace Math
    {
    /// \namespace Vespucci::Math::Quantification
    /// \brief A namespace for math functions relating to peak quantification
    ///
    ///
        namespace Quantification
        {
            //Peak determination
            VESPUCCI_EXPORT double FindPeakMax(const arma::vec &X, arma::uword min_index, arma::uword max_index, arma::uword &position);
            VESPUCCI_EXPORT arma::vec FindPeakMaxMat(const arma::mat &X, arma::vec abscissa, double &min, double &max, arma::vec &positions);
            VESPUCCI_EXPORT arma::mat FindPeakMaxesMat(const arma::mat &X, arma::vec abscissa,
                                 double &first_min, double &first_max,
                                 double &second_min, double &second_max,
                                 arma::mat positions);
        }

    }
}
#endif // MAXIMUM_H
