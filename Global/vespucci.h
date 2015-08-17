#ifndef VESPUCCI_H
#define VESPUCCI_H
#include <qcustomplot.h>
#include <armadillo>
#include "Global/libvespucci.h"

/// \namespace Vespucci
/// \brief A namespace for "global" functions, including math functions
///
///
namespace Vespucci{

    VESPUCCI_EXPORT bool SaveHDF5Obj(std::map<std::string, arma::mat*> objects, const std::string filename);
    VESPUCCI_EXPORT bool SaveVespucciBinary(std::string filename, const arma::mat &spectra, const arma::vec &x, const arma::vec &y, const arma::vec &abscissa);
    VESPUCCI_EXPORT bool SaveText(std::string basename, const arma::mat &spectra, const arma::vec &x, const arma::vec &y, const arma::vec &abscissa, arma::file_type type);
}


#endif // VESPUCCI_H

