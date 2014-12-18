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
#ifndef MAPDATA_H
#define MAPDATA_H

#include <QString>
#include "arma_ext.h"
#include <qcustomplot.h>
#include "mapviewer.h"
#include "spectrumviewer.h"
#include "vespuccidataset.h"
#include "mainwindow.h"
#include "vespucciworkspace.h"
#include "statsdialog.h"


using namespace std;
using namespace arma;


class MapViewer;
class VespucciDataset;
class PrincipalComponentsData;
class SpectrumViewer;
class StatsViewer;
class MainWindow;
class VespucciWorkspace;

///
/// \brief The MapData class
/// Class for processed map data. Images are created from this data.
class MapData
{
public:
    MapData(QString x_axis_description,
            QString y_axis_description,
            colvec x,
            colvec y,
            colvec results,
            QSharedPointer<VespucciDataset> parent,
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
    void set_baseline(rowvec abscissa, mat baseline);
    void set_baselines(rowvec first_abscissa, rowvec second_abscissa,
                       mat first_baseline, mat second_baseline);
    void set_fwhm(mat mid_lines);

    bool univariate_area();
    bool band_ratio_area();
    bool univariate_bandwidth();

    QVector<double> first_baseline(int i);
    QVector<double> second_baseline(int i);
    QVector<double> first_abscissa();
    QVector<double> second_abscissa();
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
    void HideMapWindow();
    bool MapWindowVisible();
    bool SpectrumViewerVisible();
    void HideSpectrumViewer();

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

    void ExportText();
    void ShowStatsDialog();

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
    /// \brief spectrum_display_
    /// Pointer to the window that displays spectra when the image is clicked
    SpectrumViewer *spectrum_display_; //pointer to the spectrum viewer window (not automatically opened).

    ///
    /// \brief map_qcp_
    /// Pointer to the widget that displays the image
    QCustomPlot *map_qcp_;

    ///
    /// \brief map_
    /// Pointer to the QCPColorMap object within the QCustomPlot widget
    QCPColorMap *map_;

    ///
    /// \brief spectrum_qcp_
    /// Pointer to the QCustomPlot widget in the SpectrumViewer
    QCustomPlot *spectrum_qcp_;

    ///
    /// \brief key_size_
    /// Number of unique x values
    int key_size_;

    ///
    /// \brief value_size_
    /// Number of unique y values
    int value_size_; //size of y (number of unique y values)

    ///
    /// \brief gradient_
    /// Current color gradient
    QCPColorGradient gradient_;

    ///
    /// \brief global_color_scale_
    /// The global color scale
    QSharedPointer<QCPColorScale> global_color_scale_;

    ///
    /// \brief new_color_scale_
    /// Pointer to the global color scale (kept in the main window)
    QCPColorScale *new_color_scale_;

    ///
    /// \brief directory_
    /// The global working directory
    QString *directory_;

    ///
    /// \brief color_scale_
    /// The color scale displayed on the map viewer
    QCPLayoutElement *color_scale_; //this color scale is not changed!

    //stuff related to baselines and such
    mat first_baseline_;
    mat second_baseline_;
    rowvec first_abscissa_;
    rowvec second_abscissa_;
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
Q_DECLARE_METATYPE(QSharedPointer<MapData>)
#endif // MAPDATA_H
