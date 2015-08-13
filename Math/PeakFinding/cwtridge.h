/*******************************************************************************
    Copyright (C) 2015 Wright State University - All Rights Reserved
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
#ifndef CWTRIDGE_H
#define CWTRIDGE_H
#include <mlpack/core.hpp>
#include "Global/libvespucci.h"
namespace Vespucci{
namespace Math{
class VESPUCCI_EXPORT CWTRidge
{
public:
    CWTRidge(int id);
    arma::umat points() const;
    arma::uword gap() const;
    bool HasPoint(arma::uword row, arma::uword column) const;
    arma::uword scale() const;
    void InsertPoint(arma::uword row, arma::uword column, double value);
    void Merge(CWTRidge &new_ridge);
    void RemoveLastPoint();
    double value(arma::uword row, arma::uword column) const;
    double max_value() const;
    arma::vec coefs() const;
    arma::uword LastPosition() const;
    arma::uword length() const;
    arma::uword PeakCenter() const;
    double EstimateSNR(std::string method, arma::uword window_size, const arma::vec &noise);
    double SNR() const;
    double EstimateWidth(const arma::vec &spectra, const arma::vec &first_haar_coefs, const arma::vec &second_haar_coefs, const arma::vec &abscissa);
    double EstimateWidth(const arma::vec &spectra, const arma::vec &abscissa);
    double width() const;
    int id() const;


    //operator overloads (CWTRidge essentially stores peak centers, so relational
    //operators rely on PeakCenter()
    bool operator >(const Vespucci::Math::CWTRidge &c) const;
    bool operator >(const arma::uword &c) const;

    bool operator >=(const Vespucci::Math::CWTRidge &c) const;
    bool operator >=(const arma::uword &c) const;

    bool operator ==(const Vespucci::Math::CWTRidge &c) const;
    bool operator ==(const arma::uword &c) const;

    bool operator <=(const Vespucci::Math::CWTRidge &c) const;
    bool operator <=(const arma::uword &c) const;

    bool operator <(const Vespucci::Math::CWTRidge &c) const;
    bool operator <(const arma::uword &c) const;

    bool operator !=(const Vespucci::Math::CWTRidge &c) const;
    bool operator !=(const arma::uword &c) const;

private:

    ///
    /// \brief points_
    /// Stores the coordinates of the points in the ridge
    arma::umat points_;

    ///
    /// \brief coefs_
    /// Stores the
    arma::vec coefs_;

    ///
    /// \brief id
    /// A number used by external functions for debugging purposes
    int id_;

    ///
    /// \brief calculated_snr_
    /// Whenever SNR is called, this value is set.
    double calculated_snr_;

    ///
    /// \brief width_
    /// Whenever EstimateWidth is called, this value is set
    double width_;

    arma::uword left_minimum_;
    arma::uword right_minimum_;
};
}
}


#endif // CWTRIDGE_H
