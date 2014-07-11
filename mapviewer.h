/************************************************************************************
    Copyright (C) 2014 Daniel P. Foose - All Rights Reserved

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
***************************************************************************************/

#ifndef MAPVIEWER_H
#define MAPVIEWER_H

#include <QMainWindow>
#include "mapdata.h"
#include "scalebardialog.h"

class ScaleBarDialog;
class MapData;
class SpectrumViewer;


namespace Ui {
class MapViewer;
}

class MapViewer : public QMainWindow
{
    Q_OBJECT
public:
    QCPColorGradient GetGradient(int gradient_number);

protected:
    void closeEvent(QCloseEvent *event);

public:
    MapViewer(QString name, QString *directory, MapData *parent);
    ~MapViewer();
public slots:
    void GlobalDataRangeChanged(QCPRange new_range);
    void GlobalGradientChanged(QCPColorGradient new_gradient);

private slots:
    void on_actionInterpolate_triggered();

    void on_actionInterpolate_toggled(bool arg1);

    void on_actionSave_Image_As_triggered();

    void on_actionShow_Axes_toggled(bool arg1);

    void on_actionShow_Color_Scale_toggled(bool arg1);

    void on_actionSet_Color_Scheme_triggered();

    void on_actionAdd_Scale_Bar_triggered();

    void on_actionShow_Spectrum_Viewer_triggered();

    void on_actionCommon_Color_Gradient_toggled(bool arg1);

private:
    Ui::MapViewer *ui;
    QString name_;
    QString *directory_;

    QCustomPlot *qcp_;

    //QCPColorMap *color_map_;
    MapData *parent_;

    QStringList color_list_;


};

#endif // MAPVIEWER_H
