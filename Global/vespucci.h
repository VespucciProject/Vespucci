#ifndef VESPUCCI_H
#define VESPUCCI_H
#include <qcustomplot.h>
#include <armadillo>

/// \namespace Vespucci
/// \brief A namespace for "global" functions, including math functions
///
///
namespace Vespucci{
    bool SavePlot(QCustomPlot *plot, QString filename);
    void SetQCPFonts(QCustomPlot *plot, const QFont &font);
    bool SaveHDF5Obj(std::map<std::string, arma::mat*> objects, const std::string filename);
    bool SaveZipped(std::map<std::string, const arma::mat *> objects, const std::string filename, const arma::file_type type);
    bool SaveZipped(const arma::mat &spectra, const arma::vec &abscissa, const arma::vec &x, const arma::vec &y, const std::string filename, const arma::file_type type);
    bool SaveVespucciBinary(std::string filename, const arma::mat &spectra, const arma::vec &x, const arma::vec &y, const arma::vec &abscissa);
    int CreateZipFile(std::string zip_filename, std::vector<std::string> paths);
}


#endif // VESPUCCI_H

