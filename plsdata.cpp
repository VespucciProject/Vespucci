#include "plsdata.h"

PLSData::PLSData(SpecMap *parent, QString *directory)
{
    parent_ = parent;
    directory_ = directory;
}

bool PLSData::Apply(mat spectra, rowvec wavelength, int components)
{
    mat Y;
    cout << "PLSData::Apply set size of Y" << endl;
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

colvec PLSData::Results(int i, bool &valid)
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
