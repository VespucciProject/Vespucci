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
#include "plsdata.h"

PLSData::PLSData(QSharedPointer<VespucciDataset> parent, QString *directory)
{
    parent_ = parent;
    directory_ = directory;
}

///
/// \brief PLSData::Apply
/// \param spectra Input matrix
/// \param wavelength Spectral abscissa
/// \param components Number of components to calculate
/// \return
/// Performs PLS analysis on a copy of the spectra matrix (transposed).
bool PLSData::Apply(mat spectra, vec wavelength, int components)
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
