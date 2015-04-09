#ifndef CWTRIDGE_H
#define CWTRIDGE_H
#include <mlpack/core.hpp>
namespace Vespucci{
namespace Math{
class CWTRidge
{
public:
    CWTRidge();
    arma::uword gap();
    bool HasPoint(arma::uword row, arma::uword column);
    arma::uword scale();
    void InsertPoint(arma::uword row, arma::uword column, double value);
    void RemoveLastPoint();
    double value(arma::uword row, arma::uword column);
    arma::vec coefs();
    arma::uword LastPosition();
    arma::uword length();
    arma::uword PeakCenter();
private:
    arma::umat points_;
    arma::vec coefs_;
};
}
}


#endif // CWTRIDGE_H
