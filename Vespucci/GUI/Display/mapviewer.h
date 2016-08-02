/************************************************************************************
    Copyright (C) 2014-2016 Wright State University - All Rights Reserved

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
#include "Data/Imaging/mapdata.h"
#include "GUI/Display/mapplot.h"
#include "GUI/Display/scalebardialog.h"
#include "Global/vespucciworkspace.h"
#include "GUI/Display/mapplot.h"

class ScaleBarDialog;
class MapData;
class DataExtractorDialog;
class SpectrumViewer;
class MainWindow;
class VespucciWorkspace;
class MapPlot;

namespace Ui {
class MapViewer;
class MapPlot;
}

///
/// \brief The MapViewer class
/// Displays the image created by MapData
class MapViewer : public QMainWindow
{
    Q_OBJECT
public:
    MapViewer(MainWindow *parent, QStringList map_keys, QSharedPointer<VespucciWorkspace> ws);
    QCPColorGradient GetGradient(int gradient_number);
    MapPlot *mapPlot();
    ~MapViewer();
signals:
    void RequestSpectrumPlot(QString dataset_key, QString map_name, size_t index);
    void RequestHeldSpectrumPlot(QString dataset_key, QString map_name, size_t index);
public slots:
    void SpectrumRequested(size_t index);
    void SetStatusbar(double x, double y, double z);
private slots:
    void on_actionInterpolate_toggled(bool arg1);

    void on_actionSave_Image_As_triggered();

    void on_actionShow_Axes_toggled(bool arg1);

    void on_actionShow_Color_Scale_toggled(bool arg1);

    void on_actionSet_Color_Scheme_triggered();

    void on_actionAdd_Scale_Bar_triggered();

    void on_actionLock_Size_toggled(bool arg1);

    void on_actionReset_Size_triggered();

    void on_actionReproportion_triggered();

    void on_actionSet_Font_triggered();

    void on_actionSet_Color_Scale_Label_triggered();

    void on_actionSet_Global_Color_Scale_triggered();
protected:
    void keyPressEvent(QKeyEvent *event);
    void showEvent(QShowEvent *event);

private:
    Ui::MapViewer *ui;
    ///
    /// \brief name_
    /// The map name, displayed in the title bar
    QString name_;

    ///
    /// \brief directory_
    /// The global working directory
    QString *directory_;

    QStringList map_keys_;
    QStringList data_keys_;
    QSharedPointer<VespucciWorkspace> workspace_;
    QLabel *statusbar_label_;


};

#endif // MAPVIEWER_H

