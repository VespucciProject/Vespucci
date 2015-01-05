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
    //cout << "p = " << p << endl;
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
        svds(R*trans(R)/N, d, Ud, Sd, Vd); //R_o is a mean centered version...
        x_p = trans(Ud)*R;
        projected_data = Ud * x_p.rows(0, d-1);
        mat x = trans(Ud) * R;
        mat u = mean(x, 1);
        y = x / repmat(sum(x % repmat(u, 1, N)), d, 1);
    }

    // The VCA algorithm
    colvec w;
    w.set_size(p);
    colvec f;
    vec v;
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
    cout << "MedianFilter" << endl;
    uword k = (window_size - 1) / 2;
    cout << "k = " << k << endl;
    vec filtered = X; //copy the whole thing, then add in the bit we actually filter
    cout << "X.n_rows = " << X.n_rows << endl;
    vec buffer;
    cout << "elements in filtered" << filtered.n_elem;
    cout << "k = " << k << endl;
    cout << "X.n_rows - k = " << X.n_rows - k << endl;
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



