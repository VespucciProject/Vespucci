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
#ifndef TEXTIMPORT_H
#define TEXTIMPORT_H

#if !defined( SWIG )
    // SWIG should not see #inlcude<armadillo> as it can not handle it
    #include <armadillo>
    #include "Global/libvespucci.h"
#endif
#include <map>

namespace TextImport
{

    VESPUCCI_EXPORT bool ImportWideText(std::string filename,
                        arma::mat &spectra,
                        arma::vec &abscissa,
                        arma::vec &x, arma::vec &y,
                        bool swap_spatial);

    VESPUCCI_EXPORT bool ImportMultiplePoints(std::map<std::pair<int,int>, std::string> filenames,
                              int rows,
                              int cols,
                              arma::mat &spectra,
                              arma::vec &abscissa,
                              arma::vec &x, arma::vec &y);

    VESPUCCI_EXPORT bool CheckFileValidity(QString filename, bool &comma_decimals);
}

#endif // TEXTIMPORT_H
