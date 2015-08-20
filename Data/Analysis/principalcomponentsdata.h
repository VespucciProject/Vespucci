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
#ifndef PRINCIPALCOMPONENTSDATA_H
#define PRINCIPALCOMPONENTSDATA_H

#include "Data/Dataset/vespuccidataset.h"
using namespace arma;

class VespucciDataset;
///
/// \brief The PrincipalComponentsData class
/// A class for performing and storing data from principal components analysis
class PrincipalComponentsData {
public:
    PrincipalComponentsData(QSharedPointer<VespucciDataset> parent, QString *directory);

    void Apply(const mat &spectra);

    bool SaveCoefficientMatrix();

    colvec Results(int component);

    vec *tsquared();
    mat *score();
    vec *latent();
    mat *coeff();
    vec *percent_variance();

    mat *value(QString key);
private:
    ///
    /// \brief coeff_
    /// Principal components coefficients
    mat coeff_;

    ///
    /// \brief score_
    /// Projected data
    mat score_;

    ///
    /// \brief percent_variance_
    /// Percentage of variance accounted for by each component
    vec percent_variance_;

    ///
    /// \brief latent_
    /// Eigenvalues of the covariance matrix of spectra_
    vec latent_;

    ///
    /// \brief tsquared_
    /// Hotteling's statistic for each sample
    vec tsquared_;

    ///
    /// \brief parent_
    /// The current dataset
    QSharedPointer<VespucciDataset> parent_;

    ///
    /// \brief directory_
    /// Global working directory
    QString *directory_;

};

#endif // PRINCIPALCOMPONENTSDATA_H
