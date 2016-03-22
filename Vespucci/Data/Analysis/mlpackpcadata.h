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
#ifndef MLPACKPCADATA_H
#define MLPACKPCADATA_H

#include "Data/Dataset/vespuccidataset.h"
#include <mlpack/methods/pca/pca.hpp>
#include <Data/Analysis/analysisresults.h>

using namespace arma;

class VespucciDataset;
class AnalysisResults;
class MLPACKPCAData
{
public:
    MLPACKPCAData(QString name, bool scaleData);
    void Apply(mat data);
    mat *transformed_data();
    vec *eigval();
    mat *eigvec();
    vec *percent_variance();
    mat *value(QString key);
    QSharedPointer<AnalysisResults> GetResults();

private:
    ///
    /// \brief pca_data_
    /// The MLPACK PCA object that performs analysis
    mlpack::pca::PCA pca_data_;

    ///
    /// \brief transformed_data_
    /// The transformed data
    mat transformed_data_;

    ///
    /// \brief eigval_
    /// Eigenvalues of covariance matrix
    vec eigval_;

    ///
    /// \brief eigvec_
    /// Eigenvectors of covariance matrix (loadings)
    mat eigvec_;

    ///
    /// \brief percent_variance_
    /// Percent variance attributed to each loading.
    vec percent_variance_;

    ///
    /// \brief name_
    /// Name of the AnalysisResults object to be created
    QString name_;
};

#endif // MLPACKPCADATA_H
