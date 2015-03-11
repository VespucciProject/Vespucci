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
#include <Math/DimensionReduction/dimensionreduction.h>

///
/// \brief Vespucci::MathDimensionReduction::plsregress PLS Regression Using SIMPLS algorithm.
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
bool Vespucci::Math::DimensionReduction::plsregress(arma::mat X, arma::mat Y,
                                                    int components, arma::mat &X_loadings,
                                                    arma::mat &Y_loadings,
                                                    arma::mat &X_scores,
                                                    arma::mat &Y_scores,
                                                    arma::mat &coefficients,
                                                    arma::mat &percent_variance,
                                                    arma::mat &fitted)
{
    using namespace arma;
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
