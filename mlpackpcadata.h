#ifndef MLPACKPCADATA_H
#define MLPACKPCADATA_H

#include "vespuccidataset.h"
#include <mlpack/methods/pca/pca.hpp>
using namespace arma;

class VespucciDataset;
class MLPACKPCAData
{
public:
    MLPACKPCAData(bool scaleData);
    void Apply(mat data);
    mat *transformed_data();
    vec *eigval();
    mat *eigvec();
    vec *percent_variance();

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
};

#endif // MLPACKPCADATA_H
