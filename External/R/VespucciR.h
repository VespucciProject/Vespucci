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
#ifndef VESPUCCIR_H
#define VESPUCCIR_H

#include <RcppArmadillo.h>
#include <RInside.h>
#include <Rcpp.h>

class VespucciR
{
public:
    VespucciR(int argc, char *argv[]);

    void SetEnvironment(arma::mat &x, arma::mat &y, arma::mat &abscissa, arma::mat &spectra);
    void SetEnvironment(std::map<std::string, arma::mat> &objects);

    void GetEnvironment(arma::mat &x, arma::mat &y, arma::mat &abscissa, arma::mat &spectra);
    std::map<std::string, arma::mat> GetEnvironment(std::map<std::string, std::string> keys);
    void RunScript(std::string cmd);

private:
    RInside R_;

};

#endif // VESPUCCIR_H
