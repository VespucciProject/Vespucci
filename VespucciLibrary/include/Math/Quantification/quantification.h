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
#ifndef QUANTIFICATION_H
#define QUANTIFICATION_H
#include <Math/Accessory/accessory.h>
#include "Global/libvespucci.h"
#include "Math/Quantification/bandwidth.h"
#include "Math/Quantification/correlation.h"
#include "Math/Quantification/integration.h"
#include "Math/Quantification/maximum.h"

namespace Vespucci{
    namespace Math{
        namespace Quantification{
            VESPUCCI_EXPORT arma::rowvec QuantifyPeak(const arma::vec &spectrum, const arma::vec &abscissa, double &min, double &max, arma::uword bound_window, arma::mat &total_baseline, arma::mat &inflection_baseline);
            VESPUCCI_EXPORT arma::mat QuantifyPeakMat(const arma::mat &spectra, const arma::vec &abscissa, double &min, double &max, arma::uword bound_window, arma::mat &total_baselines, arma::field<arma::mat> &inflection_baselines);
        }
    }
}


#endif //QUANTIFICATION_H
