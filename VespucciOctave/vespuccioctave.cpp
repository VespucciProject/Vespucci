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
#include <cstdlib>
#include <sstream>
#include <octave/oct.h>
#include <octave/octave.h>
#include <octave/parse.h>
#include <octave/toplev.h>
#include <octave/symtab.h>
//#include "arrayinterface.h"
#include <mlpack/core.hpp>

bool RunScript();

Matrix ArmaToOctave(const arma::mat &in_mat);
arma::mat OctaveToArma(const Matrix &in_mat);

///
/// \brief main
/// \param argc
/// \param argv
/// \return
/// This program is called from vespucci to handle output to GNU Octave. This
/// kludge is necessary because Octave ships only as 32-bit Windows binaries,
/// and a decision has been made to link dynamically to the user's installation
/// of R and Octave (the dll's are in the path at runtime). This program is to
/// be compiled using a 32-bit toolkit and linking to the .dll.a liboctave and
/// liboctinterp libraries.
///
/// Vespucci, when compiled with a version of Armadillo with 64-bit index support
/// (automatically enabled when using C++11 support, which is not required for
/// Armadillo as used by Vespucci but is required to compile Vespucci due to the
/// use of initializer lists and lambdas inside the Math namespace) can support
/// datasets with more than 2 billion elements (assuming they can be loaded into
/// RAM, as Vespucci is not a "big data" program), this 32-bit program cannot.
/// An exception will be thrown by the vespucci process if a matrix with more than
/// 2,147,483,647 is tried to be sent to this process.
int main(int argc, char *argv[])
{
    void *__gxx_personality_v0;
    if (argc != 6){
        return 1;
    }
    std::string invar_names_path(argv[1]);
    std::string invar_path(argv[2]);
    std::string outvar_names_path(argv[3]);
    std::string outvar_path(argv[4]);

    arma::field<std::string> invar_names;
    invar_names.load(invar_names_path);
    arma::field<arma::mat> invars;
    invars.load(invar_path);
    arma::field<std::string> outvar_names;
    outvar_names.load(outvar_names_path);

    string_vector oct_argv(2);
    oct_argv(0) = "embedded";
    oct_argv(1) = "-q";
    octave_main(2, oct_argv.c_str_vec(), 1);
    octave_value_list oct_invars;
    octave_value_list oct_outvars;

    //add input variables to input variable list
    for (int i = 0; i < invar_names.size(); ++i){
        arma::mat arma_mat = invars(i);
        Matrix oct_mat = ArmaToOctave(arma_mat);
        oct_invars(i) = oct_mat;
    }
    int nargout = outvar_names.size();
    oct_outvars = feval("VespucciOctave", oct_invars, nargout);

    //add output variables to output variable list
    arma::field<arma::mat> outvars(outvar_names.n_elem);
    for (int i = 0; i < outvars.n_rows; ++i){
        Matrix current_oct_mat = oct_outvars(i).matrix_value(true);
        outvars(i) = OctaveToArma(current_oct_mat);
    }
    outvars.save(outvar_path);

    //clean up octave environment
    clean_up_and_exit(0, true);
    return 0;
}

Matrix ArmaToOctave(const arma::mat &in_mat)
{
    unsigned int n_rows = in_mat.n_rows;
    unsigned int n_cols = in_mat.n_cols;
    Matrix out_mat(n_rows, n_cols);
    for (unsigned int i = 0; i < in_mat.n_rows; ++i){
        for (unsigned int j = 0; j < in_mat.n_cols; ++j){
            double val = in_mat(i,j);
            out_mat(i, j) = val;
        }
    }
    return out_mat;
}

arma::mat OctaveToArma(const Matrix &in_mat)
{
    unsigned int n_rows = in_mat.rows();
    unsigned int n_cols = in_mat.columns();
    arma::mat out_mat(n_rows, n_cols);
    for (unsigned int i = 0; i < in_mat.rows(); ++i){
        for (unsigned int j = 0; j < in_mat.columns(); ++j){
            double val = in_mat(i,j);
            out_mat(i, j) = val;
        }
    }
    return out_mat;
}
