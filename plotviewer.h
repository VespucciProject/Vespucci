/*******************************************************************************
    Copyright (C) 2014 Wright State University - All Rights Reserved
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
#ifndef PLOTVIEWER_H
#define PLOTVIEWER_H

#include <QDialog>

namespace Ui {
class PlotViewer;
}

///
/// \brief The PlotViewer class
/// For displaying plots that are not maps or spectra (PCA/VCA/PLS Scatterplots,
/// especially for meta datasets).
class PlotViewer : public QDialog
{
    Q_OBJECT

public:
    explicit PlotViewer(QWidget *parent = 0);
    ~PlotViewer();

private:
    Ui::PlotViewer *ui;
};

#endif // PLOTVIEWER_H
