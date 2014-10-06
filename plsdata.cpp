#include "plsdata.h"

PLSData::PLSData(QSharedPointer<VespucciDataset> parent, QString *directory)
{
    parent_ = parent;
    directory_ = directory;
}

///
/// \brief PLSData::Apply
/// \param spectra Input matrix
/// \param wavelength Spectral abcissa
/// \param components Number of components to calculate
/// \return
/// Performs PLS analysis on a copy of the spectra matrix (transposed).
bool PLSData::Apply(mat spectra, rowvec wavelength, int components)
{
    mat Y;
    Y.set_size(wavelength.n_elem, components);
    for (int i = 0; i < components; ++i)
        Y.col(i) = trans(wavelength);
    bool success = arma_ext::plsregress(trans(spectra), Y, components,
                                        X_loadings_, Y_loadings_,
                                        X_scores_, Y_scores_,
                                        coefficients_, percent_variance_,
                                        fitted_);
    return success;

}

mat *PLSData::X_loadings()
{
    return &X_loadings_;
}

mat *PLSData::Y_loadings()
{
    return &Y_loadings_;
}

mat *PLSData::X_scores()
{
    return &X_scores_;
}

mat* PLSData::Y_scores()
{
    return &Y_scores_;
}

mat* PLSData::coefficients()
{
    return &coefficients_;
}

mat *PLSData::percent_variance()
{
    return &percent_variance_;
}

int PLSData::NumberComponents()
{
    return coefficients_.n_cols;
}

colvec PLSData::Results(const uword i, bool &valid)
{
    if (coefficients_.n_cols < i){
        valid = false;
        return X_loadings_.col(coefficients_.n_cols - 1);
    }

    else{
        valid = true;
        return X_loadings_.col(i);
    }

}
