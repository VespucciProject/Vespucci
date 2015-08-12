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

#ifndef TEXTIMPORTQPD_H
#define TEXTIMPORTQPD_H
#include "Global/vespucciworkspace.h"
#include "Data/Import/textimport.h"
namespace TextImport{
    bool ImportLongText(QString filename,
                        arma::mat &spectra,
                        arma::vec &wavelength,
                        arma::vec &x, arma::vec &y,
                        bool swap_spatial,
                        QProgressDialog *progress);

    bool ImportWitecText(QString x_filename,
                         QString y_filename,
                         QString spectra_filename,
                         arma::mat &spectra,
                         arma::vec &wavelength,
                         arma::vec &x, arma::vec &y,
                         bool swap_spatial,
                         QProgressDialog *progress);

    bool ImportWideText(QString filename,
                        arma::mat &spectra,
                        arma::vec &wavelength,
                        arma::vec &x, arma::vec &y,
                        bool swap_spatial,
                        QProgressDialog *progress,
                        const QString sep);
}

#endif // TEXTIMPORTQPD_H

