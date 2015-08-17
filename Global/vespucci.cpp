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

#include "Global/vespucci.h"
#include <QtSvg>
#include <minizip/zip.h>
#include <cstdlib>
#include <QProcess>


///
/// \brief Vespucci::SaveVespucciBinary
/// \param spectra
/// \param x
/// \param y
/// \param abscissa
/// \return
///
bool Vespucci::SaveVespucciBinary(std::string filename, const arma::mat &spectra, const arma::vec &x, const arma::vec &y, const arma::vec &abscissa)
{
    bool success;
    try{
        arma::field<arma::mat> dataset(4);
        dataset(0) = spectra;
        dataset(1) = abscissa;
        dataset(2) = x;
        dataset(3) = y;
        success = dataset.save(filename, arma::arma_binary);
    }
    catch(std::exception e){
        std::cerr << "See armadillo exception" << std::endl;
        std::string str = "Vespucci::SaveVespucciBinary: " + std::string(e.what());
        throw std::runtime_error(str);
    }
    return success;
}





bool Vespucci::SaveText(std::string basename, const arma::mat &spectra, const arma::vec &x, const arma::vec &y, const arma::vec &abscissa, arma::file_type type)
{
    std::string extension;
    switch (type){
    case arma::csv_ascii:
        extension = ".csv";
    case arma::raw_ascii: default:
        extension = ".txt";
    }
    bool x_success, y_success, spectra_success, abscissa_success;
    spectra_success = spectra.save(basename + "_spectra" + extension, type);
    abscissa_success = abscissa.save(basename + "_abscissa" + extension, type);
    x_success = x.save(basename + "_x" + extension, type);
    y_success = y.save(basename + "_y" + extension, type);
    return (x_success && y_success && spectra_success && abscissa_success);
}
