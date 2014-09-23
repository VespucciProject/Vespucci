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
#include "arma_ext.h"
#include <qcustomplot.h>
#include "mapviewer.h"
#include "spectrumviewer.h"
#include "specmap.h"
#include "mainwindow.h"
#include "vespucciworkspace.h"

using namespace std;
using namespace arma;

// This a class for processed map data.  All other map classes are subclasses.
// Includes data (in QCPData formats), descriptive information, and statistics.
class MapViewer;
class SpecMap;
class PrincipalComponentsData;
class SpectrumViewer;
class MainWindow;
class VespucciWorkspace;
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
            QCPColorGradient gradient,
            int source_index,
            int tick_count,
            MainWindow* main_window);

    ~MapData();
    QString name();
    QString type();
    int source_index();

    void set_type(QString type);
    void set_name(QString name, QString type);
    void set_baseline(rowvec abcissa, mat baseline);
    void set_baselines(rowvec first_abcissa, rowvec second_abcissa,
                       mat first_baseline, mat second_baseline);
    void set_fwhm(mat mid_lines);

    bool univariate_area();
    bool band_ratio_area();
    bool univariate_bandwidth();

    QVector<double> first_baseline(int i);
    QVector<double> second_baseline(int i);
    QVector<double> first_abcissa();
    QVector<double> second_abcissa();
    QVector<double> half_max(int i);
    QVector<double> mid_line(int i);
    QVector<double> mid_lines(int i);

    mat stats_; //a statistics matrix like made by MATLAB

    double x_dimension_;
    double y_dimension_;

    //These modify map_data, using QCP function names.
    void setKeySize(int size);
    void setValueSize(int size);
    void setKeyRange(const QCPRange& range);
    void setValueRange(const QCPRange& range);
    //void setData(double key, double value, double z);
    QCPRange dataRange();

    void setGradient(const QCPColorGradient &gradient);
    //Displays the map window
    void ShowMapWindow();
    void CreateImage(QCPColorGradient color_scheme, bool interpolation, int tick_count);
    void SetMapData(QCPColorMapData *map_data);

    void ShowSpectrumViewer(bool enabled);

    void SetXDescription(QString description);
    void SetYDescription(QString description);

    void setSize(int key_size, int value_size);
    bool interpolate();
    void setInterpolate(bool enabled);
    void ShowColorScale(bool enabled);
    void ShowAxes(bool enabled);

    void SetDataRange(QCPRange new_range);
    void RemoveThis();

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

    double results_at_position(double x, double y);

    void UseGlobalColorScale(bool arg1);

    void RescaleMapWidget();
    void LockMapDisplaySize(bool lock);
    void ResetMapWidgetSize();

    uvec extract_range(double lower, double upper);
    colvec results_;
    void LaunchDataExtractor();
    bool crisp_clusters();
    void SetCrispClusters(bool arg1);
private:
    QString x_axis_description_; //equiv to member of SpecMap, passed to SpectraViewer constructor
    QString y_axis_description_; //equiv to member of SpecMap, passed to SpectraViewer constructor

    SpecMap* dataset_;
    MainWindow *main_window_;

    QString name_; //Name, this is displayed in the QListView
    QString type_; //Short description of type.  set by subclass constructor.
    int source_index_; //List index of this

    //QCPColorMapData map_data_;


    //MapViewer *map_display_;  //pointer to the window generated when constructed
    MapViewer *map_display_;
    SpectrumViewer *spectrum_display_; //pointer to the spectrum viewer window (not automatically opened).

    QCustomPlot *map_qcp_; //pointer to the QCustomPlot widget in MapViewer
    QCPColorMap *map_; //pointer to QCPColorMap object within the QCustomPlot widget

    //QCPColorMap map_;
    //QCPColorMapData map_data_;

    QCustomPlot *spectrum_qcp_; //pointer to the QCustomPlot widget in SpectrumViewer

    int key_size_; //size of x (number of unique x values)
    int value_size_; //size of y (number of unique y values)

    QCPColorGradient gradient_;
    QSharedPointer<QCPColorScale> global_color_scale_;
    QCPColorScale *new_color_scale_;

    QString *directory_;

    QCPLayoutElement *color_scale_; //this color scale is not changed!

    //stuff related to baselines and such
    mat first_baseline_;
    mat second_baseline_;
    rowvec first_abcissa_;
    rowvec second_abcissa_;
    mat mid_lines_;

    bool univariate_area_;
    bool band_ratio_area_;
    bool univariate_bandwidth_;
    bool using_global_color_scale_;
    bool k_means_;
    bool principal_components_;
    bool vertex_components_;
    bool partial_least_squares_;
    bool crisp_clusters_;

    QSize initial_map_size_;


};

#endif // MAPDATA_H
