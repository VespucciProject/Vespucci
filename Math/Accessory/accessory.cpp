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
#include <Math/Accessory/accessory.h>
///
/// \brief spdiags analgous to the Octave/arma::matLAB function A = spdiags(B, d, m, n).
/// \param B a arma::matrix containing the new diagonal vectors as columns
/// \param d a vector containing the row numbers to set.  The first column
/// vector of B corresponds to the first entry in d.
/// \param m the number of rows of the output arma::matrix
/// \param n the number of columns of the output arma::matrix
/// \return a m by n sparase arma::matrix with the columns of B as diagonals
/// A translation of the Octave/arma::matLAB function A = spdiags(B, d, m, n).  For
/// subdiagonals (entries of d < 0) vectors are truncated at the end when they
/// are longer than the target diagonal.  For superdiagonals, vectors are
/// truncated at the beginning when they are longer than the target diagonal.
arma::mat Vespucci::Math::spdiags(arma::mat B, arma::ivec d, arma::uword m, arma::uword n)
{
    arma::uword i, j, k;
    arma::uword size = B.n_cols;
    arma::uword number = d.size();
    arma::uword diag_size;
    arma::uword column_size = B.n_rows;
    arma::colvec column;
    arma::mat output;
    output.zeros();
    output.set_size(m, n);
    arma::colvec diagonal;
    arma::ivec subdiagonals;
    arma::ivec superdiagonals;

    if (number > size){
        throw std::invalid_argument("spdiags: vector larger than diagonal of arma::matrix");
        return output;
    }

    superdiagonals = d(find (d >= 0));
    subdiagonals = d(find (d < 0));



    for (i = 0; i < subdiagonals.size(); ++i){
        k = subdiagonals(i);
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
        k = superdiagonals(i);
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
/// \brief orth Returns an orthonormal basis of the range space of A
/// \param X arma::matrix
/// \return a arma::matrix whose columns form an orthonormal basis for X
///
arma::mat Vespucci::Math::orth(arma::mat X)
{
    double eps = arma::datum::eps;
    arma::mat U, V;
    arma::vec s;
    svd(U, s, V, X);
    double tol;
    tol = std::max(X.n_rows*s(1)*eps, X.n_cols*s(1)*eps);
    arma::uvec q1 = find(s > tol);
    double rank = q1.n_elem;

    if (rank > 0){
        return -1*U.cols(0, rank-1);
    }
    else{
        std::cerr << "orth: no basis found" << std::endl;
        //cout << "end orth" << endl;
        return arma::zeros(X.n_rows, X.n_cols);
    }
}



///
/// \brief Vespucci::Math::max
/// \param a
/// \param b
/// \return
/// std::max stopped working with arma::uwords for some reason, so I made a max function
arma::uword Vespucci::Math::max(arma::uword a, arma::uword b)
{
    return (a > b ? a : b);
}

///
/// \brief Vespucci::Math::min
/// \param a
/// \param b
/// \return
/// std::min stopped working with arma::uwords for some reason, so I made a min function
arma::uword Vespucci::Math::min(arma::uword a, arma::uword b)
{
    return (a < b ? a : b);
}






arma::vec Vespucci::Math::ExtendToNextPow(arma::vec X, arma::uword n)
{
    arma::uword nextpow = Vespucci::Math::NextPow(X.n_rows, n);
    arma::uword new_size = std::pow(n, nextpow);
    arma::vec padding;
    arma::uword extend_by = new_size - X.n_rows;
    if (extend_by == 1){
        padding = flipud(X.row(X.n_rows-1));
    }
    else{
        padding = flipud(X.rows(X.n_rows - extend_by, X.n_rows-1));
        std::cout << "padding length" << padding.n_rows << std::endl;
    }

    return join_vert(X, padding);
}

arma::uword Vespucci::Math::NextPow(arma::uword number, arma::uword power)
{
    return std::ceil(std::log(number) / std::log(power));
}

///
/// \brief Vespucci::Math::LocalMaxima
/// \param X A matrix with signals as columns
/// \return A sparse matrix containing the value of each local maximum at the appropriate
/// position.
/// Find the local maxima of a matrix of signals using the first and second
/// derivative tests. Works best with smooth signals (like CWT coeffs)
arma::sp_mat Vespucci::Math::LocalMaxima(arma::mat X)
{
    arma::mat dX = Vespucci::Math::diff(X, 1);
    arma::mat d2X = Vespucci::Math::diff(dX, 1);
    //make everything line up.
    dX.insert_rows(0, 1, true);
    d2X.insert_rows(0, 2, true);

    arma::uvec maxima_indices, extrema_indices;
    arma::vec d2X_extrema;
    arma::umat locations;
    arma::vec values;
    arma::vec extrema_buf;
    arma::vec X_buf;
    arma::uvec position_buf_vec;
    arma::umat position_buf;


    for (arma::uword i = 0; i < X.n_cols; ++i){
        X_buf = X.col(i);
        //find where the first derivative crosses the x axis
        extrema_indices = arma::find( (dX.col(i).subvec(0, X.n_rows - 2) % dX.col(i).subvec(1, X.n_rows - 1)) < 0);

        //find the maxima that are negative in the 2nd derivative
        d2X_extrema = d2X.rows(extrema_indices);
        maxima_indices = find(d2X_extrema < 0);

        position_buf_vec = extrema_indices.rows(maxima_indices);
        position_buf.set_size(maxima_indices.n_rows, 2);
        position_buf.col(0) = position_buf_vec;
        position_buf.col(1).fill(i);
        extrema_buf = X_buf.rows(position_buf_vec);
        values.insert_rows(values.n_rows - 1, extrema_buf);
        locations.insert_rows(locations.n_rows - 1, position_buf);
    }

    return arma::sp_mat(locations, values, X.n_rows, X.n_cols, false, false);
}
