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
#ifndef TEST_H
#define TEST_H
#include <Math/VespucciMath.h>
using namespace std;
using namespace arma;

namespace VespucciTest{
    vec CreateMockSpectrum(const vec &abscissa, vec peak_heights, vec peak_centers, vec peak_widths);
    vec CreateGaussianPeak(const vec &x, double sigma, double &fwhm, double &area);
    //vec CreateMockSpectrum(const vec &abscissa, const vec &a, const vec &b, const vec &c)
}
#endif // TEST_H
