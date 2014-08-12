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
#include "arma_ext.h"

///
/// \brief spdiags analgous to the Octave/MATLAB function A = spdiags(B, d, m, n).
/// \param B a matrix containing the new diagonal vectors as columns
/// \param d a vector containing the row numbers to set.  The first column
/// vector of B corresponds to the first entry in d.
/// \param m the number of rows of the output matrix
/// \param n the number of columns of the output matrix
/// \return a m by n sparase matrix with the columns of B as diagonals
/// A translation of the Octave/MATLAB function A = spdiags(B, d, m, n).  For
/// subdiagonals (entries of d < 0) vectors are truncated at the end when they
/// are longer than the target diagonal.  For superdiagonals, vectors are
/// truncated at the beginning when they are longer than the target diagonal.
mat arma_ext::spdiags(mat B, QVector<int> d, int m, int n)
{
    int i, j, k;
    int size = B.n_cols;
    int number = d.size();
    int diag_size;
    int column_size = B.n_rows;
    colvec column;
    mat output;
    output.zeros();
    output.set_size(m, n);
    colvec diagonal;
    QVector<int> subdiagonals;
    QVector<int> superdiagonals;

    if (number > size){
        QMessageBox::warning(0, "Out of Range!", "An error has occured in an "
                             "internal function.");
        return output;
    }


    for (i = 0; i < number; ++i){
        if (d[i] < 0)
            subdiagonals.append(d[i]);
        else
            superdiagonals.append(d[i]);
    }

    for (i = 0; i < subdiagonals.size(); ++i){
        k = subdiagonals[i];
        diagonal = output.diag(k);
        diag_size = diagonal.n_elem;
        column = B.col(i);

        if (diag_size < column_size){
            for (i = 0; i < diag_size; ++i){
                output.diag(k)(i) = column(i);
            }
        }

        else {
            for (i = 0; i < column_size; ++i){
                output.diag(k)(i) = column(i);
            }
        }
    }

    for (i = 0; i < superdiagonals.size(); ++i){
        k = superdiagonals[i];
        diag_size = output.diag(k).n_elem;
        column = B.col(i);
        if (diag_size < column_size){
            j = column_size;
            for (i = diag_size; i <=0; --i){
                output.diag(k)(i) = column(j);
                --j;
            }
        }

        else {
            for (i = 0; i < column_size; ++i){
                output.diag(k)(i) = column(i);
            }
        }
    }
    return output;
}

///
/// \brief arma_ext::svds
/// \param X
/// \param k
/// \param form
/// \return
///
mat arma_ext::svds(mat X, int k, std::string form)
{


}

mat arma_ext::svds(mat X, int k)
{
    return arma_ext::svds(X, k,"lm");
}

mat arma_ext::svds(mat X, std::string form)
{
    return arma_ext::svds(X, 6, form);
}

mat arma_ext::svds(mat X)
{
    return arma_ext::svds(X, 6, "lm");
}

