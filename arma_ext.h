/*******************************************************************************
    Copyright (C) 2014 Daniel P. Foose - All Rights Reserved

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

    This file incoprorates code based on Octave
    Copyright (C) 1994-2013 John W. Eaton

    Octave is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Octave is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Octave.  If not, see <http://www.gnu.org/licenses/>.

*******************************************************************************/
#ifndef ARMA_EXT_H
#define ARMA_EXT_H
#include <QApplication>
#include <QVector>
#include <QMessageBox>
#include <mlpack/core.hpp>

using namespace arma;

namespace arma_ext
{
mat spdiags(mat B, QVector<int> d, int m, int n);
bool svds(mat X, uword k, mat &U, vec &s, mat &V);
mat orth(mat X);
bool plsregress(mat X, mat Y, int components,
                mat &X_loadings, mat &Y_loadings,
                mat &X_scores, mat &Y_scores,
                mat &coefficients, mat &percent_variance, mat &fitted);
bool VCA(mat R, uword p, uvec &indices, mat &endmember_spectra,
         mat &projected_data, mat &fractional_abundances);
double estimate_snr(mat R, vec r_m, mat x);
}

#endif // ARMA_EXT_H
