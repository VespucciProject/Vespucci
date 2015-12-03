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
#ifndef UNIVARIATEDATA_H
#define UNIVARIATEDATA_H
#include "Global/enums.h"
#include "Data/Dataset/vespuccidataset.h"


class UnivariateData
{
public:
    UnivariateData(QSharedPointer<VespucciDataset> parent, QString name);
    UnivariateData(QSharedPointer<VespucciDataset> parent, QString name, vec control);

    //univariate (if window==0, don't predict)
    void Apply(double left_bound,
               double right_bound,
               uword window,
               UnivariateMethod::Method method);
    //band ratio
    void Apply(double first_left_bound,
               double first_right_bound,
               double second_left_bound,
               double second_right_bound,
               uword window,
               UnivariateMethod::Method method);

    void Calibrate(const vec &values, const vec &concentrations);

    vec results() const;
    const vec *results_ptr() const;
    bool band_ratio() const;
    double left_bound() const;
    double right_bound() const;
    double first_left_bound() const;
    double first_right_bound() const;
    double second_left_bound() const;
    double second_right_bound() const;
    QString method_description() const;

    mat calibration_curve() const;
    const mat *calibration_curve_ptr() const;
    const std::map<std::string, double> calibration_stats() const;


    QString name() const;
    void SetName(QString name);

    bool calibrated() const;

    arma::field<mat> baselines() const;
    arma::field<mat> first_baselines() const;
    arma::field<mat> second_baselines() const;
    arma::field<mat> midlines() const;


private:
    QString method_description_;

    ///
    /// \brief results_
    /// The results which are turned into an image
    vec results_;

    ///
    /// \brief control_
    /// Control correlation is calculated for.
    vec control_;

    ///
    /// \brief method_
    /// The method
    int method_;
    ///
    /// \brief band_ratio_
    /// Whether or not this is a band ratio map
    bool band_ratio_;

    ///
    /// \brief left_bound_
    /// The spectral left bound
    double left_bound_;

    ///
    /// \brief right_bound_
    /// The right bound in spectral units
    double right_bound_;

    double first_left_bound_;
    double first_right_bound_;
    double second_left_bound_;
    double second_right_bound_;


    ///
    /// \brief ratio_baselines_
    arma::field<mat> baselines_;
    arma::field<mat> first_baselines_;
    arma::field<mat> second_baselines_;
    arma::field<mat> midlines_;

    ///
    /// \brief positions_
    /// The position in abscissa units of the maximum of the peaks
    mat positions_;

    QSharedPointer<VespucciDataset> parent_;


    QString name_;

    uvec boundaries_;
    mat calibration_curve_;

    std::map<std::string, double> calibration_stats_;
    bool calibrated_;
};

#endif // UNIVARIATEDATA_H
