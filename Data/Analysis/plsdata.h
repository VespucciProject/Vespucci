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
#ifndef PLSDATA_H
#define PLSDATA_H
#include "Data/Dataset/vespuccidataset.h"
using namespace arma;
class VespucciDataset;
///
/// \brief The PLSData class
/// A class for performing and storing data related to partial least squares
/// determinant analysis
class PLSData
{
public:
    PLSData(QSharedPointer<VespucciDataset> parent,
            QString *directory);

    bool Apply(mat spectra, vec wavelength, int components);
    bool DiscriminantAnalysis(mat spectra, vec labels, int components);

    mat *X_loadings();
    mat *Y_loadings();
    mat *X_scores();
    mat *Y_scores();
    mat *coefficients();
    mat *percent_variance();  

    mat *value(QString key);

    int NumberComponents();
    colvec Results(const uword i, bool &valid);
private:
    ///
    /// \brief Y_
    /// In this case, "dummy" data.
    mat Y_;

    ///
    /// \brief X_loadings_
    /// Output
    mat X_loadings_;

    ///
    /// \brief Y_loadings_
    /// Output
    mat Y_loadings_;
    ///
    /// \brief X_scores_
    /// Output
    mat X_scores_;

    ///
    /// \brief Y_scores_
    /// Output
    mat Y_scores_;

    ///
    /// \brief coefficients_
    /// Output
    mat coefficients_;

    ///
    /// \brief percent_variance_
    /// Output. This determines how much each determinant
    mat percent_variance_;

    ///
    /// \brief fitted_
    /// Fitted data, pretty much useless for our purposes
    mat fitted_;
    ///
    /// \brief parent_
    /// The dataset that starts this instance
    QSharedPointer<VespucciDataset> parent_;

    ///
    /// \brief directory_
    /// Global working directory
    QString *directory_;
};

#endif // PLSDATA_H
