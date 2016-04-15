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
#ifndef CONFIDENCEINTERVAL_H
#define CONFIDENCEINTERVAL_H
namespace Vespucci{
    namespace Math{
        namespace Stats{
            double ZInterval(double alpha, double stddev, unsigned int n);
            double TInterval(double alpha, double stddev, unsigned int n);

            double ZTest(double alpha, int tail,
                         double mean1, double stddev1, unsigned int n1,
                         double mean2, double stddev2, unsigned int n2);

            double TTest(double alpha, int tail,
                         double mean1, double stddev1, unsigned int n1,
                         double mean2, double stddev2, unsigned int n2);

            double StdNormDist(double p);
            double StdNormDistInv(double z);
            double NormDist(double x, double mean, double stddev, bool cumulative);
            double NormDistInv(double p, double mean, double stddev);
            double TDist(double x, unsigned int df, bool cumulative);
            double TDistInv(double p, unsigned int df);
            double ChiSquaredDist(double x, int df, bool cumulative);
            double FDist(double x, double df1, double df2, bool cumulative);
        }
    }
}
#endif // CONFIDENCEINTERVAL_H
