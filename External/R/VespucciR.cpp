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
#include <External/R/VespucciR.h>

VespucciR::VespucciR(int argc, char *argv[]) : R_(argc, argv)
{

}

///
/// \brief VespucciR::SetEnvironment Sets the initial set of variables defined in the R environment
/// \param x 1-column matrix of horizontal spatial position
/// \param y 1-colum matrix of vertical spatial position
/// \param abscissa 1-column matrix containing spectral abscissa
/// \param spectra Column having the same number of rows as the abscissa and the
/// same number of columns as x and y have rows. Contains spectral data.
///
/// The nth row of x and y correspond to the nth column of spectra.
/// The nth row of abscissa corresponds to the nth row of spectra. Spectra will
/// be transposed within the R environment in keeping with R's convention of
/// samples in rows and observations in columns.
///
void VespucciR::SetEnvironment(arma::mat &x, arma::mat &y, arma::mat &abscissa, arma::mat &spectra)
{
    R_["x"] = x;
    R_["y"] = y;
    R_["spectra"] = spectra;
    R_["abscissa"] = abscissa;
}

void VespucciR::SetEnvironment(std::map<std::string, arma::mat> &objects)
{
    for (std::map<std::string, arma::mat>::iterator i = objects.begin(); i != objects.end(); ++i){
        if (i->first == "spectra" || i->first == "abscissa"){
            R_[i->first] = arma::trans(i->second);
        }
        else{
            R_[i->first] = i->second;
        }
    }
}


///
/// \brief VespucciR::GetEnvironment Returns potentially modified variables from
/// the R environment.
/// \param x
/// \param y
/// \param abscissa
/// \param spectra
///
void VespucciR::GetEnvironment(arma::mat &x, arma::mat &y, arma::mat &abscissa, arma::mat &spectra)
{
    x = Rcpp::as<arma::mat>(R_.parseEval("x"));
    y = Rcpp::as<arma::mat>(R_.parseEval("y"));
    //R prefers row major layout, so we have to convert back.
    abscissa = Rcpp::as<arma::mat>(R_.parseEval("t(abscissa)"));
    spectra = Rcpp::as<arma::mat>(R_.parseEval("t(spectra)"));
}

std::map<std::string, arma::mat> VespucciR::GetEnvironment(std::map<std::string, std::string> keys)
{
    std::map<std::string, arma::mat> objects;
    arma::mat matrix;
    std::string vespucci_key;
    std::string R_key;
    for (std::map<std::string, std::string>::iterator i = keys.begin(); i != keys.end(); ++i){
        vespucci_key = i->first;
        R_key = i->second;
        matrix = Rcpp::as<arma::mat>(R_.parseEval(R_key));
        objects[vespucci_key] = matrix;
    }
    return objects;
}

void VespucciR::RunScript(std::string cmd)
{
    R_.parseEval(cmd);
}


