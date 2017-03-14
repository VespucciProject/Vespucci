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

#include "vespucci.h"
#include <QtSvg>
#include <cstdlib>
#include <QProcess>
#include <climits>


///
/// \brief Vespucci::SaveVespucciBinary
/// \param spectra
/// \param x
/// \param y
/// \param abscissa
/// \return
///
bool Vespucci::SaveOldVespucciBinary(std::string filename, const arma::mat &spectra, const arma::vec &x, const arma::vec &y, const arma::vec &abscissa)
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

///
/// \brief Vespucci::StitchDatasets
/// \param datasets
/// \param spectra Concatenated spectra
/// \param x Concatenated x
/// \param y Concatenated y
/// \param abscissa The spectral abscissa
/// \return Whether or not stitching operation was successful.
///
/// Each dataset must have the following format:
/// dataset(0) = spectra;
/// dataset(1) = abscissa;
/// dataset(2) = x;
/// dataset(3) = y;
/// Datasets will be concatenated by "row blocks", which correspond to rows in
/// datasets.
///
/// Each row block must have the same number of unique y values.
/// Every dataset in a row block must have the same number of unique x values.
/// Each dataset must have the same number of elements in the abscissa.
/// If the above criteria are not met, the matrices are reset and the function
/// returns false.
///
/// The spatial resolution is assumed to be identical for all datasets and is
/// taken to be the difference between the largest and second-largest unique
/// values in datasets(0,0). The spatial resolution is assumed to be the same
/// for both x and y.
///
/// Row blocks are built from left to right. The dataset is constructed by
/// layering row blocks from the bottom to the top.
///
/// For each dataset in a row block, x values are offset so that the minimum x
/// value of a dataset is higher by one spatial resolution unit than the
/// maximum of the previous dataset in the row block.
/// Y values are offset by row block so that the minimum y value of a row block
/// is higher by one spatial resolution unit than the maximum value of the
/// previous block.

bool Vespucci::StitchDatasets(const arma::field<arma::field<arma::mat> > &datasets,
                              arma::mat &spectra, arma::vec &x, arma::vec &y,
                              arma::vec &abscissa)
{
    Vespucci::ResetDataset(spectra, x, y, abscissa);
    //set abscissa to be the first in the field
    abscissa = datasets(0,0)(1);

    //determine spatial resolution.
    //due to rounding errors, this may not be the same for all points in a dataset
    //to armadillo's degree of precision, but will be close. If drastically different
    //color maps will be rendered meaningless but spectral data will still be valid
    arma::mat first_x = datasets(0, 0)(2);
    arma::mat first_y = datasets(0, 0)(3);


    arma::vec unique_x = arma::unique(first_x);
    arma::vec unique_y = arma::unique(first_y);
    double spatial_resolution;
    if (unique_x.n_rows < 2){
        if (unique_y.n_rows > 1){
            spatial_resolution =
                    unique_y(unique_y.n_rows - 1)
                    - unique_y(unique_y.n_rows - 2);
        }
        else{return false;}
    }
    else{
        spatial_resolution =
                unique_x(unique_x.n_rows - 1) - unique_x(unique_x.n_rows - 2);
    }

    arma::uword n_unique_y = 0;//per row block in new dataset
    arma::uword row_block_n_unique_y = 0;
    //building bottom to top, left to right
    for (size_t i = datasets.n_rows - 1; i+1 >= 1; --i){
        row_block_n_unique_y = 0;
        unique_x = arma::unique(datasets(i, 0)(2));
        arma::uword n_unique_x = unique_x.n_rows;
        arma::mat row_block_spc;
        arma::vec row_block_x;
        arma::vec row_block_y;
        //iterate over datasets in this row block
        for (arma::uword j = 0; j < datasets.n_cols; ++j){
            arma::uword abs_ct = datasets(i,j)(1).n_rows;
            //ensure validity of spectra for this abscissa
            if (datasets(i,j)(0).n_rows != abs_ct){
                Vespucci::ResetDataset(spectra, x, y, abscissa);
                return false;
            }
            //check validity of spatial data
            unique_x = arma::unique(datasets(i,j)(2));
            unique_y = arma::unique(datasets(i,j)(3));
            if (unique_x.n_rows != n_unique_x){
                Vespucci::ResetDataset(spectra, x, y, abscissa);
                return false;
            }
            row_block_n_unique_y += unique_y.n_rows;


            //we're shifting values so that 0,0 is local origin
            //lowest value of x and y globally will be zero
            arma::vec new_x = datasets(i,j)(2) - datasets(i,j)(2).min()*arma::ones(datasets(i,j)(2).n_rows);
            arma::vec new_y = datasets(i,j)(3) - datasets(i,j)(3).min()*arma::ones(datasets(i,j)(3).n_rows);

            //x offset is equal to the current_maximum
            double x_offset = (row_block_x.n_elem ?
                                   row_block_x.max() + spatial_resolution :
                                   0);
            new_x = datasets(i,j)(2) + x_offset*arma::ones(datasets(i,j)(2).n_rows);

            //add to row block
            if (!row_block_x.n_elem){row_block_x = new_x;}
            else{row_block_x = arma::join_vert(row_block_x, new_x);}
            if (!row_block_y.n_elem){row_block_y = new_y;}
            else{row_block_y = arma::join_vert(row_block_y, new_y);}
            if (!row_block_spc.n_elem){row_block_spc = datasets(i,j)(0);}
            else{row_block_spc = arma::join_horiz(row_block_spc, datasets(i,j)(0));}
        }//datasets in row block
        //offset y values in this row block;

        double y_offset = (y.n_elem ?
                               y.max() - row_block_y.min() + spatial_resolution :
                               0);
        row_block_y = row_block_y + y_offset*arma::ones(row_block_y.n_rows);

        //if this is the first row block...
        if (i == datasets.n_rows - 1){n_unique_y = row_block_n_unique_y;}

        //check validity of this row block relative to bottom block
        if (row_block_n_unique_y != n_unique_y){
            Vespucci::ResetDataset(spectra, x, y, abscissa);
            return false;
        }

        //add this row block to the new dataset
        if (!spectra.n_cols){spectra = row_block_spc;}
        else{spectra = arma::join_horiz(spectra, row_block_spc);}
        if (!x.n_rows){x = row_block_x;}
        else{x = arma::join_vert(x, row_block_x);}
        if (!y.n_rows){y = row_block_y;}
        else{y = arma::join_vert(y, row_block_y);}

    }//row blocks in new dataset

    //recenter spatial data to put origin at center of spatial data
    //x = x - arma::median(x)*arma::ones(x.n_rows);
    //y = y - arma::median(y)*arma::ones(y.n_rows);

    return true;
}

void Vespucci::ResetDataset(arma::mat &spectra, arma::vec &x, arma::vec &y, arma::vec &abscissa)
{
    spectra.reset();
    x.reset();
    y.reset();
    abscissa.reset();
}

///
/// \brief Vespucci::CleanString
/// \param in
/// \return
/// Remove any tabs or commas from an input string so armadillo parses it properly.
/// This function is probably not necessary
std::string Vespucci::CleanString(const std::string &in)
{
    std::string out;
    for (const char c : in){
        if (c == '.' || c >= '0' || c <= '9') out.append(&c);
        else out.append(" ");
    }
    return out;
}
