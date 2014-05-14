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

class MapData
{
public:
    MapData(QString x_axis_description,
            QString y_axis_description,
            void *parent); //dataset is the parent

    QString name();
    QString type();
    QString source_index();

    void set_type(QString type);
    void set_name(QString name);

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

    void SetVariables(QString x_axis_description,
                 QString y_axis_description,
                 void *parent);
    void setSize(int key_size, int value_size);
    bool interpolate();
    void setInterpolate(bool enabled);

private:
    QString x_axis_description_; //equiv to member of SpecMap, passed to SpectraViewer constructor
    QString y_axis_description_; //equiv to member of SpecMap, passed to SpectraViewer constructor

    void* dataset_;

    QString name_; //Name, this is displayed in the QListView
    QString type_; //Short description of type.  set by subclass constructor.
    QString source_index_; //List index of this

    //QCPColorMapData map_data_;


    MapViewer *map_display_;  //pointer to the window generated when constructed
    SpectrumViewer *spectrum_display_; //pointer to the spectrum viewer window (not automatically opened).

    QCustomPlot *map_qcp_; //pointer to the QCustomPlot widget in MapViewer
    QCPColorMap *map_; //pointer to QCPColorMap object within the QCustomPlot widget

    QCustomPlot *spectrum_qcp_; //pointer to the QCustomPlot widget in SpectrumViewer

    int key_size_; //size of x (number of unique x values)
    int value_size_; //size of y (number of unique y values)

    QCPColorGradient gradient_;

};

#endif // MAPDATA_H
