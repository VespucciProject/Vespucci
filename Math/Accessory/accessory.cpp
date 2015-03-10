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
#include <mlpack/core.hpp>

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
    colarma::vec column;
    arma::mat output;
    output.zeros();
    output.set_size(m, n);
    colarma::vec diagonal;
    arma::ivec subdiagonals;
    arma::ivec superdiagonals;

    if (number > size){
        throw invalid_argument("spdiags: vector larger than diagonal of arma::matrix");
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
    double eps = datum::eps;
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
        cerr << "orth: no basis found" << endl;
        //cout << "end orth" << endl;
        return zeros(X.n_rows, X.n_cols);
    }
}



///
/// \brief Vespucci::Math::max
/// \param a
/// \param b
/// \return
/// std::max stopped working with arma::uwords for some reason, so I made a max function
template <typename> T Vespucci::Math::max(T a, T b)
{
    return (a > b ? a : b);
}

///
/// \brief Vespucci::Math::min
/// \param a
/// \param b
/// \return
/// std::min stopped working with arma::uwords for some reason, so I made a min function
template <typename> T Vespucci::Math::min(T a, T b)
{
    return (a < b ? a : b);
}



///
/// \brief Vespucci::Math::diff
/// \param X A vector represetning a signal or a arma::matrix where each column is a
/// signal
/// \param deriv_order The derivative order to calculate
/// \return The deriv_order-th derivative of X, a vector with deriv_order fewer
/// elements than X.
///
/// Perform arithmetic differentiation of a vector or arma::matrix (if X is a arma::matrix,
/// the differentiation is performed to all the columns).
///
template <typename T> T Vespucci::Math::diff(T X, arma::uword deriv_order)
{
        T return_value;
        if (deriv_order == 0){
            return_value = X;
        }

        else if (deriv_order > 0){ //perform first derivative of input
            T offset(X.n_rows, X.n_cols);
            offset.zeros();
            offset.rows(1, offset.n_rows - 1) = X.rows(0, X.n_rows - 2);
            //offset.insert_rows(0, 1);
            //offset.row(0) = zeros(1, X.n_cols);
            //offset.shed_row(offset.n_rows - 1);
            T difference;
            difference = X - offset;
            difference.shed_row(0); //difference will have one less row
            return_value = diff(difference, deriv_order - 1);
        }

        return return_value;
}



template <typename T> T rotate(T X, arma::uword shift_by, bool slide_back = true)
{
    T start;
    T end;

    if (shift_by >= X.n_rows){
        shift_by = shift_by - X.n_rows;
    }

    if (shift_by == 0){
        return X;
    }
    else if (shift_by == 1){
        if (slide_back){
            start = X.rows(1, X.n_rows - 1);
            end = X.row(0);
        }
        else{
            start = X.row(X.n_rows - 1);
            end = X.rows(0, X.n_rows - 2);
        }
    }
    else{
        if (slide_back){
            start = X.rows(shift_by, X.n_rows - 1);
            end = X.rows(0, shift_by - 1);
        }
        else{
            start = X.rows(X.n_rows - shift_by, X.n_rows - 1);
            end = X.rows(0, X.n_rows - shift_by - 1);
        }
    }

    return join_vert(start, end);
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
        cout << "padding length" << padding.n_rows << endl;
    }

    return join_vert(X, padding);
}

arma::uword NextPow(arma::uword number, arma::uword power)
{
    return std::ceil(std::log(number) / std::log(power));
}

//Column-wise 1-D convolution (may re-write with 2d fft later)
template <typename T> T conv_fft(T &A, T &B, std::string type = "circular")
{



    if (A.n_rows < B.n_rows){
        throw std::invalid_argument("B must have the same number of rows or fewer rows than A");
    }

    if (type == "filter"){
        arma::uword next_p2 = NextPow(A.n_elem, 2);
        arma::uword n = std::pow(2, next_p2);
        arma::cx_mat A_hat = fft(A, n);
        arma::cx_mat B_hat = fft(B, n);
        arma::cx_mat w = ifft(A_hat % B_hat, n);
        w = w.rows(0, A.n_rows - 1);
        return real(w);
    }
    else if (type == "circular"){

        T A_padded = join_vert(A, zeros(B.n_rows- 1));
        T B_padded = join_vert(B, zeros(A.n_rows - 1));

        arma::uword next_p2 = NextPow(A_padded.n_elem, 2);
        arma::uword n = std::pow(2, next_p2);

        arma::cx_mat A_hat = fft(A_padded, n);
        arma::cx_mat B_hat = fft(B_padded, n);
        arma::cx_mat w = ifft(A_hat % B_hat, n);
        //w = w.rows(0, A.n_rows - 1);
        return real(w);
    }
}
