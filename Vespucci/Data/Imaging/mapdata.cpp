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
#include "Data/Imaging/mapdata.h"
#include "Global/global.h"
#include <QtSvg>


///
/// \brief MapData::~MapData
///Deletes everything the new keyword is used on in this object.
/// Destructor triggered when this is removed from VespucciDataset list.
MapData::MapData(QString name,
                 QString type,
                 QStringList data_keys,
                 uword data_column,
                 QSharedPointer<VespucciWorkspace> workspace)
    :name_(name), type_(type)
{
    workspace_ = workspace;
    if (workspace_->GetMatrix(data_keys).n_cols >= data_column)
        throw invalid_argument("Requested column out of bounds");
    map_display_ = new MapViewer(workspace->main_window(), QSharedPointer<MapData>(this), workspace);
    map_display_->setWindowTitle(name);
    QSharedPointer<VespucciDataset> dataset = workspace_->GetDataset(data_keys.first());
    vec x = dataset->x();
    vec y = dataset->y();
    vec z = workspace_->GetMatrix(data_keys).col(data_column);
    map_qcp_->SetMapData(x, y, z);
}

MapData::~MapData()
{
    delete map_display_;
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

void MapData::SetMapPlot(MapPlot *plot)
{
    map_qcp_ = plot;
}


///
/// \brief MapData::set_name
/// \param name
/// \param type
/// Sets the title of the window, default name for saved files and display name in main window.
void MapData::SetName(QString name, QString type)
{
    name_ = name;
    type_ = type;
    QString window_title = dataset_->name() + "—" + name + " " + type;
    map_display_->setWindowTitle(window_title);
}

void MapData::SetGlobalGradient(QString name)
{
    if (workspace_->GlobalGradientKeys().contains(name)){
        global_gradient_key_ = name;
        UpdateGlobalGradient();
    }
}

void MapData::UpdateGlobalGradient()
{
    Vespucci::GlobalGradient gradient = workspace_->GetGlobalGradient(global_gradient_key_);
    map_qcp_->setDataRange(gradient.range);
    map_qcp_->SetGradient(gradient.gradient);
}

QStringList MapData::keys()
{
    return QStringList({dataset_->name(), name_});
}

///
/// \brief MapData::ShowMapWindow
/// Shows or hides the map window.
void MapData::ShowMapWindow(bool show)
{
    map_display_->setVisible(show);
    if (show && !map_display_->isActiveWindow())
        map_display_->activateWindow();
}

///
/// \brief MapData::HideMapWindow
/// Closes the map window.
void MapData::HideMapWindow()
{
    if(map_display_->isVisible())
        map_display_->close();
}

///
/// \brief MapData::MapWindowVisible
/// \return Whether or not map window is visible.
///
bool MapData::MapWindowVisible()
{
    return map_display_->isVisible();
}

///
/// \brief MapData::CreateImage
/// \param color_scheme
/// \param interpolation
/// Creates the image by sending data to the QCustomPlot widgets.
void MapData::CreateImage(QCPColorGradient color_scheme, bool interpolation, int tick_count)
{
    map_qcp_->SetGradient(color_scheme);
    map_qcp_->rescaleDataRange(true);
    map_qcp_->rescaleAxes(true);

    map_qcp_->setInterpolate(interpolation);

    int key_size = map_qcp_->keySize();
    int value_size = map_qcp_->valueSize();
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
    return map_qcp_->interpolate();
}

///
/// \brief MapData::setInterpolate
/// \param enabled Whether or not image should be interpolated.
/// Toggles image interpolation on or off. See QCustomPlot::QCPColorMap::setInterpolate.
void MapData::setInterpolate(bool enabled)
{
    map_qcp_->setInterpolate(enabled);
    map_qcp_->replot();
}

///
/// \brief MapData::ShowColorScale
/// \param enabled Whether or not color scale should be visible.
/// Toggles the color scale bar on or off.
void MapData::ShowColorScale(bool enabled)
{
    map_qcp_->ColorScaleVisible(enabled);
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
/// \brief MapData::savePDF
/// \param fileName
/// \param width
/// \param height
/// \param scale
/// \return
/// Saves the map as a PDF. See QCustomPlot::saveBmp.
bool MapData::savePdf(const QString &fileName, int width, int height)
{
    bool success = map_qcp_->savePdf(fileName, true, width, height, "Vespucci", name());
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
/// \brief MapData::setGradient
/// \param gradient
/// Sets the color gradient of the map.
void MapData::setGradient(const QCPColorGradient &gradient)
{
    global_gradient_key_ = QString();
    map_qcp_->SetGradient(gradient);
    map_qcp_->rescaleDataRange();
    map_qcp_->replot();
}

void MapData::SetColorScaleTickCount(int ticks)
{
    map_qcp_->SetColorScaleTickCount(ticks);
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
    QCPRange key_range = map_qcp_->keyRange();
    QCPRange value_range = map_qcp_->valueRange();

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
/// \brief MapData::SetDataRange
/// \param new_range
/// Sets the data range of the map. This is used to allow common data ranges on
/// maps that share the global color scheme, so that for a given numerical value
/// the color is the same on all maps with this scheme.
void MapData::SetDataRange(QCPRange new_range)
{
    map_qcp_->setDataRange(new_range);
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


void MapData::SetFonts(const QFont &font)
{
    map_qcp_->SetFonts(font);
}

QString MapData::global_gradient_key()
{
    return global_gradient_key_;
}

///
/// \brief MapData::min
/// \return
///
double MapData::min()
{
    return map_qcp_->min();
}

double MapData::max()
{
    return map_qcp_->max();
}
