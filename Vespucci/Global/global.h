/*******************************************************************************
    Copyright (C) 2014-2017 Wright State University - All Rights Reserved
    Daniel P. Foose - Maintainer/Lead Developer

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
#ifndef GLOBAL
#define GLOBAL
#include "qcustomplot.h"
#include <mlpack/core.hpp>
namespace Vespucci{
    bool SavePlot(QCustomPlot *plot, QString filename);
    void SetQCPFonts(QCustomPlot *plot, const QFont &font);
    QProgressDialog *DisplayProgressDialog(QWidget *parent, QString title, QString text);
    bool KeysAreEqual(QStringList &keys1, QStringList &keys2);
    struct GlobalGradient{
        QCPColorGradient gradient;
        QCPRange range;
    };
    QVector<double> FromArmaVec(const arma::vec &data);
}
#endif // GLOBAL

