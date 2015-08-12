/************************************************************************************
    Copyright (C) 2015 Wright State University - All Rights Reserved

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
#include "GUI/Display/scalebardialog.h"
#include "GUI/Processing/dataextractordialog.h"

class ScaleBarDialog;
class MapData;
class DataExtractorDialog;
class SpectrumViewer;


namespace Ui {
class MapViewer;
}

///
/// \brief The MapViewer class
/// Displays the image created by MapData
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


    void on_actionLock_Size_toggled(bool arg1);

    void on_actionReset_Size_triggered();

    void on_actionReproportion_triggered();

    void on_actionNew_Dataset_from_Map_triggered();

    void on_actionStats_triggered();

    void on_actionExport_Values_triggered();

    void on_actionSet_Font_triggered();

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

    ///
    /// \brief qcp_
    /// The QCustomPlot object located within this window
    QCustomPlot *qcp_;

    ///
    /// \brief parent_
    /// The MapData object that creates this display
    MapData *parent_;

    ///
    /// \brief color_list_
    /// A list of names of color shemes which
    QStringList color_list_;


};

#endif // MAPVIEWER_H

