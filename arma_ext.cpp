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
/// \brief orth Returns an orthonormal basis of the range space of A
/// \param X matrix
/// \return a matrix whose columns form an orthonormal basis for X
///
mat arma_ext::orth(mat X)
{
    //cout << "orth()" << endl;
    double eps = datum::eps;
    mat U, V;
    vec s;
    svd(U, s, V, X);
    //cout << "s=" << s << endl;
    double tol;
    tol = std::max(X.n_rows*s(1)*eps, X.n_cols*s(1)*eps);
    uvec q1 = find(s > tol);
    double rank = q1.n_elem;

    if (rank > 0){
        //cout << "end orth" << endl;
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
/// largest singular values (none of that sigma business).
/// To decompose the matrix X into the form \tilde{X} = U_{k}{\Sigma}_{k}{V_{k}}*
/// A sparse matrix B is constructed of the form:
/// \f[
/// \begin{pmatrix}
/// 0 & X \\
/// X' & 0
/// \end{pmatrix}
/// \f]
///
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

bool arma_ext::svds(mat X, unsigned int k, mat &U, vec &s, mat &V)
{
    if (X.n_cols < 2){
        cerr << "svds: X must be 2D matrix" << endl;
        //throw SVDS_Dimension_X;
        return false;
    }

    double root_2 = sqrt(2.0);
    double tolerance = 1e-10 / root_2; //tolerance for convergence (ARPACK default)
    double epsilon = datum::eps; //eps in Octave and MATLAB
    unsigned int m = X.n_rows;
    unsigned int n = X.n_cols;
    unsigned int p = std::min(m, n); //used below to establish tolerances.
    unsigned int q = std::max(m, n);
    //cout << "p = " << p << endl;
    //cout << "k = " << k << endl;
    if (k > p){
        if (k > m)
            cerr << "svds: value of k " << k << "is greater than number of rows " << m << endl;
        else
            cerr << "svds: value of k" << k << "is greater than number of columns " << n << endl;
    }

    k = std::min(p, k); //make sure value of k is less than smallest dimension

    sp_mat B(m+n, m+n);
    B.submat(span(0, m-1), span(m, m+n-1)) = X; // top right corner
    B.submat(span(m, m+n-1), span(0, m-1)) = X.t(); //bottom left corner

    //mat B_full(B.n_rows, B.n_cols);
    //for(int i = 0; i < B.n_rows; ++i)
    //    for (int j = 0; j < B.n_cols; ++j)
    //        B_full(i,j) = B(i, j); //used to test copying of matrix
    //B_full.save("B.csv", csv_ascii);

    //If, for some reason, a matrix of zeroes is input...
    if (B.n_nonzero == 0){
        U = eye(m, k);
        s = zeros(k, k);
        V = eye(n, k);
        cerr << "svds: input matrix has no non-zero elements" << endl;
        //throw SVDS_Zeros;
        return false;
    }

    vec eigval;
    mat eigvec;

    //because we're using sigma=0, results of eigs will be centered around 0
    //we throw out the negative ones, then add in 0 eigenvalues if we have to
    bool eigs_success = eigs_sym(eigval, eigvec, B, k*2);
    //cout << "eigval=" << eigval << endl;
    //cout << "eigvec=" << eigvec << endl;

    double eigval_max = eigval.max();



    if (!eigs_success){
        cerr << "svds: arma::eigs_sym did not converge!" << endl;
    }

    //Manipulate the results

    //vector of indices of the eigenvalues, sorted in descending order
    uvec sorted_eigval_indices = sort_index(eigval, "descend");

    //sort eigvec from largest to smallest
    eigvec = eigvec.cols(sorted_eigval_indices);

    double d_tolerance, uv_tolerance; //tolerances for D and U, V
    if (eigs_success){
        d_tolerance = q*n*sqrt(epsilon);
        uv_tolerance = m*sqrt(sqrt(epsilon));
    }
    else{
        d_tolerance = q*eigval_max*epsilon;
        uv_tolerance = m*sqrt(epsilon);

    }

    //cout << "UU" << endl;
    //mat UU = trans(eigvec.rows(0, m-1)) * eigvec.rows(0, m-1);
    //vec diag_UU = UU.diag();
    //cout << "VV" << endl;
    //mat VV = trans(eigvec.rows(m, m+n-1)) * eigvec.rows(m, m+n-1);
    //vec diag_VV = VV.diag();

    //cout << "eigvals:" << endl;
    //cout << eigval << endl;
    //find all indices that satisfy tolerances
    //See the Octave documentation for svds:
    // We wish to exclude all eigenvalues that are less than zero as these
    // are artifacts of the way the matrix passed to eigs is formed. There
    // is also the possibility that the value of sigma chosen is exactly
    // a singular value, and in that case we're dead!! So have to rely on
    // the warning from eigs. We exclude the singular values which are
    // less than or equal to zero to within some tolerance scaled by the
    // norm since if we don't we might end up with too many singular
    // values.

    //cout << "call to find()" << endl;
    uvec query = find((eigval > d_tolerance) /*&& (abs(diag_UU) > uv_tolerance) && (abs(diag_VV) > uv_tolerance)*/);
    //cout << query << endl;


    unsigned int number_of_indices = query.n_elem;
    int end = std::min(k, number_of_indices) - 1;
    //cout << "end=" << end;
    //cout << "query.n_elem=" << query.n_elem << endl;


    uvec return_indices = query.subvec(0, end);
    //cout << "return_indices " << return_indices << endl;
    U = sqrt(2) * (eigvec.rows(0, m-1));
    U = U.cols(return_indices);

    s = eigval.rows(return_indices);

    V = sqrt(2) * eigvec.rows(m, m+n-1);
    V = V.cols(return_indices);
    //cout << "return_indices.n_elem=" << return_indices.n_elem << " k=" << k << endl;
    //B may have some eigenvalues that are 0 that needed to be included if the
    //number of non_zero eigenvalues is too small. Will only add eigenvalues in if
    //they exist (obviously).
    if (return_indices.n_elem < k){
        uvec zero_indices = find(abs(eigval) <= d_tolerance);
        if (zero_indices.n_elem != 0){
            mat eigvec_sub1 = eigvec.rows(0, m-1);
            mat eigvec_sub2 = eigvec.rows(m, m+n-1);
            mat QWU = orth(eigvec_sub1.cols(zero_indices));
            mat QWV = orth(eigvec_sub2.cols(zero_indices));
            int n_zero = std::min(QWV.n_cols, std::min(QWU.n_cols, (return_indices.n_elem)));

            //necessary number of zero eigenvalues
            vec zero_eigvals = abs(eigval.cols(zero_indices.rows(0, n_zero)));
            //cout <<"n_zero" << n_zero << endl;
            U.insert_cols(U.n_cols, QWU.cols(0, n_zero));
            s.insert_rows(s.n_rows, zero_eigvals);
            V.insert_cols(V.n_cols, QWV.cols(0, n_zero));
        }
    }


    uvec indices = sort_index(s, "descend");
    s = sort(s, "descend");
    //cout << "s = " << endl << s << endl;
    //cout << "diagmat(s) = " << endl << diagmat(s) << endl;

    U = U.cols(indices);
    V = V.cols(indices);
    //test for convergence

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
    cout << "end of plsregress" << endl;
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
bool arma_ext::VCA(mat R, unsigned int p,
         uvec &indices, mat &endmember_spectra,
         mat &projected_data, mat &fractional_abundances)
{
//Initializations
    unsigned int L = R.n_rows;
    unsigned int N = R.n_cols;
    if (L == 0 || N == 0){
        cerr << "No data!" << endl;
        return false;
    }

    if (p < 0 || p > L){
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
    Ud.save("Ud.csv", csv_ascii);
    mat x_p = trans(Ud) * R_o;
    //cout <<"SNR"<<endl;
    double SNR = estimate_snr(R, r_m, x_p);
    //cout <<"end SNR"<<endl;
    double SNR_th = 15 + 10*log10(p);

//Choose projective projection or projection to p-1 subspace
    mat y;
    if (SNR < SNR_th){
        //cout << "SNR < SNR_th" << endl;
        unsigned int d = p - 1;
        //cout << "Ud.n_cols=" << Ud.n_cols << endl;
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
        //cout << "SNR !< SNR_th" << endl;
        unsigned int d = p;
        svds(R*trans(R)/N, d, Ud, Sd, Vd); //R_o is a mean centered version...
        x_p = trans(Ud)*R;
        projected_data = Ud * x_p.rows(0, d-1);
        mat x = trans(Ud) * R;
        mat u = mean(x, 1);
        y = x / repmat(sum(x % repmat(u, 1, N)), d, 1);
    }

    // The VCA algorithm
    //cout << "VCA Algorithm" << endl;
    colvec w;
    w.set_size(p);
    colvec f;
    rowvec v;
    indices.set_size(p);
    //there are no fill functions for uvecs
    for (unsigned int i = 0; i < p; ++i)
        indices(i) = 0;
    mat A = zeros(p, p);
    double v_max;
    double sum_squares;
    uvec q1;
    A(p-1, 0) = 1;
    for (unsigned int i = 0; i < p; ++i){
        //cout << "i=" << i << endl;
        w.randu();
        f = w - A*pinv(A)*w;
        sum_squares = sqrt(sum(square(f)));
        f /= sum_squares;
        v = trans(f) * y;
        v_max = max(abs(v));
        q1 = find(abs(v) == v_max, 1);
        indices(i) = q1(0);
        //cout<<"A.col"<<endl;
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
    unsigned int L = R.n_rows;
    unsigned int N = R.n_cols;
    unsigned int p = x.n_rows;
    if (x.n_cols != N){
        cerr << "invaliid input to estimate_snr" << endl;
        return 0;
    }

    double P_y = accu(arma::square(R))/N;
    double P_x = accu(arma::square(x))/N + dot(r_m, r_m);
    return 10*log10( (P_x - p/L*P_y)/(P_y - P_x) );
}
