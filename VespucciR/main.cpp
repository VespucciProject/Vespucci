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
#include <iostream>
#include <armadillo>
#include <Rcpp.h>
#include <RInside.h>
#include <RcppArmadillo.h>

using namespace std;

int main(int argc, char *argv[])
{
    if (argc != 6){
        return 1;
    }
    std::string invar_names_path(argv[1]);
    std::string invar_path(argv[2]);
    std::string outvar_names_path(argv[3]);
    std::string outvar_path(argv[4]);
    std::string cmd_path(argv[5]);

    arma::field<std::string> invar_names;
    invar_names.load(invar_names_path);
    arma::field<arma::mat> invars;
    invars.load(invar_path);
    arma::field<std::string> outvar_names;
    outvar_names.load(outvar_names_path);
    arma::field<arma::mat> outvars(outvar_names.n_elem);


    int R_argc = 1;
    char first_arg[] = "vespucci"; //done to avoid string conversion
    char* R_argv[1];
    R_argv[0] = first_arg;
    RInside R_instance(R_argc, R_argv);
    for (arma::uword i = 0; i < invar_names.n_elem; ++i)
        R_instance[invar_names(i)] = invars(i).t(); //to account for R's row-major nature

    std::istream commands(cmd_path);
    std::string command;
    try{
        while (!commands.eof()){
            std::getline(commands, command);
            std::cout << "R: " << command << std::endl;
            try{
                R_instance.parseEval(command);
            }catch(std::exception e){
                std::cerr << e.what();
                std::string what(e.what());
                what = "RInside Exception: " + what;
                return 1;
            }
        }
    }
    catch (std::exception e)
    {
        std::cerr << e.what();
        std::string what(e.what());
        what = command;
        return 1;
    }

    //Get requested variables
    arma::mat matrix;
    std::string key;
    for (arma::uword i = 0; i < outvar_names.n_elem; ++i){
        key = outvar_names(i);
        std::cout << "Get " << key << std::endl;
        try{
            //breaking into parts because everything is really templated
            key = "as.numeric(" + key + ")";
            std::string query = "as.numeric(" + key + ")";
            std::string existtest = "exists(\"" + key + "\")";
            std::string mattest = "is.matrix(" + query + ")";
            std::string vectest = "is.vector(" + query + ")";
            bool exists = Rcpp::as<bool>(R_instance.parseEval(existtest));
            bool ismat = Rcpp::as<bool>(R_instance.parseEval(mattest));
            bool isvec = Rcpp::as<bool>(R_instance.parseEval(vectest));

            //vectors must be cast as arma::vec, matrices as arma::mat
            //check if R object is matrix or vector
            if (exists && ismat)
                matrix = Rcpp::as<arma::mat>(R_instance.parseEval(query));
            else if (exists && isvec)
                matrix = (arma::mat) Rcpp::as<arma::vec>(R_instance.parseEval(query));
            else
                std::cerr << R_key << " is of unsupported type or does not exist!" << std::endl;

        }catch(std::exception e){
            std::cerr << "Conversion to matrix failed" << std::endl;
        }
        try{
            outvars(i) = matrix;
        }catch(std::exception e){
            std::cerr << "Adding " << R_key << " failed" << std::endl;
            //ignore and move on. element remains uninitialized in outvars
        }
    }
    outvars.save(outvar_path);
    return 0;
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
        std::cout << "Get " << R_key << " as " << vespucci_key << std::endl;
        try{
            //breaking into parts because everything is really templated
            R_key = "as.numeric(" + R_key + ")";
            std::string query = "as.numeric(" + R_key + ")";
            std::string existtest = "exists(\"" + R_key + "\")";
            std::string mattest = "is.matrix(" + query + ")";
            std::string vectest = "is.vector(" + query + ")";
            bool exists = Rcpp::as<bool>(R_.parseEval(existtest));
            bool ismat = Rcpp::as<bool>(R_.parseEval(mattest));
            bool isvec = Rcpp::as<bool>(R_.parseEval(vectest));

            //vectors must be cast as arma::vec, matrices as arma::mat
            //check if R object is matrix or vector
            if (exists && ismat)
                matrix = Rcpp::as<arma::mat>(R_.parseEval(query));
            else if (exists && isvec)
                matrix = (arma::mat) Rcpp::as<arma::vec>(R_.parseEval(query));
            else
                std::cerr << R_key << " is of unsupported type or does not exist!" << std::endl;

        }catch(std::exception e){
            std::cerr << "Conversion to matrix failed" << std::endl;
        }
        try{
            objects[vespucci_key] = matrix;
        }catch(std::exception e){
            std::cerr << "Adding " << R_key << " failed" << std::endl;
            //ignore and move on.
        }
    }
    return objects;
}

void VespucciR::RunScript(std::string cmd)
{
    //read commands line by line for better error output
    std::istringstream commands(cmd);
    std::string command;
    try{
        while (!commands.eof()){
            std::getline(commands, command);
            std::cout << "R: " << command << std::endl;
            try{
                R_.parseEval(command);
            }catch(std::exception e){
                std::cerr << e.what();
                std::string what(e.what());
                what = "RInside Exception: " + what;
                throw std::runtime_error(what.c_str());
            }
        }
    }
    catch (std::exception e)
    {
        std::cerr << e.what();
        std::string what(e.what());
        what = command;
        throw std::runtime_error(what.c_str());
    }
}


