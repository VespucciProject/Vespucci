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


    This file incorporates a port of peakfinder, originally written in MATLAB,
    as arma_ext::peakfinder().

    peakfinder is covered under the following copyright and permission notice:

    Copyright (c) 2013, Nathanael C. Yoder
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are
    met:

        * Redistributions of source code must retain the above copyright
          notice, this list of conditions and the following disclaimer.
        * Redistributions in binary form must reproduce the above copyright
          notice, this list of conditions and the following disclaimer in
          the documentation and/or other materials provided with the distribution
        * Neither the name of the  nor the names
          of its contributors may be used to endorse or promote products derived
          from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
    AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
    ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
    LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
    CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
    SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
    INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
    CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
    ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
    POSSIBILITY OF SUCH DAMAGE.

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
mat arma_ext::spdiags(mat B, ivec d, uword m, uword n)
{
    uword i, j, k;
    uword size = B.n_cols;
    uword number = d.size();
    uword diag_size;
    uword column_size = B.n_rows;
    colvec column;
    mat output;
    output.zeros();
    output.set_size(m, n);
    colvec diagonal;
    ivec subdiagonals;
    ivec superdiagonals;

    if (number > size){
        QMessageBox::warning(0, "Out of Range!", "An error has occured in an "
                             "internal function.");
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
/// \param X matrix
/// \return a matrix whose columns form an orthonormal basis for X
///
mat arma_ext::orth(mat X)
{
    double eps = datum::eps;
    mat U, V;
    vec s;
    svd(U, s, V, X);
    double tol;
    tol = std::max(X.n_rows*s(1)*eps, X.n_cols*s(1)*eps);
    uvec q1 = find(s > tol);
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
/// \brief arma_ext::svds Finds a few largest singular values of the matrix X.
/// This is based on the Matlab/Octave function svds(), a truncated singular
/// value decomposition. This is designed to only take the kinds of inputs
/// Vespucci will need (It only works on arma::mat types, and only returns the k
/// largest singular values (none of that sigma business). U and V tolerances
/// are not defined currently due to difficulties with Armadillo's find() method
/// A sparse matrix [0 X; X.t() 0] is formed.
/// The eigenvalues of this matrix are then found using arma::eigs_sym, a wrapper
/// for ARPACK.
/// If X is square, it can be "reconstructed" using X = U*diagmat(s)*V.t(). This
/// "reconstruction" will have lower noise.
/// \param X An m \times n matrix.
/// \param k The number of eigenvalues to be found.
/// \param U An m \times k unitary matrix
/// \param s a vector with n
/// \param V
/// \return Whether or not algorithm converged.

bool arma_ext::svds(mat X, uword k, mat &U, vec &s, mat &V)
{
    if (X.n_cols < 2){
        cerr << "svds: X must be 2D matrix" << endl;
        return false;
    }

    uword m = X.n_rows;
    uword n = X.n_cols;
    uword p = arma_ext::min(m, n); //used below to establish tolerances.
    //uword q = arma_ext::max(m, n);

    if (k > p){
        if (k > m)
            cerr << "svds: value of k " << k << "is greater than number of rows " << m << endl;
        else
            cerr << "svds: value of k" << k << "is greater than number of columns " << n << endl;
    }

    k = arma_ext::min(p, k); //make sure value of k is less than smallest dimension

    sp_mat B(m+n, m+n);
    B.submat(span(0, m-1), span(m, m+n-1)) = X; // top right corner
    B.submat(span(m, m+n-1), span(0, m-1)) = X.t(); //bottom left corner
    //If, for some reason, a matrix of zeroes is input...
    if (B.n_nonzero == 0){
        U = eye(m, k);
        s = zeros(k, k);
        V = eye(n, k);
        cerr << "svds: input matrix has no non-zero elements" << endl;
        return false;
    }

    vec eigval;
    mat eigvec;

    //because we're using sigma=0, results of eigs will be centered around 0
    //we throw out the negative ones, then add in 0 eigenvalues if we have to
    //trying some weird stuff to figure out why arpack call fails
    bool eigs_success = eigs_sym(eigval, eigvec, B, 2*k);

    if (!eigs_success){
        cerr << "svds: arma::eigs_sym did not converge!" << endl;
    }

    //Manipulate the results

    //sort eigvec from largest to smallest:
    eigvec = eigvec.cols(sort_index(eigval, "descend"));

    //the negative eigenvalues are artifacts of how the matrix is constructed
    //it is possible that there are two 0 eigenvalues. Only the first is taken
    //The octave and matlab svds routines are a little more precise with this...
    //It is highly unlikely that a 0 eigenvalue will occur with the types of data
    //processed by Vespucci
    uvec return_indices = find(eigval >= 0, k, "first");

    U = sqrt(2) * (eigvec.rows(0, m-1));
    U = U.cols(return_indices);

    s = eigval.rows(return_indices);

    V = sqrt(2) * eigvec.rows(m, m+n-1);
    V = V.cols(return_indices);

    uvec indices = sort_index(s, "descend");
    s = sort(s, "descend");

    U = U.cols(indices);
    V = V.cols(indices);
    return eigs_success;
}


///
/// \brief arma_ext::plsregress PLS Regression Using SIMPLS algorithm.
/// This is essentially a line-for-line rewrite of plsregress from the Octave
/// statistics package.
/// Copyright (C) 2012 Fernando Damian Nieuwveldt <fdnieuwveldt@gmail.com>
/// This is an implementation of the SIMPLS algorithm:
/// Reference: SIMPLS: An alternative approach to partial least squares regression.
/// Chemometrics and Intelligent Laboratory Systems (1993)
/// \param x
/// \param y
/// \param components Number of PLS components
/// \param x_loadings "Output" values
/// \param y_loadings
/// \param x_scores
/// \param y_scores
/// \param coefficients
/// \param fitted_data
/// \return
///
bool arma_ext::plsregress(mat X, mat Y, int components,
                mat &X_loadings, mat &Y_loadings,
                mat &X_scores, mat &Y_scores,
                mat &coefficients, mat &percent_variance, mat &fitted)
{
    uword observations = X.n_rows;
    uword predictors = X.n_cols;
    uword responses = Y.n_cols;
    //Mean centering
    mat Xmeans = arma::mean(X);
    mat Ymeans = arma::mean(Y);
    //Octave does below with bsxfun. After optimization, this should hopefully not
    // be slower.
    X.each_row() -= Xmeans; //element-wise subtraction of mean
    Y.each_row() -= Ymeans; //same

    mat S = trans(X) * Y;
    mat R = zeros(predictors, components);
    mat P = R;
    mat V = R;
    mat T = zeros(observations, components);
    mat U = T;
    mat Q = zeros(responses, components);
    mat eigvec;
    vec eigval;

    mat q;
    mat r;
    mat t;
    mat nt;
    mat p;
    mat u;
    mat v;
    double max_eigval;
    for (int i = 0; i < components; ++i){
        eig_sym(eigval, eigvec, (trans(S) * S));
        max_eigval = eigval.max();
        uvec dom_index = find(eigval >= max_eigval);
        uword dominant_index = dom_index(0);

        q = eigvec.col(dominant_index);

        r = S*q; //X block factor weights
        t = X*r; //X block factor weights
        t.each_row() -= mean(t); //center t
        nt = arma::sqrt(trans(t)*t); //compute norm (is arma::norm() the same?)
        t.each_row() /= nt;
        r.each_row() /= nt; //normalize

        p = trans(X)*t; //X block factor loadings
        q = trans(Y)*t; //Y block factor loadings
        u = Y*q; //Y block factor scores
        v = p;

        //Ensure orthogonality
        if (i > 0){
            v = v - V*(trans(V)*p);
            u = u - T*(trans(T)*u);
        }
        v.each_row() /= arma::sqrt(trans(v) * v); //normalize orthogonal loadings
        S = S - v * (trans(v)*S); //deflate S wrt loadings
        R.col(i) = r;
        T.col(i) = t;
        P.col(i) = p;
        Q.col(i) = q;
        U.col(i) = u;
        V.col(i) = v;
    }

    //Regression coefficients
    mat B = R*trans(Q);
    fitted = T*trans(Q);
    fitted.each_row() += Ymeans;

    //Octave creates copies from inputs before sending to output. Doing same
    //here just to be safe.
    coefficients = B;
    X_scores = T;
    X_loadings = P;
    Y_scores = U;
    Y_loadings = Q;
    //projection = R;
    percent_variance.set_size(2, coefficients.n_cols);
    percent_variance.row(0) = sum(arma::abs(P)%arma::abs(P)) / sum(sum(arma::abs(X)%arma::abs(X)));
    percent_variance.row(1) = sum(arma::abs(Q)%arma::abs(Q)) / sum(sum(arma::abs(Y)%arma::abs(Y)));
    return true;

}

///
/// \brief arma_ext::VCA
/// Vertex Component Analysis
/// \param R The dataset
/// \param endmembers Number of endmembers to compute
/// \param indices Row indices of pure components.
/// \param endmember_spectra Spectra of pure components (note that these are in
/// columns, not rows as in spectra_)
/// \param projected_data Projected data
/// \param fractional_abundances Purity of a given spectrum relative to endmember
/// \return Convergeance (no actual test implemented...)
///
bool arma_ext::VCA(mat R, uword p,
         uvec &indices, mat &endmember_spectra,
         mat &projected_data, mat &fractional_abundances)
{
//Initializations
    uword L = R.n_rows;
    uword N = R.n_cols;
    if (L == 0 || N == 0){
        cerr << "No data!" << endl;
        return false;
    }

    if (p > L){
        cerr << "wrong number of endmembers (" << p << ")!"<< endl;
        cerr << "set to 5 or one less than number of spectra" << endl;
        p = (L < 5? 5: L-1);
    }
//Estimations of SNR
    mat r_m = mean(R, 1);
    mat R_m = repmat(r_m, 1, N); //the mean of each spectral band
    mat R_o = R - R_m; //mean-center the data
    mat Ud;
    vec Sd;
    mat Vd;
    svds(R_o*trans(R_o)/N, p, Ud, Sd, Vd);
    mat x_p = trans(Ud) * R_o;
    double SNR = estimate_snr(R, r_m, x_p);
    double SNR_th = 15 + 10*log10(p);

//Choose projective projection or projection to p-1 subspace
    mat y;
    if (SNR < SNR_th){
        uword d = p - 1;
        Ud = Ud.cols(0, d-1);
        projected_data = Ud * x_p.rows(0, d-1) + R_m; //in dimension L
        mat x = x_p.rows(0, d-1);//x_p = trans(Ud)*R_o, p-dimensional subspace
        //following three lines are one in matlab...
        mat sum_squares = sum(pow(x, 2));
        double c = sum_squares.max();
        c = std::sqrt(c);
        y = join_vert(x, c*ones(1, N));
      }
    else{
        uword d = p;
        svds(R*R.t()/N, d, Ud, Sd, Vd); //R_o is a mean centered version...
        x_p = trans(Ud)*R;
        projected_data = Ud * x_p.rows(0, d-1);
        mat x = trans(Ud) * R;
        mat u = mean(x, 1);
        y = x / repmat(sum(x % repmat(u, 1, N)), d, 1);
    }


    // The VCA algorithm
    vec w;
    w.set_size(p);
    vec f;
    rowvec v;
    indices.set_size(p);
    //there are no fill functions for uvecs
    for (uword i = 0; i < p; ++i)
        indices(i) = 0;

    mat A = zeros(p, p);
    double v_max;
    double sum_squares;
    uvec q1;
    A(p-1, 0) = 1;
    for (uword i = 0; i < p; ++i){
        w.randu();
        f = w - A*pinv(A)*w;
        sum_squares = sqrt(sum(square(f)));
        f /= sum_squares;
        v = trans(f) * y;
        v_max = max(abs(v));
        q1 = find(abs(v) == v_max, 1);
        indices(i) = q1(0);
        A.col(i) = y.col(indices(i)); //same as x.col(indices(i));
    }
    endmember_spectra = projected_data.cols(indices);
    fractional_abundances = trans(pinv(endmember_spectra) * projected_data);
    return true;
}

///
/// \brief estimate_snr Estimates Signal-to-Noise ratio.
/// \cite Nascimento2005
/// \param R Input
/// \param r_m
/// \param x
/// \return
///
double arma_ext::estimate_snr(mat R, vec r_m, mat x)
{
    uword L = R.n_rows;
    uword N = R.n_cols;
    uword p = x.n_rows;
    if (x.n_cols != N){
        cerr << "invaliid input to estimate_snr" << endl;
        return 0;
    }

    double P_y = accu(arma::square(R))/N;
    double P_x = accu(arma::square(x))/N + dot(r_m, r_m);
    return 10*log10( (P_x - p/L*P_y)/(P_y - P_x) );
}


///
/// \brief arma_ext::sgolay Find a matrix of Savitzky-Golay convolution coefficients
/// \param poly_order
/// \param window_size
/// \param deriv_order
/// \param scaling_factor
/// \return A matrix of size window_size by window_size containing coefficients.
/// Finds a matrix of Savitzky-Golay convolution coefficients, similar to the
/// sgolay function in the Octave and MATLAB signal packages. The central row is
/// the filter used for most of the dataset. The outer rows are used on the edges
/// of the signal to be filtered.
///
mat arma_ext::sgolay(uword poly_order,
                     uword window_size,
                     uword deriv_order,
                     uword scaling_factor)
{
    if (window_size % 2 == 0){
        cerr << "sgolay: invalid window size. Window size must be odd." << endl;
        cerr << "using window size poly_order + 2 or nearest odd value" << endl;
        window_size = poly_order + 3 - (poly_order % 2);
    }

    if (deriv_order > poly_order){
        cerr << "sgolay: derivative order greater than polynomial order." << endl;
        cerr << "using derivative order poly order - 1" << endl;
        deriv_order = poly_order - 1;
    }
    mat F = zeros(window_size, window_size);
    uword k = (window_size - 1) / 2;
    mat C(window_size, poly_order + 1);
    sword signed_window_size = window_size;
    vec column;
    sword row; //starting at 1 not 0
    mat A;
    for (uword i = 0; i <= k; ++i){
        row = i + 1;
        column = linspace<vec>((1-row), (signed_window_size - row), window_size);
        for (uword j = 0; j <= poly_order; ++j){
            C.col(j) = arma::pow(column, j);
        }
        A = pinv(C);
        F.row(i) = A.row(deriv_order);
    }
    sword sign = (deriv_order % 2 == 0 ? 1: -1);
    uword start_index = 0;
    for (uword i = window_size - 1; i >k; --i){
        F.row(i) = sign*F.row(start_index);
        start_index++;
    }
    double product = (deriv_order == 0 ? 1 : prod(linspace<vec>(1, deriv_order, deriv_order)));
    double power = pow(scaling_factor, deriv_order);
    F /= (power/product);
    return F;
}

///
/// \brief sgolayfilt Apply Savitzky-Golay smoothing to each column of a matrix
/// \param x Input matrix. Each column is a signal
/// \param poly_order Polynomial order for smoothing
/// \param window_size Size of filter window. Must be odd and larger than poly order
/// \param deriv_order Derivative order, to extract derivatized data directly
/// \param scaling_factor A scaling factor
/// \return Smoothed data
///
mat arma_ext::sgolayfilt(mat x, uword poly_order, uword window_size, uword deriv_order, uword scaling_factor)
{
    mat return_value(x.n_rows, x.n_cols);

    //this function filters by column.
    //any invalid window size is set to preferred poly_order + 2 or poly_order + 3 for even poly_order
    if ((poly_order > window_size) || (window_size % 2 == 0)){
        window_size = poly_order + 3 - (poly_order % 2);
    }
    //if deriv_order is too high, make it one less than polynomial order
    if (deriv_order > poly_order){
        deriv_order = poly_order - 1;
    }
    if (x.n_rows < window_size){
        cerr << "sgolayfilt: not enough data for filter window of this size" << endl;
        return x;
    }

    mat coefficients = sgolay(poly_order, window_size, deriv_order, scaling_factor);

    for (uword i = 0; i < x.n_cols; ++i)
        return_value.col(i) = ApplyFilter(x.col(i), coefficients, window_size);

    return return_value;
}

///
/// \brief arma_ext::ApplyFilter Apply FIR filters to a column vector.
/// \param x Data to be filtered
/// \param coefficients A matrix of FIR filters.
/// \param window_size Filter window size
/// \return Filtered data
/// Apply FIR filters to a column vector. The central row of coefficients
/// contains the filter used for most of the data. The first (window_size - 1)/2
/// rows are filtered with the lower rows ofccoefficients, and likewise for the
/// last (window_size - 1)/2 rows and the first rows of coefficients.
///
vec arma_ext::ApplyFilter(vec x, mat coefficients, uword window_size)
{
    uword k = (window_size - 1) / 2;
    vec filter = trans(coefficients.row(k));
    //coefficients output by sgolay are in reverse order expected.
    filter = fliplr(filter);
    vec filtered_data = conv(filter, x);
    //for conv() to be equivalent to filter, we have to chop off the last window_size - 1 elements
    filtered_data = filtered_data.rows(0, x.n_rows - 1);
    //Now we have to fix the ends of the filtered data
    filtered_data.rows(0, k) = coefficients.rows(k, window_size-1)*x.rows(0, window_size-1);
    filtered_data.rows(x.n_rows - k, x.n_rows - 1) = coefficients.rows(0, k - 1)*x.rows(x.n_rows - window_size, x.n_rows - 1);
    return filtered_data;
}

///
/// \brief arma_ext::ApplyFilter
/// \param x The vector to filter
/// \param filter The filter
/// \return Filtered data
/// Applies an FIR filter to x
vec arma_ext::ApplyFilter(vec x, vec filter)
{
    uword k = (filter.n_elem - 1)/ 2;
    vec out = conv(filter, x);
    //cut off the part that hangs over
    out = out.rows(0, x.n_elem - 1);
    //replace boundaries with values from the original
    out.rows(0, k) = x.rows(0, k);
    out.rows(x.n_elem - k, x.n_elem - 1) = x.rows(x.n_elem - k, x.n_elem - 1);
    return out;
}

///
/// \brief arma_ext::CreateMovingAverageFilter
/// \param window_size The window size of the filter.
/// \return A moving average filter
/// Create a moving average filter of a certain window size.
vec arma_ext::CreateMovingAverageFilter(uword window_size)
{
    double value = 1 / (double) window_size;
    vec filter(window_size);
    filter.fill(value);
    return filter;
}

///
/// \brief arma_ext::StandardScore
/// \param X A signal to be standardized
/// \return Standardized signal
///
vec arma_ext::StandardScore(vec X)
{
    vec normalized = X;
    double mean = arma::mean(normalized);
    double std_dev = arma::stddev(X);
    normalized -= mean * ones(normalized.n_elem);
    normalized /= std_dev;
    return normalized;
}

///
/// \brief arma_ext::StandardScore
/// \param X
/// \return A matrix in which each column of X is replaced by its standard score.
///
mat arma_ext::StandardScoreMat(mat X)
{
    mat normalized = X;
    for (uword j = 0; j < normalized.n_cols; ++j)
        normalized.col(j) = StandardScore(normalized.col(j));
    return normalized;
}


///
/// \brief arma_ext::max
/// \param a
/// \param b
/// \return
/// std::max stopped working with uwords for some reason, so I made a max function
uword arma_ext::max(uword a, uword b)
{
    return (a > b ? a : b);
}

///
/// \brief arma_ext::min
/// \param a
/// \param b
/// \return
/// std::min stopped working with uwords for some reason, so I made a min function
uword arma_ext::min(uword a, uword b)
{
    return (a < b ? a : b);
}

///
/// \brief arma_ext::MedianFilter
/// \param X
/// \param window_size
/// \return
/// Performs median filtering on a signal X. This just ignores the edges becuause
/// they will be fairly small and not very interesting.
vec arma_ext::MedianFilter(vec X, uword window_size)
{
    uword k = (window_size - 1) / 2;

    vec filtered = X; //copy the whole thing, then add in the bit we actually filter
    vec buffer;
    uvec sorted;
    //The armadillo median function results in a crash on some mingw compilers
    //This method might not be as fast, but it always works.
    try{
        //sort the window then pick the middle value
        for (uword i = k; i < (X.n_rows - k); ++i){
            buffer = X.subvec(i-k, i+k);
            sorted = stable_sort_index(buffer);
            filtered(i) = buffer(sorted(k));
            //filtered(i) = median(buffer);
        }

    }catch(std::exception e){
        cout << e.what();
    }

    return filtered;
}

///
/// \brief arma_ext::MedianFilterMat
/// \param X
/// \param window_size
/// \return
/// Performs Median Filter over a matrix with spectra in columns
mat arma_ext::MedianFilterMat(mat X, uword window_size)
{
    mat filtered;
    filtered.set_size(X.n_rows, X.n_cols);
    for(uword i = 0; i < X.n_cols; ++i)
        filtered.col(i) = MedianFilter(X.col(i), window_size);
    return filtered;
}

///
/// \brief arma_ext::IntegratePeak
/// \param X
/// \param min_index
/// \param max_index
/// \param abscissa_step
/// \param baseline
/// \return
/// Takes a Riemann sum under a peak defined by certain indices
double arma_ext::IntegratePeak(vec X, uword min_index, uword max_index, double abscissa_step, vec &baseline)
{
    vec region = X.subvec(min_index, max_index);

    double start = X(min_index);
    double end = X(max_index);

    baseline = linspace(start, end, region.n_elem);

    double baseline_area = sum(baseline) / abscissa_step;
    double region_area = sum(region) / abscissa_step;

    return region_area - baseline_area;
}

///
/// \brief arma_ext::IntegratePeakMat
/// \param X
/// \param abscissa
/// \param min
/// \param max
/// \param baselines
/// \return
/// Finds the index of specified start and end values, then calls IntegratePeak
/// on each column of the matrix
vec arma_ext::IntegratePeakMat(mat X, vec abscissa, double &min, double &max, mat &baselines, uvec &boundaries)
{
    double delta = std::abs(abscissa(1) - abscissa(0));
    uvec left_bound = find(((min-delta) <= abscissa) && (abscissa <= (min+delta)));
    uvec right_bound = find(((max-delta) <= abscissa) && (abscissa <= (max+delta)));

    uword min_index = left_bound(0);
    uword max_index = right_bound(0);
    min = abscissa(min_index);
    max = abscissa(max_index);
    boundaries << min_index << endr << max_index;
    vec results(X.n_cols);
    baselines.set_size(X.col(0).subvec(min_index, max_index).n_elem, X.n_cols);
    vec baseline(baselines.n_cols);
    for (uword i = 0; i < X.n_cols; ++i){
        results(i) = IntegratePeak(X.col(i), min_index, max_index, delta, baseline);
        baselines.col(i) = baseline;
    }

    return results;
}

///
/// \brief arma_ext::IntegratePeaksMat
/// \param X
/// \param abscissa
/// \param first_min
/// \param first_max
/// \param second_min
/// \param second_max
/// \param first_baselines
/// \param second_baselines
/// \return
/// Performs two peak integrations
mat arma_ext::IntegratePeaksMat(mat X, vec abscissa, double &first_min, double &first_max, double &second_min, double &second_max, mat &first_baselines, mat &second_baselines, uvec &boundaries)
{
    double delta = std::abs(abscissa(1) - abscissa(0));
    uvec first_left_bound = find(((first_min-delta) <= abscissa) && (abscissa <= (first_min+delta)));
    uvec first_right_bound = find(((first_max-delta) <= abscissa) && (abscissa <= (first_max+delta)));
    uvec second_left_bound = find(((second_min-delta) <= abscissa) && (abscissa <= (second_min+delta)));
    uvec second_right_bound = find(((second_max-delta) <= abscissa) && (abscissa <= (second_max+delta)));

    uword first_min_index = first_left_bound(0);
    uword first_max_index = first_right_bound(0);
    uword second_min_index = second_left_bound(0);
    uword second_max_index = second_right_bound(0);

    first_min = abscissa(first_min_index);
    first_max = abscissa(first_max_index);
    second_min = abscissa(second_min_index);
    second_max = abscissa(second_max_index);

    boundaries.set_size(4);
    boundaries(0) = first_min_index;
    boundaries(1) = first_max_index;
    boundaries(2) = second_min_index;
    boundaries(3) = second_max_index;
    //The << operator doesn't seem to work on uvecs...
    /*
    boundaries << first_min_index << endr << first_max_index <<
                  second_min_index << endr << second_max_index;
    */
    first_baselines.set_size(X.col(0).subvec(first_min_index, first_max_index).n_elem, X.n_cols);
    vec first_baseline(first_baselines.n_cols);
    second_baselines.set_size(X.col(0).subvec(second_min_index, second_max_index).n_elem, X.n_cols);
    vec second_baseline(second_baselines.n_cols);

    mat results (X.n_cols, 2);
    for (uword i = 0; i < X.n_cols; ++i){
        results(i, 0) = IntegratePeak(X.col(i), first_min_index, first_max_index, delta, first_baseline);
        first_baselines.col(i) = first_baseline;
    }
    for (uword i = 0; i < X.n_cols; ++i){
        results(i, 1) = IntegratePeak(X.col(i), second_min_index, second_max_index, delta, second_baseline);
        second_baselines.col(i) = second_baseline;
    }

    return results;
}
///
/// \brief arma_ext::FindPeakMax
/// \param X
/// \param min_index
/// \param max_index
/// \param position
/// \return
/// Finds the maximum of a peak bound by min_index and max_index
double arma_ext::FindPeakMax(vec X, uword min_index, uword max_index, uword &position)
{
    vec region = X.subvec(min_index, max_index);
    double max = region.max();
    uvec positions = find(region == max);
    position = min_index + positions(0);
    return max;
}

///
/// \brief arma_ext::FindPeakMaxMat
/// \param X
/// \param abscissa
/// \param min
/// \param max
/// \param positions
/// \return
/// Iterates FindPeakMat over the columns of a matrix. Finds the indices of specified
/// min and max inputs
vec arma_ext::FindPeakMaxMat(mat X, vec abscissa, double &min, double &max, vec &positions)
{
    double delta = std::abs(abscissa(1) - abscissa(0));
    uvec left_bound = find(((min-delta) <= abscissa) && (abscissa <= (min+delta)));
    uvec right_bound = find(((max-delta) <= abscissa) && (abscissa <= (max+delta)));

    uword min_index = left_bound(0);
    uword max_index = right_bound(0);

    min = abscissa(min_index);
    max = abscissa(max_index);

    vec results(X.n_cols);
    uword position;
    positions.set_size(X.n_cols);
    for (uword i = 0; i < X.n_cols; ++i){
        results(i) = FindPeakMax(X.col(i), min_index, max_index, position);
        positions(i) = abscissa(position);
    }

    return results;
}

///
/// \brief arma_ext::FindPeakMaxesMat
/// \param X
/// \param abscissa
/// \param first_min
/// \param first_max
/// \param second_min
/// \param second_max
/// \param positions
/// \return
/// Finds two peaks in the manner of FindPeakMaxMat
mat arma_ext::FindPeakMaxesMat(mat X, vec abscissa, double &first_min, double &first_max, double &second_min, double &second_max, mat positions)
{
    double delta = std::abs(abscissa(1) - abscissa(0));
    uvec first_left_bound = find(((first_min-delta) <= abscissa) && (abscissa <= (first_min+delta)));
    uvec first_right_bound = find(((first_max-delta) <= abscissa) && (abscissa <= (first_max+delta)));
    uvec second_left_bound = find(((second_min-delta) <= abscissa) && (abscissa <= (second_min+delta)));
    uvec second_right_bound = find(((second_max-delta) <= abscissa) && (abscissa <= (second_max+delta)));

    uword first_min_index = first_left_bound(0);
    uword first_max_index = first_right_bound(0);
    uword second_min_index = second_left_bound(0);
    uword second_max_index = second_right_bound(0);

    first_min = abscissa(first_min_index);
    first_max = abscissa(first_max_index);
    second_min = abscissa(second_min_index);
    second_max = abscissa(second_max_index);

    positions.set_size(X.n_cols, 2);
    mat results(X.n_cols, 2);
    uword first_position;
    uword second_position;
    for (uword i = 0; i < X.n_cols; ++i){
        results(i, 0) = FindPeakMax(X.col(i), first_min_index, first_max_index, first_position);
        results(i, 1) = FindPeakMax(X.col(i), second_min_index, second_max_index, second_position);
        positions(i, 0) = abscissa(first_position);
        positions(i, 1) = abscissa(second_position);
    }

    return results;

}

///
/// \brief arma_ext::FindBandwidth
/// \param X
/// \param min_index
/// \param max_index
/// \param midline
/// \param abscissa_step
/// \return
/// Finds the full-width at half maximum of a peak bound by min_index and max_index
double arma_ext::FindBandwidth(vec X, uword min_index, uword max_index, vec &midline, vec &baseline, double abscissa_step)
{
   vec region = X.subvec(min_index, max_index);
   uword size = region.n_elem;
   double maximum, half_maximum;
   double start_value, end_value;
   midline.set_size(size);
   max_index = 0;
   uword left_index = 0;
   uword right_index = 0;
   start_value = X(min_index);
   end_value = X(max_index);
   baseline = linspace(start_value, end_value, size);
   region -= baseline;
   maximum = region.max();
   half_maximum = maximum / 2.0;
   uvec max_indices = find(region == maximum);
   max_index = max_indices(0);

   //search for left inflection point
   for (uword i = max_index; i > 0; --i){
       if (X(i) - half_maximum < 0){
           left_index = i;
           break;
       }
   }

   //search for right inflection point
   for (uword i = max_index; i < size; ++i){
       if (X(i) - half_maximum < 0){
           right_index = i;
           break;
       }
   }

   //check to make sure the values on the other side of the inflection point aren't better
   if (left_index > 0 && right_index < size - 1){
       if(std::fabs(X(left_index) - half_maximum) > std::fabs(X(left_index - 1) - half_maximum)){
           --left_index;
       }

       if (std::fabs(X(right_index) - half_maximum) > std::fabs(X(right_index + 1) - half_maximum)){
           ++right_index;
       }
   }

   double region_size = region.subvec(left_index, right_index).n_elem;
   return abscissa_step * region_size;
}

///
/// \brief arma_ext::FindBandwidthMat
/// \param X
/// \param abscissa
/// \param min
/// \param max
/// \param midlines
/// \param baselines
/// \return
/// Finds the bandwidth of every column of a matrix.
vec arma_ext::FindBandwidthMat(mat X, vec abscissa, double &min, double &max, mat &midlines, mat &baselines, uvec &boundaries)
{
    double delta = std::abs(abscissa(1) - abscissa(0));
    uvec left_bound = find(((min-delta) <= abscissa) && (abscissa <= (min+delta)));
    uvec right_bound = find(((max-delta) <= abscissa) && (abscissa <= (max+delta)));

    uword min_index = left_bound(0);
    uword max_index = right_bound(0);
    boundaries << min_index << endr << max_index;

    min = abscissa(min_index);
    max = abscissa(max_index);

    uword size = abscissa.subvec(min_index, max_index).n_elem;
    vec results(X.n_cols);
    midlines.set_size(X.n_cols, size);
    baselines.set_size(X.n_cols, size);
    vec midline;
    vec baseline;
    for (uword i = 0; i < X.n_cols; ++i){
        results(i) = FindBandwidth(X.col(i), min_index, max_index, midline, baseline, delta);
        midlines.row(i) = midline;
        baselines.row(i) = baseline;
    }

    return results;
}

vec arma_ext::CorrelationMat(mat X, vec control)
{
    vec results;
    results.set_size(X.n_cols);
    for(uword i = 0; i < X.n_cols; ++i){
        results(i) = as_scalar(cor(control, X.col(i)));
    }
    return results;
}

int arma_ext::HySime(mat y,
                     mat n,
                     mat Rn,
                     mat &Ek)
{
    cout << "arma_ext::HySime()" << endl;
    if (n.n_rows != y.n_rows || n.n_cols  != y.n_cols){
        cerr << "HySime: Empty noise matrix or its size does not agree with size of y" << endl;
        throw(std::runtime_error("HySime: Empty noise matrix or does not agree with size of y"));
    }

    if (Rn.n_rows != Rn.n_cols || Rn.n_rows != y.n_rows){
        cerr << "Bad noise correlation matrix" << endl;
        Rn = n*n.t() / y.n_cols;
    }

    mat x = y - n;

    mat Ry = y*y.t() / y.n_cols;
    mat Rx = x*x.t() / y.n_cols;
    mat E, D, V;
    vec dx;
    svd(E, dx, V, Rx);
    D = diagmat(dx);

    Rn = Rn + sum(Rx.diag())/y.n_rows/10000*eye(y.n_rows, y.n_rows);

    mat P = E.t() * Ry * E;
    vec Py = P.diag();

    P = E.t() * Rn * E;
    vec Pn = P.diag();

    vec cost_F = -Py + 2 * Pn;
    uvec negatives = find (cost_F < 0);
    int kf = negatives.n_elem;
    uvec sorted_cols = sort_index(cost_F);
    Ek = E.cols(sorted_cols);
    return kf;

}

///
/// \brief arma_ext::EstimateAdditiveNoise
/// \param noise
/// \param noise_correlation
/// \param sample
/// The slow additive noise estimator from the HySime paper. I'm looking into
/// faster ones.
void arma_ext::EstimateAdditiveNoise(mat &noise, mat &noise_correlation, mat sample)
{
    double small = 1e-6;
    noise = zeros(sample.n_rows, sample.n_cols);
    mat RR = sample * sample.t();
    mat RRi = inv(RR + small*eye(sample.n_rows, sample.n_rows));
    mat XX, RRa, beta;
    for (uword i = 0; i < sample.n_rows; ++i){
        XX = RRi - (RRi.col(i) * RRi.row(i))/RRi(i, i);
        RRa = RR.col(i);
        RRa(i) = 0;
        beta = XX * RRa;
        beta(i) = 0;
        noise.row(i) = sample.row(i) - beta.t() * sample;
    }
    mat nn = noise * noise.t() / sample.n_rows;
    noise_correlation = diagmat(nn.diag());
}







///
/// \brief arma_ext::FindPeaks an implementation of the peakfinder MATLAB routine
/// \param X A vector representing a spectrum
/// \param dX A first-derivative spectrum of X
/// \param sel The amount above surrounding data for a peak to be identified
/// \param threshold A threshold value which peaks must be larger than to be
/// maxima
/// \param poly_order The polynomial order of the Savitzky-Golay filter used for
/// derivatization
/// \param window_size The window size of the Savitzky-Golay filter used for
/// derivatization
/// \param peak_locations A vector containing one and zeros corresponding to
/// whether or not a peak center exists at that wavelength
/// \param peak_magnitudes A vector containing values at peak centers.
/// \return
///
vec arma_ext::FindPeaks(vec X,
                        vec dX,
                        double sel,
                        double threshold,
                        vec &peak_magnitudes)
{
    uvec peak_indices;
    uvec peak_locations;
    peak_magnitudes = zeros(X.n_rows, 1);

    dX.elem( find(dX == 0) ).fill(-datum::eps); //to find first of repeated values
    double minimum_magnitude = X.min();
    double left_min = X(0);
    uword length = X.n_elem;
    uword temporary_location = 0;

    //Find where derivative changes sign:
    uvec extrema_indices = find( (dX.subvec(0, length - 2) % dX.subvec(1, length - 1) )< 0);
    vec X_extrema = X.elem(extrema_indices);
    length = X_extrema.n_elem;
    double temporary_magnitude = minimum_magnitude;
    bool peak_found = false;
    uword ii;
    if (X(1) >= X(2))
        ii = 1;
    else
        ii = 2;

    //This loop finds the peak locations
    while (ii < length){
        //reset peak finding if we had a peak earlier and the next peak is
        //bigger than the last or the left minimum was small enough to reset
        if (peak_found){
            temporary_magnitude = minimum_magnitude;
            peak_found = false;
        }

        if ( (X_extrema(ii) > temporary_magnitude) && (X_extrema(ii) > left_min + sel) ){
            cout << "temporary_location = " << ii << endl;
            temporary_magnitude = X_extrema(ii);
        }

        ++ii; //move into the valley
        if (ii >= length){
            break;
        }
        //come down at least sel from peak
        if (!peak_found && (temporary_magnitude > sel + X_extrema(ii) ) ){
            peak_found = true;
            cout << "peak found!" << endl;
            left_min = X_extrema(ii);
            peak_locations << temporary_location;
        }
        else if(X(ii) < left_min)
            left_min = X_extrema(ii);

        ++ii;


    }
    //remove all peaks below the threshold

    vec results = zeros(X.n_elem, 1);
    peak_indices = extrema_indices.elem(peak_locations);
    results.elem(peak_indices).ones();
    peak_magnitudes.elem(peak_indices) = X(peak_indices);



    uvec indices = find(peak_magnitudes <= threshold);
    peak_magnitudes.elem(indices).zeros();
    results.elem(indices).zeros();
    return results;
}


///
/// \brief arma_ext::FindPeaksMat Performs FindPeaks on a spectra matrix
/// \param x
/// \param sel
/// \param threshold
/// \param poly_order
/// \param window_size
/// \param peak_locations
/// \param peak_magnitudes
/// \return
///
mat arma_ext::FindPeaksMat(mat X, double sel, double threshold, uword poly_order, uword window_size, mat &peak_magnitudes)
{
    mat peak_locations(X.n_rows, X.n_cols); //value that gets returned
    peak_magnitudes.set_size(X.n_rows, X.n_cols);

    //calculate a smoothed first derivative:
    mat derivatized = arma_ext::sgolayfilt(X, poly_order, window_size, 1, 1);
    vec thresh(X.n_cols);
    vec sels(X.n_cols);
    if (std::isnan(sel)){
        for (uword i = 0; i < X.n_cols; ++i){
            sels(i) = (X.col(i).max() - X.col(i).min() ) / 16.0;
        }
    }
    else{
        sels.fill(sel);
    }

    if (std::isnan(threshold)){
        uword k = (X.n_rows % 2 == 0 ? (X.n_rows / 2) : (X.n_rows + 1 / 2 ));
        vec buffer(X.n_rows);
        uvec sorted;
        //we calculate median this way due to a problem with my MinGW compiler that
        //causes crashes when arma::median is called.
        for (uword i = 0; i < X.n_cols; ++i){
            buffer = arma_ext::diff(X, 1); //use arithmetic derivative, rather than smoothed derivative
            sorted = stable_sort_index(buffer);
            thresh(i) = buffer(sorted(k));
        }
    }
    else{
        thresh.fill(threshold);
    }
    vec current_spectrum;
    vec current_derivative;
    vec current_magnitudes;
    vec current_peaks;
    for (uword i = 0; i < X.n_cols; ++i){
        current_spectrum = X.col(i);
        current_derivative = derivatized.col(i);
        current_peaks = arma_ext::FindPeaks(current_spectrum, current_derivative, sels(i), thresh(i), current_magnitudes);
        peak_locations.col(i) = current_peaks;
        peak_magnitudes.col(i) = current_magnitudes;
    }
    return peak_locations;
}

///
/// \brief arma_ext::diff
/// \param X A vector represetnign a signal
/// \param deriv_order The derivative order to calculate
/// \return The deriv_order-th derivative of X, a vector with deriv_order fewer
/// elements than X.
///
/// Perform arithmetic differentiation of a vector.
///
vec arma_ext::diff(vec X, uword deriv_order)
{
    cout << "deriv_order = " << deriv_order << endl;
    vec return_value;
    if (deriv_order == 0){
        cout << "deriv_order == 0 !" << endl;
        return_value = X;
    }

    else if (deriv_order > 0){ //perform first derivative of input
        vec offset = X;
        offset.insert_rows(0, 1);
        offset(0) = 0;
        offset.shed_row(offset.n_rows - 1);
        vec difference;
        difference = X - offset;
        difference.shed_row(0); //difference will have one less element
        return_value = diff(difference, deriv_order - 1);
    }

    return return_value;
}

///
/// \brief arma_ext::cwt
/// \param X A vector (in this function, it must be real, because Vespucci is only
/// handling real-valued data).
/// \param wavelet A string describing wavelet type. See below.
/// \param qscale A parameter related to scale in the traditional sense. A higher
/// value is a better reproduction of the original, but decays faster. 128 seems
/// to be an ok value.
/// \return A vector of CWT coefficients.
/// Calculate a vector of CWT coefficients using a Fast Fourier Transform (FFT).
/// Peaks in the CWT will correspond to peaks in the spectrum. A higher value of
/// qscale will result in a higher number of peaks represented in the CWT, but
/// with the CWT valid for a lower number of points (within the range of Raman
/// spectra, this concern is not particulary important, as a higher scale will
/// also create a noisier cwt.
vec arma_ext::cwt(vec X, std::string wavelet, uword qscale)
{

    uword n = X.n_rows;
    uword k = n/2; //will be truncated if odd
    cx_vec X_hat = fft(X);
    vec X_i = join_vert(linspace(0, k, 1+k), linspace(-k+1, -1.0, k-1));
    X_i = X_i * (2 * datum::pi/n);

    vec omega = (n * X_i) / qscale;

    vec window; //the wavelet
    if (wavelet == "gaus")
        window = exp(-pow(omega,2) / 2);
    //else if (wavelet == "haar")
    //    ;
    else //if (wavelet == 'mexh')
        window = pow(omega, 2) % exp(-1 * pow(omega,2) / 2);

    //normalize window to the scale
    window = window / std::sqrt(qscale);

    //multiply the window by the fft of the vector, then take the ifft of that
    cx_vec w = ifft(window % X_hat);

    return real(w); //return real part of the ifft

}



///
/// \brief arma_ext::FindPeakPositions
/// \param X
/// \param dX A buffered first derivative (same size of X with value of X(i) equal
/// to the derivative of X at i. Taken as a parameter incase a smoothed derivative is to be used
/// \param threshold A value for threshold of significance. Depending on type
/// \param threshold_method Describes the method to be used (see below)
/// \return A umat in which each row represents a peak. The first column contains
/// indices of peak centers, the second column contains the left bound of the peak
/// and the third column contains the right bound of the peak. A matrix of this
/// format is the expected input for arma_ext::EstimateBaseline. The peak determination
/// may be carried out on a transformed spectra (such as heavy S-G smoothing, kernel
/// convolution or CWT) then EstimateBaseline called with these peak centers and
/// the original spectrum. EstimateBaseline will then exclude the larger peaks
/// from the baseline. Since EstimateBaseline uses a local minimum filter, smaller
/// peaks will be retained while preventing wider peaks from being cut into by the
/// filter.
///
/// threshold_method can be the following:
/// "magnitude" - a minimum peak-height threshold, arbitrary double
/// "count" - maximum number of peaks found, largest magnitude first (1 to many)
/// "countpercentage" - largest percentage of all peaks found (0 to 1)
/// "ratio" - ratio of largest magnitude (0 to 1)
umat arma_ext::FindPeakPositions(vec X, vec dX,
                                 double threshold,
                                 std::string threshold_method,
                                 vec &peak_magnitudes)
{
    //threshold can be arbitrary or calculated
    cout << "FindPeakPositions" << endl;

    vec d2X = arma_ext::diff(dX, 1);
    d2X.insert_rows(0, 1, true);
    if (threshold_method == "count" && threshold <= 0){
        throw std::runtime_error("Invalid threshold value for given method");
    }
    if ((threshold_method == "countpercentage" || threshold_method == "ratio") && threshold > 1){
        throw std::runtime_error("Invalid threshold value for given method");
    }

    //find where first derivative changes sign
    uvec extrema_indices = find( (dX.subvec(0, X.n_rows - 2) % dX.subvec(1, X.n_rows - 1) )< 0);
    vec d2X_extrema = d2X(extrema_indices);
    uvec maxima_indices = find(d2X_extrema < 0); //the indices in d2X_extrema, X_extrema and extrema_indices that correspond to maxima
    cout << "d2X_extrema.min() = " << d2X_extrema.min() << endl;
    cout << "maxima_indices.n_elem = " << maxima_indices.n_elem << endl;
    umat results(maxima_indices.n_elem, 3);
    uvec buffer(3);
    peak_magnitudes.set_size(maxima_indices.n_elem); //each maximum corresponds to a magnitude

    //we have the indices of the maxima, now we need to find magnitudes and positions
    //of the minima associated with each maximum
    uword center_index = 0;
    uword left_index = 0;
    uword right_index = 0;

    //find peak edges for each center and the magnitude of the peaks
    uword i;
    try{
        for (i = 0; i < maxima_indices.n_elem; ++i){

            if(maxima_indices(i) == 0){
                //no left index (at beginning)
                center_index = extrema_indices(maxima_indices(i)); //index of the center of peak
                right_index = extrema_indices(maxima_indices(i) + 1); //index of right edge
                left_index = center_index; //left edge is same as center
            }

            else if(maxima_indices(i) + 1 >= extrema_indices.n_elem){
                //no right_index (we're at the end)
                center_index = extrema_indices(maxima_indices(i)); //index of the center of peak
                left_index = extrema_indices(maxima_indices(i) - 1); //index of left edge
                right_index = center_index;
            }

            else{
                //normal
                center_index = extrema_indices(maxima_indices(i)); //index of the center of peak
                left_index = extrema_indices(maxima_indices(i) - 1); //index of left edge
                right_index = extrema_indices(maxima_indices(i) + 1); //index of right edge
            }

            //if left_index == right_index, average is the same as difference...
            peak_magnitudes(i) = (2.0*X(center_index) - X(left_index) - X(right_index))/2.0;
            buffer(0) = center_index;
            buffer(1) = left_index;
            buffer(2) = right_index;
            results.row(i) = buffer.t();
        }
    }catch(std::exception e){
        cout << "Exception!" << endl;
        cout << e.what() << endl;
        cout << "iterator index = " << i << endl;
        cout << "maxima index = " << maxima_indices(i) << endl;
        cout << "extrema_indices.n_rows = " << extrema_indices.n_rows << endl;
        cout << "center_index = " << center_index << endl;
        cout << "left_index = " << left_index << endl;
        cout << "right_index = " << right_index << endl;
        throw std::runtime_error(e.what());
    }

    //Fix thresholds//
    //sort with largest first:
    uvec sorted_peak_indices = stable_sort_index(peak_magnitudes, "descend");
    peak_magnitudes = peak_magnitudes.rows(sorted_peak_indices);
    results = results.rows(sorted_peak_indices);

    if (threshold_method == "magnitude"){
        uvec indices = find(peak_magnitudes > threshold);
        peak_magnitudes = peak_magnitudes.rows(indices);
        return results.rows(indices);
    }
    else if (threshold_method == "count"){
        uword uthresh = (std::ceil(threshold) < results.n_rows - 1 ? std::ceil(threshold) : results.n_rows - 1);
        peak_magnitudes = peak_magnitudes.rows(0, uthresh);
        return results.rows(0, uthresh);
    }
    else if (threshold_method == "ratio"){
        if (threshold == 1){
            peak_magnitudes = peak_magnitudes.row(0);
            return results.row(0);
        }
        else{
            double cutoff = peak_magnitudes(0) * threshold;
            uvec indices = find(peak_magnitudes > cutoff);
            peak_magnitudes = peak_magnitudes.rows(indices);
            return results.rows(indices);
        }
    }
    else{ //(threshold_method == "countpercentage")
        uword count = std::floor(threshold*results.n_rows);
        peak_magnitudes = peak_magnitudes.rows(0, count);
        return results.rows(0, count);
    }

}

///
/// \brief EstimateBaseline
/// \param X Signal
/// \param peaks Positions of peaks to exclude from spectrum to calculate baseline
/// \param window_size Size of local minimum filter applied to baseline
/// \return
///
vec arma_ext::EstimateBaseline(vec X,
                               umat peaks,
                               uword window_size)
{
    if (window_size % 2 == 0){
        window_size--;
        cerr << "invalid window_size, using one less" << endl;
    }

    vec baseline = X;
    uword start, end, center, size;
    //linearize across bases of peaks
    uword i;
    try{
        for (i = 0; i < peaks.n_rows; ++i){
            center = peaks(i, 0);
            start = peaks(i, 1);
            end = peaks(i, 2);
            //cut accross in a straight line when peak is on edge of spectrum
            size = end - start;

            //linearize baseline across base of peak
            // if the start or end of the peak is the center, take a flat line
            // from the minimum
            if (start == center){
                baseline.subvec(start, end-1) = X(end) * ones(size);
            }
            else if (end == center){
                baseline.subvec(start, end-1) = X(start) * ones(size);
            }
            else{
                baseline.subvec(start, end-1) = linspace(X(start), X(end), size);
            }

        }
    }catch(std::exception e){
        cout << "Exception! (peak exclusion)" << endl;
        cout << "size of peaks = " << peaks.n_rows << " " << peaks.n_cols << endl;
        cout << "i = " << i << endl;
        throw std::runtime_error(e.what());
    }

    //apply local minimum filtering to baseline

    uword k = (window_size - 1) / 2; //mid-point of window
    vec buffer;
    vec filtered(baseline.n_elem);
    try{
        for (uword i = k; i < (X.n_rows - k); ++i){
            buffer = baseline.subvec(i-k, i+k);
            filtered(i) = buffer.min();
        }
        //fill edges with first and last values
        for (uword i = 0; i < k; ++i){
            filtered(i) = filtered(k);
        }
        for (uword i = filtered.n_rows - k; i < filtered.n_elem; ++i){
            filtered(i) = filtered(filtered.n_rows - k - 2);
        }
    }catch(std::exception e){
        cout << "Exception! (filtering)" << endl;
        cout << "i = " << i << endl;
        throw std::runtime_error(e.what());
    }

    return filtered;


}


vec arma_ext::cwt_spdbc(vec X, std::string wavelet, uword qscale, double threshold, std::string threshold_method, uword window_size, uvec &peak_positions, vec &baseline)
{
    umat peaks;
    vec peak_magnitudes;
    vec X_transform = arma_ext::cwt(X, wavelet, qscale);
    vec dX_transform = arma_ext::diff(X, 1);
    dX_transform.insert_rows(0, 1, true); //buffer so that X and dX have same
    //number of elements and dX(i) is the derivative of X at i.
    peaks = arma_ext::FindPeakPositions(X_transform, dX_transform,
                                        threshold,
                                        threshold_method,
                                        peak_magnitudes);
    peak_positions = peaks.col(0);
    baseline = arma_ext::EstimateBaseline(X, peaks, window_size);
    return X - baseline;

}


mat arma_ext::cwt_spdbc_Mat(mat X, std::string wavelet, uword qscale,
                            double threshold, std::string threshold_method,
                            uword window_size, field<uvec> &peak_positions,
                            mat &baselines)
{
    baselines.set_size(X.n_rows, X.n_cols);
    vec baseline;
    vec spectrum;
    vec current_corrected;
    mat corrected;
    uvec current_peakpos;
    peak_positions.set_size(X.n_cols);
    for (uword i = 0; i < X.n_cols; ++i){
        spectrum = X.col(i);
        current_corrected = arma_ext::cwt_spdbc(spectrum, wavelet,
                                               qscale, threshold,
                                               threshold_method, window_size,
                                               current_peakpos, baseline);

        peak_positions(i) = current_peakpos;
        baselines.col(i) = baseline;
        corrected.col(i) = current_corrected;
    }
    return corrected;
}
