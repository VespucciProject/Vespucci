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
#include "Global/vespucci.h"
bool BinaryImport::ImportVespucciBinary(std::string filename,
                                        arma::mat &spectra,
                                        arma::vec &abscissa,
                                        arma::vec &x, arma::vec &y)
{
    Vespucci::ResetDataset(spectra, x, y, abscissa);
    arma::field<arma::mat> input_data;
    bool success = input_data.load(filename);
    std::cout << (success ? "success" : "failure") << std::endl;
    if(success){
        spectra = input_data(0);
        abscissa = input_data(1);
        x = input_data(2);
        y = input_data(3);
    }
    return success;
}
