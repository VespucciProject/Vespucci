#ifndef ACCESSORY_H
#define ACCESSORY_H
#include <mlpack/core.hpp>
namespace Vespucci
{
    namespace Math
    {
        //general Math functions
        template <typename T> T diff(T X, arma::uword deriv_order = 1);

        arma::vec ExtendToNextPow(arma::vec X, arma::uword n);
        arma::uword NextPow(arma::uword number, arma::uword power);
        template <typename T> T conv_fft(T &A, T &B, std::string type);

        template <typename T> T rotate(T X, arma::uword shift_by, bool slide_back = true);
        arma::mat spdiags(arma::mat B, arma::ivec d, arma::uword m, arma::uword n);
        arma::mat orth(arma::mat X); //this is implemented better in mlpack


        template <typename T> T min(T a, T b);
        template <typename T> T max(T a, T b);

    }
}
#endif //ACCESSORY_H
