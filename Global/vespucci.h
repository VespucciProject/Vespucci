#ifndef VESPUCCI_H
#define VESPUCCI_H
#include <qcustomplot.h>
#include <armadillo>
#if defined(VESPUCCI_EXPORT)
#  define VESPUCCI_EXPORT Q_DECL_EXPORT
#else
#  define VESPUCCI_EXPORT Q_DECL_IMPORT
#endif
/// \namespace Vespucci
/// \brief A namespace for "global" functions, including math functions
///
///
namespace Vespucci{
    bool SavePlot(QCustomPlot *plot, QString filename);
    void SetQCPFonts(QCustomPlot *plot, const QFont &font);
    bool SaveHDF5Obj(std::map<std::string, arma::mat*> objects, const std::string filename);
    bool SaveVespucciBinary(std::string filename, const arma::mat &spectra, const arma::vec &x, const arma::vec &y, const arma::vec &abscissa);
    bool SaveText(std::string basename, const arma::mat &spectra, const arma::vec &x, const arma::vec &y, const arma::vec &abscissa, arma::file_type type);
}


#endif // VESPUCCI_H

