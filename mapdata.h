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

#ifndef MAPDATA_H
#define MAPDATA_H

#include <QString>
#include <armadillo>
#include "qcustomplot.h"
#include "mapviewer.h"
#include "spectrumviewer.h"
#include "specmap.h"

using namespace std;
using namespace arma;

// This a class for processed map data.  All other map classes are subclasses.
// Includes data (in QCPData formats), descriptive information, and statistics.
class MapViewer;
class SpecMap;
class PrincipalComponentsData;

class MapData
{
public:
    MapData(QString x_axis_description,
            QString y_axis_description,
            colvec x,
            colvec y,
            colvec results,
            SpecMap *parent,
            QString *directory,
            QCPColorGradient gradient);

    QString name();
    QString type();
    int source_index();
    int global_index();
    int parent_index();

    void SetSourceIndex(int source_index);
    void SetGlobalIndex(int global_index);

    void set_type(QString type);
    void set_name(QString name, QString type);

    mat stats_; //a statistics matrix like made by MATLAB

    double x_dimension_;
    double y_dimension_;

    //These modify map_data, using QCP function names.
    void setKeySize(int size);
    void setValueSize(int size);
    void setKeyRange(const QCPRange& range);
    void setValueRange(const QCPRange& range);
    //void setData(double key, double value, double z);

    //Displays the map window
    void ShowMapWindow();
    void CreateImage(QCPColorGradient color_scheme, bool interpolation);
    void SetMapData(QCPColorMapData *map_data);

    void ShowSpectrumViewer();

    void SetXDescription(QString description);
    void SetYDescription(QString description);

    void setSize(int key_size, int value_size);
    bool interpolate();
    void setInterpolate(bool enabled);
    void ShowColorScale(bool enabled);
    void ShowAxes(bool enabled);

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

    bool MapDisplayVisible();
    void SetMapDisplayVisible(bool visible);

private:
    QString x_axis_description_; //equiv to member of SpecMap, passed to SpectraViewer constructor
    QString y_axis_description_; //equiv to member of SpecMap, passed to SpectraViewer constructor

    SpecMap* dataset_;

    QString name_; //Name, this is displayed in the QListView
    QString type_; //Short description of type.  set by subclass constructor.
    int source_index_; //List index of this
    int global_index_;

    //QCPColorMapData map_data_;


    MapViewer *map_display_;  //pointer to the window generated when constructed
    SpectrumViewer *spectrum_display_; //pointer to the spectrum viewer window (not automatically opened).

    QCustomPlot *map_qcp_; //pointer to the QCustomPlot widget in MapViewer
    QCPColorMap *map_; //pointer to QCPColorMap object within the QCustomPlot widget

    //QCPColorMap map_;
    //QCPColorMapData map_data_;

    QCustomPlot *spectrum_qcp_; //pointer to the QCustomPlot widget in SpectrumViewer

    int key_size_; //size of x (number of unique x values)
    int value_size_; //size of y (number of unique y values)

    QCPColorGradient gradient_;

    QString *directory_;

    QCPLayoutElement *color_scale_;
};

#endif // MAPDATA_H
