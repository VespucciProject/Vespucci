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
#include <External/R/VespucciR.h>

VespucciR::VespucciR(int argc, char *argv[]) : R_(argc, argv)
{

}

///
/// \brief VespucciR::SetEnvironment Sets the initial set of variables defined in the R environment
/// \param x 1-column matrix of horizontal spatial position
/// \param y 1-colum matrix of vertical spatial position
/// \param abscissa 1-column matrix containing spectral abscissa
/// \param spectra Column having the same number of rows as the abscissa and the
/// same number of columns as x and y have rows. Contains spectral data.
///
/// The nth row of x and y correspond to the nth column of spectra.
/// The nth row of abscissa corresponds to the nth row of spectra. Spectra will
/// be transposed within the R environment in keeping with R's convention of
/// samples in rows and observations in columns.
///
void VespucciR::SetEnvironment(arma::mat &x, arma::mat &y, arma::mat &abscissa, arma::mat &spectra)
{

}

///
/// \brief VespucciR::SetEnvironment Sets the initial set of variables defined in the R environment
/// \param objects Contains a copy of all the dataset elements to be sent to R.
/// Only matrix and numeric types can be sent.
///
void VespucciR::SetEnvironment(arma::field<arma::mat> &objects)
{

}

///
/// \brief VespucciR::GetEnvironment Returns potentially modified variables from
/// the R environment.
/// \param x
/// \param y
/// \param abscissa
/// \param spectra
///
void VespucciR::GetEnvironment(arma::mat &x, arma::mat &y, arma::mat &abscissa, arma::mat &spectra)
{

}

///
/// \brief VespucciR::GetEnvironment Returns potentially modified variables from
/// the R environment.
/// \param objects Contains a copy of all the dataset elements to be sent to R
/// Only matrix and numeric types can be sent
///
void VespucciR::GetEnvironment(arma::field<arma::mat> &objects)
{

}
