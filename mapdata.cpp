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


MapData::MapData(QString x_axis_description,
                 QString y_axis_description,
                 colvec x,
                 colvec y,
                 colvec results,
                 SpecMap *parent,
                 QString *directory,
                 QCPColorGradient gradient,
                 int source_index)
{
    name_ = parent->name();
    directory_ = directory;

    univariate_area_ = false;
    band_ratio_area_ = false;
    univariate_bandwidth_ = false;

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



    spectrum_qcp_ = spectrum_display_->findChild<QCustomPlot *>("spectrum");
    //create the objects the pointers point to

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

QString MapData::name()
{
    return name_;
}

QString MapData::type()
{
    return type_;
}

int MapData::source_index()
{
    return source_index_;
}

void MapData::set_type(QString type)
{
    type_ = type;
}

void MapData::set_name(QString name, QString type)
{
    name_ = name;
    type_ = type;
    QString window_title = dataset_->name() + "—" + name + " " + type;
    map_display_->setWindowTitle(window_title);
}

void MapData::ShowMapWindow()
{
    if(!map_display_->isVisible()){
        map_display_->show();
    }
    if(!map_display_->isActiveWindow()){
        map_display_->activateWindow();
    }
}

//Plots the data on the MapViewer window
void MapData::SetMapData(QCPColorMapData *map_data)
{
    map_->setData(map_data);
}

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

void MapData::SetXDescription(QString description)
{
    x_axis_description_ = description;
}

void MapData::SetYDescription(QString description)
{
    y_axis_description_ = description;
}

bool MapData::interpolate()
{
    if (!map_qcp_->hasPlottable(map_)){
        map_qcp_->addPlottable(map_);
    }
    return map_->interpolate();
}

void MapData::setInterpolate(bool enabled)
{
    if (!map_qcp_->hasPlottable(map_)){
        map_qcp_->addPlottable(map_);
    }
    map_->setInterpolate(enabled);
    map_qcp_->replot();
}

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

bool MapData::saveBmp(const QString &fileName, int width, int height, double scale)
{
    map_qcp_->setBackground(Qt::white);
    map_qcp_->replot();
    bool success = map_qcp_->saveBmp(fileName, width, height, scale);

    map_qcp_->setBackground(map_display_->palette().window());
    map_qcp_->replot();
    return success;
}

bool MapData::savePng(const QString &fileName, int width, int height, double scale, int quality)
{
    map_qcp_->setBackground(Qt::transparent);
    map_qcp_->replot();
    bool success = map_qcp_->savePng(fileName, width, height, scale, quality);
    map_qcp_->setBackground(map_display_->palette().window());
    map_qcp_->replot();
    return success;
}

bool MapData::saveJpg(const QString &fileName, int width, int height, double scale, int quality)
{
    map_qcp_->setBackground(Qt::white);
    map_qcp_->replot();
    bool success = map_qcp_->saveJpg(fileName, width, height, scale, quality);
    map_qcp_->setBackground(map_display_->palette().window());
    map_qcp_->replot();
    return success;
}

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

void MapData::setGradient(const QCPColorGradient &gradient)
{
    map_->setGradient(gradient);
    QCPColorScale *color_scale = qobject_cast<QCPColorScale *>(color_scale_);
    color_scale->setGradient(gradient);
    map_qcp_->replot();
}

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

void MapData::ShowSpectrumViewer(bool enabled)
{
    spectrum_display_->setVisible(enabled);
}

void MapData::set_baseline(rowvec abcissa, mat baseline)
{
    first_abcissa_ = abcissa;
    first_baseline_ = baseline;
    univariate_area_ = true;
}

void MapData::set_baselines(rowvec first_abcissa, rowvec second_abcissa,
                            mat first_baseline, mat second_baseline)
{
    first_abcissa_ = first_abcissa;
    second_abcissa_ = second_abcissa;
    first_baseline_ = first_baseline;
    second_baseline_ = second_baseline;
    band_ratio_area_ = true;
}

void MapData::set_fwhm(mat mid_lines)
{
    mid_lines_ = mid_lines;
    univariate_bandwidth_ = true;
}

bool MapData::univariate_area()
{
    return univariate_area_;
}

bool MapData::band_ratio_area()
{
    return band_ratio_area_;
}

bool MapData::univariate_bandwidth()
{
    return univariate_bandwidth_;
}

double MapData::results_at_position(double x, double y)
{
    return map_->data()->data(x, y);
}

QVector<double> MapData::first_abcissa()
{
    std::vector<double> abcissa = conv_to<std::vector<double> >::from(first_abcissa_);
    return QVector<double>::fromStdVector(abcissa);
}

QVector<double> MapData::first_baseline(int i)
{
    rowvec baseline_r = first_baseline_.row(i);
    std::vector<double> baseline = conv_to<std::vector<double> >::from(baseline_r);
    return QVector<double>::fromStdVector(baseline);
}

QVector<double> MapData::second_abcissa()
{
    std::vector<double> abcissa = conv_to<std::vector<double> >::from(second_abcissa_);
    return QVector<double>::fromStdVector(abcissa);
}

QVector<double> MapData::second_baseline(int i)
{
    rowvec baseline_r = second_baseline_.row(i);
    std::vector<double> baseline = conv_to<std::vector<double> >::from(baseline_r);
    return QVector<double>::fromStdVector(baseline);
}

QVector<double> MapData::mid_line(int i)
{
    QVector<double> output;
    output.append(mid_lines_(i, 0));
    output.append(mid_lines_(i, 1));
    return output;
}

QVector<double> MapData::mid_lines(int i)
{
    std::vector<double> lines = conv_to<std::vector<double> >::from(mid_lines_.row(i));
    return QVector<double>::fromStdVector(lines);
}
