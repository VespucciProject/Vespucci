#ifndef ACCESSORY_H
#define ACCESSORY_H
#include <mlpack/core.hpp>
#include "Math/Accessory/accessory_impl.h"
namespace Vespucci
{
    namespace Math
    {
        //general Math functions
        arma::sp_mat LocalMaxima(arma::mat X);

        arma::vec ExtendToNextPow(arma::vec X, arma::uword n);
        arma::uword NextPow(arma::uword number, arma::uword power);

        arma::mat spdiags(arma::mat B, arma::ivec d, arma::uword m, arma::uword n);
        arma::mat orth(arma::mat X); //this is implemented better in mlpack


        arma::uword min(arma::uword a, arma::uword b);
        arma::uword max(arma::uword a, arma::uword b);

        void position(arma::uword index,
                      arma::uword n_rows, arma::uword n_cols,
                      arma::uword &i, arma::uword &j);
        arma::umat to_row_column(arma::uvec indices, arma::uword n_rows, arma::uword n_cols);
    }
}


#endif //ACCESSORY_H
