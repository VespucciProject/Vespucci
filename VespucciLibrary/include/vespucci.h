#ifndef VESPUCCI_H
#define VESPUCCI_H
#if !defined( SWIG )
    // SWIG should not see #inlcude<armadillo> as it can not handle it
    #include <mlpack/core.hpp>
    #include "libvespucci.h"
#endif
#include <iostream>
#include <stdlib.h>
#include <string>
#include <map>

/// \namespace Vespucci
/// \brief A namespace for "global" functions, including math functions
///
///
namespace Vespucci{

    VESPUCCI_EXPORT bool SaveOldVespucciBinary(std::string filename, const arma::mat &spectra, const arma::vec &x, const arma::vec &y, const arma::vec &abscissa);
    VESPUCCI_EXPORT bool SaveText(std::string basename, const arma::mat &spectra, const arma::vec &x, const arma::vec &y, const arma::vec &abscissa, arma::file_type type);
    VESPUCCI_EXPORT bool StitchDatasets(const arma::field<arma::field<arma::mat> > &datasets, arma::mat &spectra, arma::vec &x, arma::vec &y, arma::vec &abscissa);
    VESPUCCI_EXPORT void ResetDataset(arma::mat &spectra, arma::vec &x, arma::vec &y, arma::vec &abscissa);
    VESPUCCI_EXPORT std::string CleanString(const std::string &in);
}


#endif // VESPUCCI_H

