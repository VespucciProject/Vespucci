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
#include "Data/Import/textimport.h"
#include "vespucci.h"
#include <regex>
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>
#include <string>
#include <iostream>

using namespace arma;
///
/// \brief TextImport::CheckFileValidity
/// \param filename
/// \param comma_decimals
/// \return
///
bool TextImport::CheckFileValidity(std::string filename, bool &comma_decimals)
{
    std::ifstream filestream(filename);
    std::string line;
    std::getline(filestream, line);
    std::getline(filestream, line); //start from second line in case of single line header
    std::regex comma(",");
    std::regex tab("\\t");
    std::sregex_token_iterator tab_it(line.begin(), line.end(), tab, -1);
    std::sregex_token_iterator comma_it(line.begin(), line.end(), comma, -1);
    std::sregex_token_iterator end;
    unsigned int tab_count = std::distance(tab_it, end);
    unsigned int comma_count = std::distance(comma_it, end);
    if(tab_count <= 0 && comma_count <= 0){
        return false;
    }
    else if(tab_count > 0 && comma_count > 0){
        //we probably have a tab delimited file with commas for decimal points
        //import function will probabably throw exception if comma is separator
        //and type is wide text, so comma_decimals must be checked
        comma_decimals = true;
        return true;
    }
    else if(tab_count > 0 && comma_count == 0){
        comma_decimals = false;
        while(tab_it != end){
            try{
                double x = boost::lexical_cast<double>(*tab_it);
                tab_it++;
            }catch(...){return false;}
        }
        return true;
    }
    else if(tab_count == 0 && comma_count > 0){
        comma_decimals = false; //we don't know if this is true or not.
        //users using instruments that save csv files with local number formatting
        //in regions where commas are used for decimal places should be super
        //careful.
        while(comma_it != end){
            try{
                double x = boost::lexical_cast<double>(*comma_it);
                comma_it++;
            }catch(...){return false;}
        }
       return true;
    }
    return false;
}


///
/// \brief TextImport::ImportWideText
/// \param filename
/// \param spectra
/// \param abscissa
/// \param x
/// \param y
/// \param swap_spatial
/// \return
/// May throw exceptions or give improper results. Not intended for use in GUI
/// programs. See textimportqpd.h in Vespucci
bool TextImport::ImportWideText(std::string filename,
                                arma::mat &spectra,
                                arma::vec &abscissa,
                                arma::vec &x, arma::vec &y,
                                bool swap_spatial)
{
    bool ok;
    Vespucci::ResetDataset(spectra, x, y, abscissa);
    std::ifstream file(filename);
    std::string first_line;
    std::getline(file, first_line);
    first_line.erase(0, 2); //remove leading empty elements

    abscissa = mat(first_line).t();

    ok = spectra.load(file, raw_ascii);
    if (!ok){
        Vespucci::ResetDataset(spectra, x, y, abscissa);
        return false;
    }

    if (swap_spatial){
        x = spectra.col(1);
        y = spectra.col(0);
    }
    else{
        x = spectra.col(0);
        y = spectra.col(1);
    }

    spectra.shed_cols(0, 1);
    arma::inplace_trans(spectra);

    //check to make sure everything is sorted the way Vespucci expects
    arma::uvec sorted_indices = arma::stable_sort_index(abscissa);
    abscissa = abscissa.rows(sorted_indices);
    spectra = spectra.rows(sorted_indices);
    if (swap_spatial){

    }

    return true;
}

///
/// \brief TextImport::ImportMultiplePoints
/// \param filenames
/// \param rows
/// \param cols
/// \param spectra
/// \param abscissa
/// \param x
/// \param y
/// \return
///
bool TextImport::ImportMultiplePoints(std::map<std::pair<int, int>, std::string> filenames,
                                      int rows, int cols,
                                      arma::mat &spectra,
                                      arma::vec &abscissa,
                                      arma::vec &x, arma::vec &y)
{
    //fill x and y based on rows and colums
    x.set_size(rows*cols);
    y.set_size(rows*cols);

    bool have_abscissa = false;
    mat current_spectrum;
    std::pair<int,int> origin(0,0);
    bool ok = current_spectrum.load(filenames[origin]);
    int spec_rows = current_spectrum.n_rows;
    spectra.set_size(spec_rows, x.n_rows);
    if (!ok){
        return false;
    }
    typedef std::map<std::pair<int,int>, std::string>::iterator map_it;
    uword i = 0;
    std::string filename;
    for (map_it it = filenames.begin(); it!=filenames.end(); ++it){
        std::pair<int,int> keys = it->first;
        x(i) = (double) keys.first;
        y(i) = (double) keys.second;
        filename = it->second;
        ok = current_spectrum.load(filename);
        if (!have_abscissa && current_spectrum.n_cols == 2){
            if (current_spectrum.n_rows > current_spectrum.n_cols){
                abscissa = current_spectrum.col(0);
                current_spectrum.shed_col(0);
            }
            else{
                abscissa = current_spectrum.row(0).t();
                current_spectrum.shed_row(0);
            }
            have_abscissa = true;
        }
        try{
            if (current_spectrum.n_rows > current_spectrum.n_cols){
                if (current_spectrum.n_cols > 1)
                    spectra.col(i) = current_spectrum.col(1);
                else
                    spectra.col(i) = current_spectrum.col(0);
            }
            else{
                if (current_spectrum.n_rows > 1)
                    spectra.col(i) = current_spectrum.row(1).t();
                else
                    spectra.col(i) = current_spectrum.row(0).t();
            }
        }catch(std::exception e){
            spectra.clear();
            abscissa.clear();
            x.clear();
            y.clear();
            std::cerr << "Exception thrown. Spectra " << i << "." << "Type: " << e.what() << "." << std::endl;
            return false;
        }

         if (!ok){
            std::cerr << "Could not load file " << filename << "." << std::endl;
        }
        ++i;
    }

    //check to make sure everything is sorted the way Vespucci expects
    arma::uvec sorted_indices = arma::stable_sort_index(abscissa);
    abscissa = abscissa.rows(sorted_indices);
    spectra = spectra.rows(sorted_indices);

    return have_abscissa;
}

///
/// \brief TextImport::ImportWitec
/// \param filename
/// \param x_start
/// \param y_start
/// \param x_end
/// \param y_end
/// \param x_count
/// \param y_count
/// \param spectra
/// \param abscissa
/// \param x
/// \param y
/// \return
///
bool TextImport::ImportWitec(std::string filename,
                             double x_start,
                             double y_start,
                             double x_end,
                             double y_end,
                             arma::uword x_count,
                             arma::uword y_count,
                             arma::mat &spectra,
                             arma::vec &abscissa,
                             arma::vec &x,
                             arma::vec &y)
{
    Vespucci::ResetDataset(spectra, x, y, abscissa);
    spectra.load(filename);
    if (spectra.n_cols < 2) return false;
    abscissa = spectra.col(0);
    spectra.shed_col(0);
    if (x_count * y_count != spectra.n_cols){
        Vespucci::ResetDataset(spectra, x, y, abscissa);
        return false;
    }
    GenerateSpatialData(x_start, y_start,
                                  x_end, y_end,
                                  x_count, y_count,
                                  x, y);
    if (!(x.n_rows == y.n_rows && x.n_rows == spectra.n_cols)){
        Vespucci::ResetDataset(spectra, x, y, abscissa);
        return false;
    }

    return true;
}

///
/// \brief TextImport::GenerateSpatialData
/// \param x_start
/// \param y_start
/// \param x_end
/// \param y_end
/// \param x_count
/// \param y_count
/// \param x
/// \param y
///
void TextImport::GenerateSpatialData(double x_start, double y_start, double x_end, double y_end, arma::uword x_count, arma::uword y_count, arma::vec &x, arma::vec &y)
{
    x.clear();
    y.clear();
    arma::vec unique_x = arma::linspace(x_start, x_end, x_count);
    arma::vec unique_y = arma::linspace(y_start, y_end, y_count);
    arma::uword vec_size = x_count * y_count;
    x.set_size(vec_size);
    y.set_size(vec_size);
    for (arma::uword i = 0; i < unique_y.n_rows; ++i){
        y.rows(i, i*x_count).fill(unique_y(i));
        x.rows(i, i*x_count) = unique_x;
    }

}

///
/// \brief TextImport::ImportLongText
/// \param filename
/// \param spectra
/// \param x
/// \param y
/// \param swap_spatial
/// \return
///
bool TextImport::ImportLongText(std::string filename, arma::mat &spectra, arma::mat &abscissa, arma::vec &x, arma::vec &y, bool swap_spatial)
{
    arma::mat all_data;
    try{
        all_data.load(filename);
    }catch(...){
        return false;
    }
    if (all_data.n_cols < 4) return false;
    arma::vec all_x = (swap_spatial ? all_data.col(1) : all_data.col(0));
    arma::vec all_y = (swap_spatial ? all_data.col(0) : all_data.col(1));
    arma::vec all_abscissa = all_data.col(2);
    arma::vec all_spectra = all_data.col(3);
    double abscissa_max = all_abscissa.max();
    arma::uvec max_indices = arma::find(all_abscissa == abscissa_max);
    x = all_x.elem(max_indices);
    y = all_y.elem(max_indices);
    abscissa = arma::unique(all_abscissa);
    spectra = arma::mat(all_spectra.memptr(), abscissa.n_rows, x.n_rows);

    arma::uvec sorted_indices = arma::stable_sort_index(abscissa);
    abscissa = abscissa.rows(sorted_indices);
    spectra = spectra.rows(sorted_indices);
    return true;
}
