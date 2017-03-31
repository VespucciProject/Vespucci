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

#ifndef ENUMS_H
#define ENUMS_H

namespace UnivariateMethod{
///
/// \brief The Method enum
/// Univariate methods
    enum Method{Intensity, Area, FWHM, AreaRatio, IntensityRatio, SignalNoise, Correlation, Derivative, DerivativeRatio};
}

namespace MetaMethod{
    enum Method{AverageSpectra, ConcatenateDatasets, StitchDatasets};
}

#endif // ENUMS_H
