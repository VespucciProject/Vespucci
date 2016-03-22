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
//create mock peaks using various gaussian, lorentzian and pseudo-voigt functions.
//assert that the calculated areas and bandwidths are equivalent
#include <Math/Quantification/quantification.h>
#include <boost/test/unit_test.hpp>
#include "test.h"
using namespace std;
using namespace arma;
BOOST_AUTO_TEST_CASE(IntensityTest)
{
    vec x = linspace(-5, 5, 501);
    double sigma = 1.0;
    double fwhm, area;
    vec y = VespucciTest::CreateGaussianPeak(x, sigma, fwhm, area);
    double known_max = y(250);
    uword position;
    double test_max = Vespucci::Math::Quantification::FindPeakMax(y, 200, 400, position);
    BOOST_REQUIRE_EQUAL(position, 250);
    BOOST_REQUIRE_CLOSE(test_max, known_max, 1e-5);
}

BOOST_AUTO_TEST_CASE(AreaTest)
{
    vec x = linspace(-5, 5, 501);
    double sigma = 1.0;
    double fwhm, area;
    vec y = VespucciTest::CreateGaussianPeak(x, sigma, fwhm, area);
    double abscissa_step = std::abs(x(1) - x(0));
    uword position;
    vec baseline;
    double test_area = Vespucci::Math::Quantification::IntegratePeak(y, 0, 501, abscissa_step, baseline);
    BOOST_REQUIRE_CLOSE(area, test_area, 1e-5);
}

BOOST_AUTO_TEST_CASE(BandwidthTest)
{
    vec x = linspace(-5, 5, 501);
    double sigma = 1.0;
    double fwhm, area;
    vec y = VespucciTest::CreateGaussianPeak(x, sigma, fwhm, area);
    double abscissa_step = std::abs(x(1) - x(0));
    uword position;
    vec baseline, midline;
    double test_fwhm = Vespucci::Math::Quantification::FindBandwidth(y, 200, 400, midline, baseline, abscissa_step);
    BOOST_REQUIRE_CLOSE(fwhm, test_fwhm, 1e-5);
}

