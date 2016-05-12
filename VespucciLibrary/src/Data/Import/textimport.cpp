/*******************************************************************************
    Copyright (C) 2014-2016 Wright State University - All Rights Reserved
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
#include <Data/Import/textimport.h>
#include <Global/vespucci.h>
#include <regex>
#include <QtCore>
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>


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
    Vespucci::ResetDataset(spectra, x, y, abscissa);
    std::vector<double> x_s;
    std::vector<double> y_s;
    std::vector<double> abs_s;
    std::vector<double> spec_s;
    std::ifstream file_stream(filename);
    std::string current_line;
    std::getline(file_stream, current_line);
    typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
    tokenizer::iterator it;
    boost::char_separator<char> sep(",\t");
    tokenizer tok(current_line, sep);
    for (it = tok.begin(); it != tok.end(); ++it){
        try{
            abs_s.push_back(boost::lexical_cast<double>(*it));
        }catch(boost::bad_lexical_cast & b){
            std::cout << b.what() << std::endl;
        }
    }
    while (file_stream){
        std::getline(file_stream, current_line);
        if (current_line.empty()){break;}
        tok = tokenizer(current_line, sep);
        it = tok.begin();
        if (swap_spatial){
            try{
                y_s.push_back(boost::lexical_cast<double>(*it));
            }catch (boost::bad_lexical_cast & b){
                std::cout << b.what();
            }

            ++it;
            try{
                x_s.push_back(boost::lexical_cast<double>(*it));
            }catch (boost::bad_lexical_cast & b){
                std::cout << b.what();
            }

            ++it;
        }
        else{
            try{
                x_s.push_back(boost::lexical_cast<double>(*it));
            }catch (boost::bad_lexical_cast & b){
                std::cout << b.what();
            }

            ++it;
            try{
                y_s.push_back(boost::lexical_cast<double>(*it));
            }catch (boost::bad_lexical_cast & b){
                std::cout << b.what();
            }

            ++it;
        }
        while (it != tok.end()){
            try{
               spec_s.push_back(boost::lexical_cast<double>(*it));
            }catch (boost::bad_lexical_cast & b){
                std::cout << b.what();
            }

            ++it;
        }
    }
    abscissa = arma::vec(abs_s);
    x = arma::vec(x_s);
    y = arma::vec(y_s);
    spectra = arma::mat(spec_s);
    spectra.reshape(abscissa.n_rows, x.n_rows);

    //check to make sure everything is sorted the way Vespucci expects
    arma::uvec sorted_indices = arma::stable_sort_index(abscissa);
    abscissa = abscissa.rows(sorted_indices);
    spectra = spectra.rows(sorted_indices);

    return true;
}


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
                spectra.col(i) = current_spectrum.col(0);
            }
            else{
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
/// \param spec_filename
/// \param x_filename
/// \param y_filename
/// \param spectra
/// \param abscissa
/// \param x
/// \param y
/// \return
/// Parses a Witec dataset. These consist of the abscissa and spectra in a column-major format
/// With the x and y values in a separate file.
/// For each value of x, a repeating list of y values is given.
/// This function can also automatically transpose datasets if given y for x.
bool TextImport::ImportWitec(std::string spec_filename, std::string x_filename, std::string y_filename, arma::mat &spectra, arma::vec &abscissa, arma::vec &x, arma::vec &y)
{
    bool spec_ok = spectra.load(spec_filename);
    if (!spec_ok){
        spectra.clear();
        abscissa.clear();
        x.clear();
        y.clear();
        return false;
    }

    abscissa = spectra.col(0);
    spectra.shed_col(0);
    bool x_ok = x.load(x_filename);
    bool y_ok = y.load(y_filename);
    if (!(x_ok && y_ok)){
        spectra.clear();
        abscissa.clear();
        x.clear();
        y.clear();
        return false;
    }

    //the repeating, unique values in y are what we keep with constant x
    //if y has the unique values, then we keep constant x
    arma::vec unique_x = arma::unique(x);
    arma::vec unique_y = arma::unique(y);
    bool x_unique = unique_x.n_rows == x.n_rows;

    //if data doesn't look like we expect it to, give up
    if (!x_unique && (unique_y.n_rows == y.n_rows)){
        spectra.clear();
        abscissa.clear();
        x.clear();
        y.clear();
        return false;
    }


    if (x_unique){
        //we're going to fill x with new values (y already follows Vespucci conventions)
        x.clear();
        for (arma::uword i = 0; i < unique_x.n_rows; ++i){
            arma::vec x_values(unique_y.n_rows);
            x_values.fill(unique_x(i));
            x = arma::join_vert(x, x_values);
        }
    }
    else{
        //we're going to fill y with new values (x already follows Vespucci conventions)
        y.clear();
        for (arma::uword i = 0; i < unique_y.n_rows; ++i){
            arma::vec y_values(unique_x.n_rows);
            y_values.fill(unique_y(i));
            y = arma::join_vert(y, y_values);
        }
    }

    return true;
}
