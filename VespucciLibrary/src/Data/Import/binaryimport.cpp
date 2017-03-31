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
#include "Data/Import/binaryimport.h"
#include "vespucci.h"

#include <H5Cpp.h>
bool BinaryImport::ImportVespucciBinary(std::string filename,
                                        arma::mat &spectra,
                                        arma::vec &abscissa,
                                        arma::vec &x, arma::vec &y)
{
    Vespucci::ResetDataset(spectra, x, y, abscissa);
    using namespace H5;
    bool success;
    try{
        H5File file(filename, H5F_ACC_RDONLY);

        DataSet ds = file.openDataSet("Spectra");
        DataSpace dspace = ds.getSpace();
        hsize_t dims[2];
        dspace.getSimpleExtentDims(dims);
        spectra.set_size(dims[0], dims[1]);
        ds.read(spectra.memptr(), PredType::NATIVE_DOUBLE);

        ds = file.openDataSet("x");
        dspace = ds.getSpace();
        dspace.getSimpleExtentDims(dims);
        if (dims[1] == 1){
            x.set_size(dims[0]);
            ds.read(x.memptr(), PredType::NATIVE_DOUBLE);
        }

        ds = file.openDataSet("y");
        dspace = ds.getSpace();
        dspace.getSimpleExtentDims(dims);
        if (dims[1] == 1) {
            y.set_size(dims[0]);
            ds.read(y.memptr(), PredType::NATIVE_DOUBLE);
        }

        ds = file.openDataSet("Spectral Abscissa");
        dspace = ds.getSpace();
        dspace.getSimpleExtentDims(dims);
        if (dims[1] == 1){
            abscissa.set_size(dims[0]);
            ds.read(abscissa.memptr(), PredType::NATIVE_DOUBLE);
        }

        success = true;
    }catch (...){
        success = false;
    }

    if(success){
        //check to make sure everything is sorted the way Vespucci expects
        arma::uvec sorted_indices = arma::stable_sort_index(abscissa);
        abscissa = abscissa.rows(sorted_indices);
        spectra = spectra.rows(sorted_indices);
    }
    return success;

}

bool BinaryImport::ImportOldVespucciBinary(std::string filename,
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
