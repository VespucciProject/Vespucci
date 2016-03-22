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
#ifndef UNIVARIATEDATA_H
#define UNIVARIATEDATA_H
#include "Global/enums.h"
#include "Data/Dataset/vespuccidataset.h"
#include "Math/Quantification/quantification.h"
class AnalysisResults;
class UnivariateData
{
public:
    UnivariateData(QString name);
    UnivariateData(QString name, vec control);

    void Apply(double left_bound, double right_bound, uword bound_window, const mat &spectra, const vec &abscissa);
    void Apply(double first_left_bound, double first_right_bound,
               double second_left_bound, double second_right_bound, uword bound_window, const mat &spectra, const vec &abscissa);
    void ApplyCorrelation(const mat &spectra);

    void Calibrate(const vec &values, const vec &concentrations);

    mat results() const;
    const mat *results_ptr() const;
    bool band_ratio() const;
    double left_bound() const;
    double right_bound() const;
    double first_left_bound() const;
    double first_right_bound() const;
    double second_left_bound() const;
    double second_right_bound() const;
    QString MethodDescription() const;

    mat calibration_curve() const;
    const mat *calibration_curve_ptr() const;
    const std::map<std::string, double> calibration_stats() const;


    uvec Boundaries() const;
    uvec MidlineBoundaries() const;
    mat first_baselines() const;
    mat second_baselines() const;

    QString name() const;
    void SetName(QString name);

    bool calibrated() const;

    field<mat> CalculateRatios();

    QSharedPointer<AnalysisResults> GetResults();

private:
    QString method_description_;

    mat first_results_; //matrix with 8 or 16 columns and spectra count rows, depending on type
    mat second_results_;


    ///
    /// \brief control_
    /// Control correlation is calculated for.
    vec control_;

    ///
    /// \brief band_ratio_
    /// Whether or not this is a band ratio map
    bool band_ratio_;

    ///
    /// \brief correlation_
    /// Whether or not this is a correlation map
    bool correlation_;

    //only first_ ones used when not band ratio
    double first_left_bound_;
    double first_right_bound_;
    double second_left_bound_;
    double second_right_bound_;

    uword bound_window_;
    mat first_inflection_points_;
    mat second_inflection_points_;
    mat first_baselines_;
    mat second_baselines_;
    field<mat> inflection_first_baselines_;
    field<mat> inflection_second_baselines_;

    QString name_;

    uvec boundaries_;
    mat calibration_curve_;

    std::map<std::string, double> calibration_stats_;
    bool calibrated_;
};

#endif // UNIVARIATEDATA_H
