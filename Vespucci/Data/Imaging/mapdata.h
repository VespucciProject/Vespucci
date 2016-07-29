/*******************************************************************************
    Copyright (C) 2014-2016 Wright State University - All Rights Reserved
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
#ifndef MAPDATA_H
#define MAPDATA_H

#include <QString>
#include "Math/VespucciMath.h"
#include "GUI/Display/mapplot.h"
#include "GUI/Display/mapviewer.h"
#include "Data/Dataset/vespuccidataset.h"
#include "GUI/mainwindow.h"
#include "Global/vespucciworkspace.h"

using namespace std;
using namespace arma;

class VespucciDataset;
class PlotViewer;
class MainWindow;
class VespucciWorkspace;
class MapPlot;
class MapViewer;

///
/// \brief The MapData class
/// Class for processed map data. Images are created from this data.
class MapData
{
public:
    MapData(QString name,
            QString type,
            QStringList data_keys,
            uword data_column,
            QSharedPointer<VespucciWorkspace> workspace);
    ~MapData();
    QString name();
    QString type();

    void InstantiateMapWindow();
    void SetMapPlot(MapPlot *plot);

    void setGradient(const QCPColorGradient &gradient);
    void SetColorScaleTickCount(int ticks);

    //Displays the map window
    void ShowMapWindow(bool show);
    void HideMapWindow();
    bool MapWindowVisible();

    void CreateImage(QCPColorGradient color_scheme, bool interpolation, int tick_count);
    void SetMapData(QCPColorMapData *map_data);

    void SetXDescription(QString description);
    void SetYDescription(QString description);

    void setSize(int key_size, int value_size);
    bool interpolate();
    void setInterpolate(bool enabled);
    void ShowColorScale(bool enabled);
    void ShowAxes(bool enabled);

    void SetDataRange(QCPRange new_range);

    bool savePdf(const QString &fileName,
                 int width,
                 int height);

    bool savePng(const QString & fileName,
                 int width = 0,
                 int height = 0,
                 double scale = 1.0,
                 int quality = -1);

    bool saveJpg(const QString & fileName,
                 int width=0,
                 int height = 0,
                 double scale = 1.0,
                 int quality = -1);

    bool saveBmp(const QString & fileName,
                 int width = 0,
                 int height = 0,
                 double scale = 1.0);

    bool saveTiff(const QString & fileName,
                  int width = 0,
                  int height = 0,
                  double scale = 1.0,
                  int quality = 0);

    void DrawScaleBar(double width, double height, QString units, QColor color, QString position, QFont font);

    void RescaleMapWidget();
    void LockMapDisplaySize(bool lock);
    void ResetMapWidgetSize();

    void SetFonts(const QFont &font);

    QString global_gradient_key();

    double min();
    double max();

    void SetName(QString name, QString type);
    void SetGlobalGradient(QString name);
    void UpdateGlobalGradient();

    QStringList keys();
    MapPlot *map_qcp();



private:
    ///
    /// \brief x_axis_description_
    /// The x-axis description for the spectra in this map (see VespucciDataset::x_axis_description_)
    QString x_axis_description_; //equiv to member of VespucciDataset, passed to SpectraViewer constructor

    ///
    /// \brief y_axis_description_
    /// The y-axis description for the spectra in this map (see VespucciDataset::y_axis_description_)
    QString y_axis_description_; //equiv to member of VespucciDataset, passed to SpectraViewer constructor

    ///
    /// \brief dataset_
    /// The current dataset
    QSharedPointer<VespucciDataset> dataset_;

    ///
    /// \brief main_window_
    /// The main window
    MainWindow *main_window_;

    ///
    /// \brief name_
    /// Name of the map
    QString name_;

    ///
    /// \brief type_
    /// Short description of the type, set by constructor
    QString type_;

    ///
    /// \brief source_index_
    /// Index of this in the map list of the dataset
    int source_index_;

    ///
    /// \brief map_display_
    /// Pointer the window that displays the image
    MapViewer *map_display_;

    ///
    /// \brief map_qcp_
    /// Pointer to the widget that displays the image
    MapPlot *map_qcp_;

    QSize initial_map_size_;

    QString global_gradient_key_;

    QSharedPointer<VespucciWorkspace> workspace_;

    uword data_column_;
    QStringList data_keys_;


};
Q_DECLARE_METATYPE(QSharedPointer<MapData>)
#endif // MAPDATA_H
