#include "arma_ext.h"

///
/// \brief spdiags analgous to the MATLAB function A = spdiags(B, d, m, n).
/// \param B a matrix containing the new diagonal vectors as columns
/// \param d a vector containing the row numbers to set.  The first column
/// vector of B corresponds to the first entry in d.
/// \param m the number of rows of the output matrix
/// \param n the number of columns of the output matrix
/// \return a m by n sparase matrix with the columns of B as diagonals
/// A translation of the MATLAB function A = spdiags(B, d, m, n).  For
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

mat arma_ext::svds(mat X, int k, std::string form)
{
    sp_mat B;
    int m = X.n_rows;
    int n = X.n_cols;
    int dimension = m + n;
    B.set_size(dimension, dimension);
    B.submat(0, m-1, m, dimension) = X;
    B.submat(m, dimension, 0, n-1) = X.t();
    cx_vec eigval;
    cx_mat eigvec;
    //eigs_gen(eigval, eigvec, X, k, form);
    return mat();

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
