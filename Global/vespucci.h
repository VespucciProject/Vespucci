#ifndef VESPUCCI_H
#define VESPUCCI_H
#include <qcustomplot.h>
namespace Vespucci{
    bool SavePlot(QCustomPlot *plot, QString filename);
    void SetQCPFonts(QCustomPlot *plot, const QFont &font);
}

#endif // VESPUCCI_H

