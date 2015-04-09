#include "cwtridge.h"
#include "Math/VespucciMath.h"
Vespucci::Math::CWTRidge::CWTRidge()
{
}

///
/// \brief Vespucci::Math::CWTRidge::InsertPoint
/// \param row
/// \param column
/// \param value
/// Add a pair to the list of ridge positions
void Vespucci::Math::CWTRidge::InsertPoint(arma::uword row, arma::uword column, double value)
{
    arma::uvec new_row(2);
    new_row(0) = row;
    new_row(1) = column;
    points_.insert_rows(points_.n_rows, new_row.t());
    coefs_ << value;
}

///
/// \brief Vespucci::Math::CWTRidge::RemoveLastPoint
/// remove the last added point (in case gap is too high)
void Vespucci::Math::CWTRidge::RemoveLastPoint()
{
    if (points_.n_rows > 0)
        points_.shed_row(points_.n_rows - 1);
}

///
/// \brief Vespucci::Math::CWTRidge::HasPoint
/// \param row
/// \param column
/// \return Whether or not a point with coordinates given exists
///
bool Vespucci::Math::CWTRidge::HasPoint(arma::uword row, arma::uword column)
{
    arma::uvec row_ind = arma::find(points_.col(0) == row && points_.col(1) == column);
    return (row_ind.n_elem > 0 ? true : false);
}

///
/// \brief Vespucci::Math::CWTRidge::value
/// \param row
/// \param column
/// \return The CWT coefficient at a particular row/column pair
///
double Vespucci::Math::CWTRidge::value(arma::uword row, arma::uword column)
{
    arma::uvec ind = arma::find(points_.col(0) == row && points_.col(1) == column);
    return (ind.n_elem > 0 ? coefs_(ind(0)) : std::nan(""));
}

///
/// \brief Vespucci::Math::CWTRidge::coefs
/// \return A vector consisting of the CWT coefficients of this ridge
///
arma::vec Vespucci::Math::CWTRidge::coefs()
{
    return coefs_;
}

///
/// \brief Vespucci::Math::CWTRidge::LastPosition
/// \return The position most recently added (if checking for gap)
///
arma::uword Vespucci::Math::CWTRidge::LastPosition()
{
    if (points_.n_rows == 0)
        return 0;
    else
        return points_(points_.n_rows-1);
}

///
/// \brief Vespucci::Math::CWTRidge::length
/// \return The ridge length (number of points in the ridge)
///
arma::uword Vespucci::Math::CWTRidge::length()
{
    return coefs_.n_rows;
}

///
/// \brief Vespucci::Math::CWTRidge::PeakCenter
/// \return The column number of the center of the peak (point with highest coefficient)
///
arma::uword Vespucci::Math::CWTRidge::PeakCenter()
{
    arma::uword scl = this->scale();
    arma::uvec ind = arma::find(((arma::uvec) points_.col(0)) == scl);
    return points_(ind(0),1);
}

///
/// \brief Vespucci::Math::CWTRidge::scale
/// \return The ideal scale (proportional to peak width) of this ridge
///
arma::uword Vespucci::Math::CWTRidge::scale()
{
    arma::uvec max_indices = arma::find(coefs_ == coefs_.max());
    return max_indices(0);
}

///
/// \brief Vespucci::Math::CWTRidge::gap
/// \return The maximum difference between two adjacent points on the ridge
///
arma::uword Vespucci::Math::CWTRidge::gap()
{
    arma::uvec gaps = Vespucci::Math::diff((arma::uvec) points_.col(0));
    return gaps.max(); //maximum difference between two rows
}

