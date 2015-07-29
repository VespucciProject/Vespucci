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
    bool SaveHDF5Obj(std::map<std::string, arma::mat*> objects, std::string filename);
}


#endif // VESPUCCI_H

