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
#include "Data/Import/binaryimport.h"
bool BinaryImport::ImportVespucciBinary(std::string filename,
                                        arma::mat &spectra,
                                        arma::vec &wavelength,
                                        arma::vec &x, arma::vec &y)
{
    arma::field<arma::mat> input_data;
    bool success = input_data.load(filename);
    std::cout << (success ? "success" : "failure") << std::endl;
    if(success){
        spectra = input_data(0);
        arma::mat wl= input_data(1);
        arma::mat xm = input_data(2);
        arma::mat ym = input_data(3);

        //field subview doesn't properly convert to vec forms
        wavelength = wl;
        x = xm.col(0);
        y = ym.col(0);
    }
    return success;
}
