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
#ifndef NORMALIZATION_H
#define NORMALIZATION_H
#include <Math/Accessory/accessory.h>

namespace Vespucci
{
    namespace Math
    {
        namespace Normalization
        {
            arma::vec StandardScore(const arma::vec &X);
            arma::mat StandardScoreMat(const arma::mat &X);

            arma::mat SNVNorm(const arma::mat &X, const double offset);
        }
    }
}
#endif //NORMALIZATION_H
