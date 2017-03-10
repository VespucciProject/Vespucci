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
#ifndef UNIVARIATEDATA_H
#define UNIVARIATEDATA_H
#include "Global/enums.h"
#include "Math/Quantification/quantification.h"
#include "Data/Analysis/analysisresults.h"
class UnivariateData: public AnalysisResults
{
public:
    UnivariateData(QString name);
    UnivariateData(QString name, vec control);

    void Apply(double left_bound, double right_bound,
               uword bound_window, const mat &spectra, const vec &abscissa);
    void Apply(QString peak_shape,
               double left_bound,
               double right_bound,
               const mat &spectra,
               const vec &abscissa);
    void Apply(double first_left_bound, double first_right_bound,
               double second_left_bound, double second_right_bound,
               uword bound_window, const mat &spectra, const vec &abscissa);
    void ApplyStatistics(double left_bound, double right_bound,
                         const mat &spectra, const vec &abscissa);
    void ApplyCorrelation(const mat &spectra, const vec &control);
    void Calibrate(const vec &x, const vec &y, uword column);

};

#endif // UNIVARIATEDATA_H
