/*******************************************************************************
    Copyright (C) 2014-2017 Wright State University - All Rights Reserved
    Daniel P. Foose - Maintainer/Lead Developer

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
#ifndef BINARYIMPORT_H
#define BINARYIMPORT_H
#if !defined( SWIG )
    // SWIG should not see #inlcude<armadillo> as it can not handle it
    #include <mlpack/core.hpp>
    #include "libvespucci.h"
#endif

namespace BinaryImport
{
    VESPUCCI_EXPORT bool ImportOldVespucciBinary(std::string filename,
                              arma::mat &spectra,
                              arma::vec &abscissa,
                              arma::vec &x, arma::vec &y);
    VESPUCCI_EXPORT bool ImportVespucciBinary(std::string filename,
                                               arma::mat &spectra,
                                               arma::vec &abscissa,
                                               arma::vec &x, arma::vec &y);
}

#endif // BINARYIMPORT_H
