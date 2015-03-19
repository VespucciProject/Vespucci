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
#ifndef UNIVARIATEDATA_H
#define UNIVARIATEDATA_H
#include "Global/enums.h"
#include "Data/Dataset/vespuccidataset.h"


class UnivariateData
{
public:
    UnivariateData(QSharedPointer<VespucciDataset> parent, QString name);
    UnivariateData(QSharedPointer<VespucciDataset> parent, QString name, vec control);
    void Apply(double left_bound,
          double right_bound,
          UnivariateMethod::Method method);

    void Apply(double first_left_bound,
          double first_right_bound,
          double second_left_bound,
          double second_right_bound,
          UnivariateMethod::Method method);

    vec results();
    vec *results_ptr();
    bool band_ratio();
    double left_bound();
    double right_bound();
    double first_left_bound();
    double first_right_bound();
    double second_left_bound();
    double second_right_bound();
    QString MethodDescription();

    mat Midlines();
    uvec Boundaries();
    uvec MidlineBoundaries();
    mat first_baselines();
    mat second_baselines();

    QString name();
    void SetName(QString name);

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
    /// \brief midlines_
    /// The midlines (band ratio)
    mat midlines_;

    ///
    /// \brief ratio_baselines_
    /// Contains the two sets of baselines for an intensity band ratio.
    arma::field<mat> baselines_;

    mat first_baselines_;
    mat second_baselines_;

    ///
    /// \brief positions_
    /// The position in abscissa units of the maximum of the peaks
    mat positions_;

    QSharedPointer<VespucciDataset> parent_;


    QString name_;

    uvec boundaries_;
};

#endif // UNIVARIATEDATA_H
