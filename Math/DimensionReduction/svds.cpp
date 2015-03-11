#include <Math/DimensionReduction/dimensionreduction.h>
///
/// \brief Vespucci::Math::DimensionReduction::svds Finds a few largest singular values of the arma::matrix X.
/// This is based on the arma::matlab/Octave function svds(), a truncated singular
/// value decomposition. This is designed to only take the kinds of inputs
/// Vespucci will need (It only works on arma::mat types, and only returns the k
/// largest singular values (none of that sigma business). U and V tolerances
/// are not defined currently due to difficulties with Armadillo's find() method
/// A sparse arma::matrix [0 X; X.t() 0] is formed.
/// The eigenvalues of this arma::matrix are then found using arma::eigs_sym, a wrapper
/// for ARPACK.
/// If X is square, it can be "reconstructed" using X = U*diagarma::mat(s)*V.t(). This
/// "reconstruction" will have lower noise.
/// \param X An m \times n arma::matrix.
/// \param k The number of eigenvalues to be found.
/// \param U An m \times k unitary arma::matrix
/// \param s a vector with n
/// \param V
/// \return Whether or not algorithm converged.

bool Vespucci::Math::DimensionReduction::svds(arma::mat X, arma::uword k, arma::mat &U, arma::vec &s, arma::mat &V)
{
    if (X.n_cols < 2){
        std::cerr << "svds: X must be 2D arma::matrix" << std::endl;
        return false;
    }

    arma::uword m = X.n_rows;
    arma::uword n = X.n_cols;
    arma::uword p = (m < n ? m : n);
    //arma::uword p = Vespucci::Math::min(m, n); //used below to establish tolerances.
    //arma::uword q = Vespucci::Math::max(m, n);

    if (k > p){
        if (k > m)
            std::cerr << "svds: value of k " << k << "is greater than number of rows " << m << std::endl;
        else
            std::cerr << "svds: value of k" << k << "is greater than number of columns " << n << std::endl;
    }

    k = (p < k ? p : k);
            //Vespucci::Math::min(p, k); //make sure value of k is less than smallest dimension

    arma::sp_mat B(m+n, m+n);
    B.submat(arma::span(0, m-1), arma::span(m, m+n-1)) = X; // top right corner
    B.submat(arma::span(m, m+n-1), arma::span(0, m-1)) = X.t(); //bottom left corner
    //If, for some reason, a arma::matrix of zeroes is input...
    if (B.n_nonzero == 0){
        U = arma::eye(m, k);
        s = arma::zeros(k, k);
        V = arma::eye(n, k);
        std::cerr << "svds: input arma::matrix has no non-zero elements" << std::endl;
        return false;
    }

    arma::vec eigval;
    arma::mat eigvec;

    //because we're using sigma=0, results of eigs will be centered around 0
    //we throw out the negative ones, then add in 0 eigenvalues if we have to
    //trying some weird stuff to figure out why arpack call fails
    bool eigs_success = eigs_sym(eigval, eigvec, B, 2*k);

    if (!eigs_success){
        std::cerr << "svds: arma::eigs_sym did not converge!" << std::endl;
    }

    //Manipulate the results

    //sort eigvec from largest to smallest:
    eigvec = eigvec.cols(sort_index(eigval, "descend"));

    //the negative eigenvalues are artifacts of how the arma::matrix is constructed
    //it is possible that there are two 0 eigenvalues. Only the first is taken
    //The octave and arma::matlab svds routines are a little more precise with this...
    //It is highly unlikely that a 0 eigenvalue will occur with the types of data
    //processed by Vespucci
    arma::uvec return_indices = find(eigval >= 0, k, "first");

    U = sqrt(2) * (eigvec.rows(0, m-1));
    U = U.cols(return_indices);

    s = eigval.rows(return_indices);

    V = sqrt(2) * eigvec.rows(m, m+n-1);
    V = V.cols(return_indices);

    arma::uvec indices = sort_index(s, "descend");
    s = sort(s, "descend");

    U = U.cols(indices);
    V = V.cols(indices);
    return eigs_success;
}
