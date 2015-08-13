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
#include <Data/Import/textimport.h>
#include <regex>
#include <QtCore>


using namespace arma;
bool TextImport::CheckFileValidity(QString filename, bool &comma_decimals)
{
    QFile inputfile(filename);
    inputfile.open(QIODevice::ReadOnly);
    QTextStream inputstream(&inputfile);
    QString line = inputstream.readLine();
    QStringList tab_list = line.split("\t");
    QStringList comma_list = line.split(",");
    bool valid;

    if(tab_list.size() <= 0 && comma_list.size() <= 0){
        return false;
    }
    else if(tab_list.size() > 0 && comma_list.size() > 0){
        //we probably have a tab delimited file with commas for decimal points
        //import function will probabably throw exception if comma is separator
        //and type is wide text, so comma_decimals must be checked
        comma_decimals = true;
        return true;
    }
    else if(tab_list.size() > 0 && comma_list.size() == 0){
        comma_decimals = false;
        for (int i = 0; i < tab_list.size(); ++i){
            tab_list[i].toDouble(&valid);
            if(!valid)
                return false;
        }
        return true;
    }
    else if(tab_list.size() == 0 && comma_list.size() > 0){
        comma_decimals = false; //we don't know if this is true or not.
        //users using instruments that save csv files with local number formatting
        //in regions where commas are used for decimal places should be super
        //careful.
        for (int i = 0; i < comma_list.size(); ++i){
            comma_list[i].toDouble(&valid);
            if(!valid)
                return false;
        }
        return true;
    }
    else{
        return false;
    }


}

///
/// \brief TextImport::ImportMultiplePoints
/// \param filenames
/// \param rows
/// \param cols
/// \param spectra
/// \param wavelength
/// \param x
/// \param y
/// \return
/// Function for importing a collection of text files with manually verified/set
/// spatial information.
bool TextImport::ImportMultiplePoints(QMap<QPair<int, int>, QString> filenames,
                                      int rows, int cols,
                                      mat &spectra,
                                      vec &wavelength,
                                      vec &x, vec &y)
{
    //fill x and y based on rows and colums
    x.set_size(rows*cols);
    y.set_size(rows*cols);

    filenames.first().toStdString();
    bool have_abscissa = false;
    mat current_spectrum;
    bool ok = current_spectrum.load(filenames.first().toStdString());
    int spec_rows = current_spectrum.n_rows;
    spectra.set_size(spec_rows, x.n_rows);
    if (!ok){
        return false;
    }
    typedef QMap<QPair<int, int>, QString>::iterator map_it;
    uword i = 0;
    QString filename;
    for (map_it it = filenames.begin(); it!=filenames.end(); ++it){
        QPair<int, int> keys = it.key();
        x(i) = (double) keys.first;
        y(i) = (double) keys.second;
        filename = it.value();
        ok = current_spectrum.load(filename.toStdString());
        if (!have_abscissa && current_spectrum.n_cols == 2){
            if (current_spectrum.n_rows > current_spectrum.n_cols){
                wavelength = current_spectrum.col(0);
                current_spectrum.shed_col(0);
            }
            else{
                wavelength = current_spectrum.row(0).t();
                current_spectrum.shed_row(0);
            }
            have_abscissa = true;
        }
        try{
            if (current_spectrum.n_rows > current_spectrum.n_cols){
                spectra.col(i) = current_spectrum.col(0);
            }
            else{
                spectra.col(i) = current_spectrum.row(0).t();
            }
        }catch(std::exception e){
            spectra.clear();
            wavelength.clear();
            x.clear();
            y.clear();
            std::cerr << "Exception thrown. Spectra " << i << "." << "Type: " << e.what() << "." << std::endl;
            return false;
        }

         if (!ok){
            std::cerr << "Could not load file " << filename.toStdString() << "." << std::endl;
        }
        ++i;
    }

    return have_abscissa;

}


bool TextImport::ImportWideText(std::string filename, arma::mat &spectra, arma::vec &wavelength, arma::vec &x, arma::vec &y, bool swap_spatial)
{
    std::ifstream file_stream(filename);
    std::string current_line;
    arma::rowvec current_row;
    std::getline(file_stream, current_line);

    current_line = std::regex_replace(current_line, std::regex("\t|,"), std::string(" "));
    current_row = arma::rowvec(current_line);
    wavelength = current_row.t();
    while (file_stream){
        std::getline(file_stream, current_line);
        //replace every separatory character with space
        current_line = std::regex_replace(current_line, std::regex("\t|,"), std::string(" "));
        current_row = arma::rowvec(current_line);
        if (swap_spatial){
            x.insert_rows(x.n_rows, 1);
            x(x.n_rows-1) = current_row(1);
            y.insert_rows(y.n_rows, 1);
            y(y.n_rows-1) = current_row(0);
        }
        else{
            y.insert_rows(y.n_rows, 1);
            y(y.n_rows-1) = current_row(1);
            x.insert_rows(x.n_rows, 1);
            x(x.n_rows-1) = current_row(0);
        }

        current_row.shed_cols(0,1);
        spectra.insert_cols(spectra.n_cols, 1);
        spectra.col(spectra.n_cols-1) = current_row.t();
    }
    return true;

}
