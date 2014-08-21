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

#include "mapdata.h"

///
/// \brief MapData::MapData
/// \param x_axis_description Description of the spectral abcissa
/// \param y_axis_description Description of the spectral ordinate
/// \param x Vector of horizontal coordinates map points
/// \param y Vector of vertical coordinates for map points
/// \param results Vector of results at x and y
/// \param parent The SpecMap object storing the map data.
/// \param directory The current working directory on the filesystem
/// \param gradient The initially-chosen color gradient.
/// \param source_index The index of this object in parent.
/// \param main_window The main window of the app.
///
MapData::MapData(QString x_axis_description,
                 QString y_axis_description,
                 colvec x,
                 colvec y,
                 colvec results,
                 SpecMap *parent,
                 QString *directory,
                 QCPColorGradient gradient,
                 int source_index,
                 MainWindow *main_window)
{
    name_ = parent->name();
    directory_ = directory;
    main_window_ = main_window;
    univariate_area_ = false;
    band_ratio_area_ = false;
    univariate_bandwidth_ = false;
    using_global_color_scale_ = false;
    map_display_ = new MapViewer(name_, directory_, this);
    map_qcp_ = map_display_->findChild<QCustomPlot *>("mapView");
    map_ = new QCPColorMap(map_qcp_->xAxis, map_qcp_->yAxis);
    map_->setGradient(gradient);
    map_qcp_->addPlottable(map_);
    map_->data()->setKeyRange(parent->KeyRange());
    map_->data()->setValueRange(parent->ValueRange());
    map_->data()->setKeySize(parent->KeySize());
    map_->data()->setValueSize(parent->ValueSize());

    for(unsigned int i = 0; i<x.n_elem; ++i){
        map_->data()->setData(x(i), y(i), results(i));
    }
    map_->rescaleDataRange();
    map_qcp_->rescaleAxes();
    this->CreateImage(gradient, false);
    x_axis_description_ = x_axis_description;
    y_axis_description_ = y_axis_description;
    dataset_ = parent;
    source_index_ = source_index;
    spectrum_display_ = new SpectrumViewer(map_display_,
                                           this,
                                           x_axis_description,
                                           y_axis_description,
                                           dataset_,
                                           map_qcp_->size(),
                                           *directory);
    QObject::connect(map_qcp_,
                     SIGNAL(plottableClick(QCPAbstractPlottable*,QMouseEvent*)),
                     spectrum_display_,
                     SLOT(MapClicked(QCPAbstractPlottable*,QMouseEvent*)));

    QObject::connect(main_window_,
                     SIGNAL(GlobalDataRangeChanged(QCPRange)),
                     map_display_,
                     SLOT(GlobalDataRangeChanged(QCPRange)));

    QObject::connect(main_window_,
                     SIGNAL(GlobalGradientChanged(QCPColorGradient)),
                     map_display_,
                     SLOT(GlobalGradientChanged(QCPColorGradient)));

    spectrum_qcp_ = spectrum_display_->findChild<QCustomPlot *>("spectrum");
    //create the objects the pointers point to
    initial_map_size_ = map_qcp_->size();

}


///
/// \brief MapData::~MapData
///Deletes everything the new keyword is used on in this object.
/// Destructor triggered when this is removed from SpecMap list.
MapData::~MapData()
{

    delete spectrum_display_;

    delete new_color_scale_;

    delete map_;

}

///
/// \brief MapData::name
/// \return The name of the map.
///
QString MapData::name()
{
    return name_;
}

///
/// \brief MapData::type
/// \return A string describing the type of map.
///
QString MapData::type()
{
    return type_;
}

///
/// \brief MapData::source_index
/// \return
/// Sets the index of the map in the list of maps in the SpecMap object.
int MapData::source_index()
{
    return source_index_;
}

///
/// \brief MapData::set_type
/// \param type
/// Sets the type description of the map.
void MapData::set_type(QString type)
{
    type_ = type;
}

///
/// \brief MapData::set_name
/// \param name
/// \param type
/// Sets the title of the window, default name for saved files and display name in main window.
void MapData::set_name(QString name, QString type)
{
    name_ = name;
    type_ = type;
    QString window_title = dataset_->name() + "—" + name + " " + type;
    map_display_->setWindowTitle(window_title);
}

///
/// \brief MapData::ShowMapWindow
/// Shows or hides the map window.
void MapData::ShowMapWindow()
{
    if(!map_display_->isVisible()){
        map_display_->show();
    }
    if(!map_display_->isActiveWindow()){
        map_display_->activateWindow();
    }
}

///
/// \brief MapData::SetMapData
/// \param map_data
/// Sets the data of the map. See QCustomPlot::QCPColorMap::setData().
void MapData::SetMapData(QCPColorMapData *map_data)
{
    map_->setData(map_data);
}

///
/// \brief MapData::CreateImage
/// \param color_scheme
/// \param interpolation
/// Creates the image by sending data to the QCustomPlot widgets.
void MapData::CreateImage(QCPColorGradient color_scheme, bool interpolation)
{

    map_->setGradient(color_scheme);
    map_->rescaleDataRange(true);
    map_qcp_->rescaleAxes(true);

    new_color_scale_ = new QCPColorScale(map_qcp_);
    new_color_scale_->setGradient(color_scheme);
    new_color_scale_->setDataRange(map_->dataRange());
    map_qcp_->plotLayout()->addElement(0, 1, new_color_scale_);

    color_scale_ = map_qcp_->plotLayout()->element(0, 1);

    map_->setInterpolate(interpolation);
    int key_size = map_->data()->keySize();
    int value_size = map_->data()->valueSize();
    key_size *= 9;
    value_size *= 9;
    map_qcp_->resize(key_size, value_size);
    key_size +=50;
    value_size +=50;
    map_display_->resize(key_size, value_size);
}

///
/// \brief MapData::SetXDescription
/// \param description
/// Sets description for the X axis in the spectrum viewer.
void MapData::SetXDescription(QString description)
{
    x_axis_description_ = description;
}

///
/// \brief MapData::SetYDescription
/// \param description
/// Sets description for the Y axis in the spectrum viewer.
void MapData::SetYDescription(QString description)
{
    y_axis_description_ = description;
}

///
/// \brief MapData::interpolate
/// \return Whether or not map is interpolated
/// See QCustomPlot::QCPColorMap::interpolate().
bool MapData::interpolate()
{
    if (!map_qcp_->hasPlottable(map_)){
        map_qcp_->addPlottable(map_);
    }
    return map_->interpolate();
}

///
/// \brief MapData::setInterpolate
/// \param enabled Whether or not image should be interpolated.
/// Toggles image interpolation on or off. See QCustomPlot::QCPColorMap::setInterpolate.
void MapData::setInterpolate(bool enabled)
{
    if (!map_qcp_->hasPlottable(map_)){
        map_qcp_->addPlottable(map_);
    }
    map_->setInterpolate(enabled);
    map_qcp_->replot();
}

///
/// \brief MapData::ShowColorScale
/// \param enabled Whether or not color scale should be visible.
/// Toggles the color scale bar on or off.
void MapData::ShowColorScale(bool enabled)
{
    if (enabled){
        if (!map_qcp_->plotLayout()->element(0,1)->visible()){
            map_qcp_->plotLayout()->element(0, 1)->setVisible(true);
            map_qcp_->replot();
            map_qcp_->repaint();
        }
    }

    else{
        if (map_qcp_->plotLayout()->element(0, 1)->visible()){
            map_qcp_->plotLayout()->element(0, 1)->setVisible(false);
            map_qcp_->replot();
            map_qcp_->repaint();
        }
    }
}

///
/// \brief MapData::ShowAxes
/// \param enabled Whether or not axes should be visible on map.
/// Toggles the axes on and off.
void MapData::ShowAxes(bool enabled)
{
    if (enabled){
        if (!map_qcp_->xAxis->visible()){
            map_qcp_->xAxis->setVisible(true);
            map_qcp_->yAxis->setVisible(true);
            map_qcp_->replot();
            map_qcp_->repaint();
        }
    }

    else{
        if (map_qcp_->xAxis->visible()){
            map_qcp_->xAxis->setVisible(false);
            map_qcp_->yAxis->setVisible(false);
            map_qcp_->replot();
            map_qcp_->repaint();
        }
    }
}

///
/// \brief MapData::saveBmp
/// \param fileName
/// \param width
/// \param height
/// \param scale
/// \return
/// Saves the map as a bitmap. See QCustomPlot::saveBmp.
bool MapData::saveBmp(const QString &fileName, int width, int height, double scale)
{
    map_qcp_->setBackground(Qt::white);
    map_qcp_->replot();
    bool success = map_qcp_->saveBmp(fileName, width, height, scale);

    map_qcp_->setBackground(map_display_->palette().window());
    map_qcp_->replot();
    return success;
}

///
/// \brief MapData::savePng
/// \param fileName
/// \param width
/// \param height
/// \param scale
/// \param quality
/// \return
/// Saves map as a png. See QCustomPlot::savePng.
bool MapData::savePng(const QString &fileName, int width, int height, double scale, int quality)
{
    map_qcp_->setBackground(Qt::transparent);
    map_qcp_->replot();
    bool success = map_qcp_->savePng(fileName, width, height, scale, quality);
    map_qcp_->setBackground(map_display_->palette().window());
    map_qcp_->replot();
    return success;
}

///
/// \brief MapData::saveJpg
/// \param fileName
/// \param width
/// \param height
/// \param scale
/// \param quality
/// \return
/// Saves the map as a JPEG. See QCustomPlot::saveJpg
bool MapData::saveJpg(const QString &fileName, int width, int height, double scale, int quality)
{
    map_qcp_->setBackground(Qt::white);
    map_qcp_->replot();
    bool success = map_qcp_->saveJpg(fileName, width, height, scale, quality);
    map_qcp_->setBackground(map_display_->palette().window());
    map_qcp_->replot();
    return success;
}

///
/// \brief MapData::saveTiff
/// \param fileName
/// \param width
/// \param height
/// \param scale
/// \param quality
/// \return
/// Saves the map as a tiff. See QCustomPlot::saveRastered.
bool MapData::saveTiff(const QString &fileName, int width, int height, double scale, int quality)
{
    map_qcp_->setBackground(Qt::transparent);
    map_qcp_->replot();
    bool success = map_qcp_->saveRastered(fileName, width, height, scale, "TIF", quality);
    map_qcp_->setBackground(map_display_->palette().window());
    map_qcp_->replot();
    return success;
}

///
/// \brief MapData::RemoveThis
///Triggers the SpecMap object to remove this from the list.  Since SpecMap
/// stores MapData objects as shared pointers, and only one object (the map list)
/// contains this pointer, this removal results in this object being deleted.
void MapData::RemoveThis()
{
    dataset_->RemoveMapAt(source_index_);
}

///
/// \brief MapData::setGradient
/// \param gradient
/// Sets the color gradient of the map.
void MapData::setGradient(const QCPColorGradient &gradient)
{
    map_->setGradient(gradient);
    QCPColorScale *color_scale = qobject_cast<QCPColorScale *>(map_qcp_->plotLayout()->element(0, 1));
    color_scale->setGradient(gradient);
    map_qcp_->replot();
}

///
/// \brief MapData::DrawScaleBar
/// \param width The width of the bar in map units.
/// \param height The height of the bar.
/// \param units The number to display on the bar label.
/// \param color The color of the bar.
/// \param position The position of the bar (Top or Bottom, Left or Right)
/// \param font The desired font for the bar label.
/// Draws a scale bar object on the map.
void MapData::DrawScaleBar(double width,
                           double height,
                           QString units,
                           QColor color,
                           QString position,
                           QFont font)
{
    //instantiate shape

    //QCustomPlot object will take ownership of these and make sure they're deleted.
    QCPItemRect *scale_bar = new QCPItemRect(map_qcp_);
    QCPItemText *scale_bar_text = new QCPItemText(map_qcp_);


    map_qcp_->addItem(scale_bar);
    map_qcp_->addItem(scale_bar_text);

    QString text = QString::number(width) + " " + units;
    scale_bar_text->setText(text);
    scale_bar_text->setPositionAlignment(Qt::AlignCenter);
    scale_bar_text->setColor(color);
    scale_bar_text->setFont(font);

    QPen pen(color);
    QBrush brush(color, Qt::SolidPattern);
    scale_bar->setPen(pen);
    scale_bar->setBrush(brush);
    QCPRange key_range = map_->keyAxis()->range();
    QCPRange value_range = map_->valueAxis()->range();

    //find corners of image
    double key_max = key_range.upper;
    double key_min = key_range.lower;
    double value_max = value_range.upper;
    double value_min = value_range.lower;

    double x_min, y_min, x_max, y_max;

    if (position == "Top Left"){
        x_min = key_min + 2.0;
        x_max = x_min + width;
        y_max = value_max - 4.0;
        y_min = y_max - height;
    }

    else if (position == "Top Right"){
        x_max = key_max - 2.0;
        x_min = x_max - width;
        y_max = value_max - 4.0;
        y_min = y_max - height;
    }

    else if (position == "Bottom Left"){
        x_min = key_min + 2.0;
        x_max = x_min + width;
        y_min = value_min + 2.0;
        y_max = y_min + height;
    }

    else{
        x_max = key_max - 2.0;
        x_min = x_max - width;
        y_min = value_min + 2.0;
        y_max = y_min + height;
    }

    scale_bar->bottomRight->setCoords(x_max, y_min);
    scale_bar->topLeft->setCoords(x_min, y_max);
    double x_mid = ((x_max - x_min) / 2.0) + x_min;
    double y_text = y_max + 1;
    if (font.pointSize() >= 10){
        y_text += (font.pointSize() - 10) / 4;
    }

    scale_bar_text->position->setCoords(x_mid, y_text);

    map_qcp_->show();
}

///
/// \brief MapData::ShowSpectrumViewer
/// \param enabled Whether or not spectrum viewer should be visible
///
void MapData::ShowSpectrumViewer(bool enabled)
{
    spectrum_display_->setVisible(enabled);
}

///
/// \brief MapData::set_baseline
/// \param abcissa
/// \param baseline
/// Sets a single baseline of this object.
void MapData::set_baseline(rowvec abcissa, mat baseline)
{
    first_abcissa_ = abcissa;
    first_baseline_ = baseline;
    univariate_area_ = true;
}

///
/// \brief MapData::set_baselines
/// \param first_abcissa
/// \param second_abcissa
/// \param first_baseline
/// \param second_baseline
/// Sets the baselines of this object
void MapData::set_baselines(rowvec first_abcissa, rowvec second_abcissa,
                            mat first_baseline, mat second_baseline)
{
    first_abcissa_ = first_abcissa;
    second_abcissa_ = second_abcissa;
    first_baseline_ = first_baseline;
    second_baseline_ = second_baseline;
    band_ratio_area_ = true;
}

///
/// \brief MapData::set_fwhm
/// \param mid_lines the FWHM representations
///
void MapData::set_fwhm(mat mid_lines)
{
    mid_lines_ = mid_lines;
    univariate_bandwidth_ = true;
}

///
/// \brief MapData::univariate_area
/// \return Whether or not this is a univariate area map
///
bool MapData::univariate_area()
{
    return univariate_area_;
}

///
/// \brief MapData::band_ratio_area
/// \return Whether or not this is a band ratio area map.
///
bool MapData::band_ratio_area()
{
    return band_ratio_area_;
}

///
/// \brief MapData::univariate_bandwidth
/// \return Whether or not this is a univariate bandwidth map
///
bool MapData::univariate_bandwidth()
{
    return univariate_bandwidth_;
}

///
/// \brief MapData::results_at_position
/// \param x The horizontal position of the mouse click relative to the axes of the map.
/// \param y The vertical position of the mouse click relative to the axes of the map.
/// \return The value of the data point at x and y.
///
double MapData::results_at_position(double x, double y)
{
    return map_->data()->data(x, y);
}

///
/// \brief MapData::first_abcissa
/// \return The abcissa for the first baseline
///
QVector<double> MapData::first_abcissa()
{
    std::vector<double> abcissa = conv_to<std::vector<double> >::from(first_abcissa_);
    return QVector<double>::fromStdVector(abcissa);
}

///
/// \brief MapData::first_baseline
/// \param i The index of first_baseline_ of the spectrum in question.
/// \return The values of the first baseline.
///
QVector<double> MapData::first_baseline(int i)
{
    rowvec baseline_r = first_baseline_.row(i);
    std::vector<double> baseline = conv_to<std::vector<double> >::from(baseline_r);
    return QVector<double>::fromStdVector(baseline);
}

///
/// \brief MapData::second_abcissa
/// \return The abcissa for the second baseline
///
QVector<double> MapData::second_abcissa()
{
    std::vector<double> abcissa = conv_to<std::vector<double> >::from(second_abcissa_);
    return QVector<double>::fromStdVector(abcissa);
}

///
/// \brief MapData::second_baseline
/// \param i index of SpecMap obejct (and also second_baseline_) of the spectrum in question.
/// \return A QVector representing the second baseline for a two-peak map.
///
QVector<double> MapData::second_baseline(int i)
{
    rowvec baseline_r = second_baseline_.row(i);
    std::vector<double> baseline = conv_to<std::vector<double> >::from(baseline_r);
    return QVector<double>::fromStdVector(baseline);
}

///
/// \brief MapData::mid_line
/// \param i The row in the SpecMap object (and therefore also in mid_lines_)
/// corresponding the the spectrum in question.
/// \return A QVector for a line drawn from one edge of the peak in question to
/// the other at the half-maximum of the peak.
/// Used to visualize the FWHM.
QVector<double> MapData::mid_line(int i)
{
    QVector<double> output;
    output.append(mid_lines_(i, 0));
    output.append(mid_lines_(i, 1));
    return output;
}

///
/// \brief MapData::mid_lines
/// \param i The row in the specmap object (and therefore also in mid_lines_)
/// that contains the spectrum in question.
/// \return Returns a QVector for two lines that extend from one edge of two peaks to
/// the other with a vertical position equal to the half maximum of the peak.
///  Used to visualize the FWHM.
QVector<double> MapData::mid_lines(int i)
{
    std::vector<double> lines = conv_to<std::vector<double> >::from(mid_lines_.row(i));
    return QVector<double>::fromStdVector(lines);
}

///
/// \brief MapData::UseGlobalColorScale
/// \param arg1 Whether or not the global color scale should be used for this map.
/// Set the global color scale on or off.
void MapData::UseGlobalColorScale(bool arg1)
{
    cout << "MapData::UseGlobalColorScale()" << endl;
    if (arg1){
        if (using_global_color_scale_){
            cout << "arg1 && using_global_color_scale_" << endl;
            return;
        }
        else{
            cout << "arg1 && !using_global_color_scale_" << endl;
            QCPColorScale *color_scale = qobject_cast<QCPColorScale *>(map_qcp_->plotLayout()->element(0, 1));
            color_scale->setGradient(*main_window_->global_gradient());
            QCPRange data_range = map_->dataRange();
            main_window_->RecalculateGlobalDataRange(&data_range);
            color_scale->setDataRange(*main_window_->global_data_range());
            map_->setGradient(*main_window_->global_gradient());
            map_qcp_->replot();
            return;
        }
    }
    else{
        cout << "!arg1" << endl;
        if(!using_global_color_scale_){
            cout << "!arg1 && !using_global_color_scale_" << endl;
            return;
        }
        else{
            cout << "!arg1 && using_global_color_scale_" << endl;
            using_global_color_scale_ = false;
            QCPColorScale *color_scale = qobject_cast<QCPColorScale *>(map_qcp_->plotLayout()->element(0, 1));
            color_scale->setGradient(gradient_);
            map_->setGradient(gradient_);
            color_scale->setDataRange(map_->dataRange());
            map_qcp_->replot();
            return;
        }
    }
}

///
/// \brief MapData::SetDataRange
/// \param new_range
/// Sets the data range of the map. This is used to allow common data ranges on
/// maps that share the global color scheme, so that for a given numerical value
/// the color is the same on all maps with this scheme.
void MapData::SetDataRange(QCPRange new_range)
{
    QCPColorScale *color_scale = qobject_cast<QCPColorScale *>(map_qcp_->plotLayout()->element(0, 1));
    color_scale->setDataRange(new_range);
    map_->setColorScale(color_scale);
}

///
/// \brief MapData::RescaleMapWidget
/// Changes the proportions of the map window back to the initial proportions.
/// Maintains the longest of the resized dimensions and adjusts the other
/// dimension accordingly.
void MapData::RescaleMapWidget()
{
    double width_ratio = map_qcp_->width()/initial_map_size_.width();
    double height_ratio = map_qcp_->height()/initial_map_size_.height();
    double max_ratio = std::max(width_ratio, height_ratio);
    double new_height = max_ratio * initial_map_size_.height();
    double new_width = max_ratio * initial_map_size_.width();
    map_qcp_->resize(new_width, new_height);
    map_display_->resize(new_width + 50, new_height + 50);
    return;
}

///
/// \brief MapData::LockMapDisplaySize
/// \param lock Whether the window should be locked or unlocked
/// Locks the size of the map window to maintain proportions
void MapData::LockMapDisplaySize(bool lock)
{
    if (lock)
        map_display_->setFixedSize(map_display_->size());
    else

        map_display_->setFixedSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
}

///
/// \brief MapData::ResetMapWidgetSize
/// Returns the size of the map window to the original proportions
void MapData::ResetMapWidgetSize()
{
    map_qcp_->resize(initial_map_size_);
    map_display_->resize(initial_map_size_.width() + 50, initial_map_size_.height() + 50);
    return;
}
