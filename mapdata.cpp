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
                 QCPColorGradient gradient)
{
    name_ = parent->name();
    directory_ = directory;
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

    spectrum_display_ = new SpectrumViewer;
    spectrum_qcp_ = spectrum_display_->findChild<QCustomPlot *>("spectrum");
    map_qcp_->setBackground(Qt::transparent);
    //create the objects the pointers point to
    x_axis_description_ = x_axis_description;
    y_axis_description_ = y_axis_description;
    dataset_ = parent;
}


QString MapData::name()
{
    return name_;
}

QString MapData::type()
{
    return type_;
}

QString MapData::source_index()
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

//void MapData::setKeyRange(const QCPRange& range)
//{
//    map_data_.setKeyRange(range);
//}

//void MapData::setValueRange(const QCPRange& range)
//{
//    map_data_.setValueRange(range);
//}

//void MapData::setKeySize(int size)
//{
//    cout << "MapData::setKeySize" << endl;
//   map_data_.setKeySize(size);
//}

//void MapData::setValueSize(int size)
//{
//    //++size;
//    map_data_.setValueSize(size);
//}

//void MapData::setSize(int key_size, int value_size)
//{
//    cout << "MapData::setSize" << endl;
//    map_data_.setSize(key_size, value_size);
//}

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
    map_qcp_->addPlottable(map_);
    map_qcp_->rescaleAxes(true);

    QCPColorScale *color_scale = new QCPColorScale(map_qcp_);
    color_scale->setGradient(color_scheme);
    map_qcp_->plotLayout()->addElement(0, 1, color_scale);
    map_->setInterpolate(interpolation);
    int key_size = map_->data()->keySize();
    int value_size = map_->data()->valueSize();
    key_size *= 9;
    value_size *= 9;
    key_size +=75;
    value_size +=75;
    map_qcp_->resize(key_size, value_size);
    QSize plot_size = map_qcp_->size();
    map_display_->resize(plot_size);

}

//void MapData::setData(double key, double value, double z)
//{
//    map_data_.setData(key, value, z);
//}

void MapData::SetXDescription(QString description)
{
    x_axis_description_ = description;
}

void MapData::SetYDescription(QString description)
{
    y_axis_description_ = description;
}

//QCPColorMapData *MapData::map_data()
//{
//    return map_data_;
//}

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

bool MapData::saveBmp(const QString &fileName, int width, int height, double scale)
{

    map_qcp_->setBackground(Qt::white);
    if (!map_qcp_->hasPlottable(map_)){
        map_qcp_->addPlottable(map_);
    }

    map_qcp_->replot();
    bool success = map_qcp_->saveBmp(fileName, width, height, scale);
    map_qcp_->setBackground(Qt::transparent);
    map_qcp_->replot();

    return success;
}

bool MapData::savePng(const QString &fileName, int width, int height, double scale, int quality)
{
    if (!map_qcp_->hasPlottable(map_)){
        map_qcp_->addPlottable(map_);
        map_qcp_->replot();
    }

    bool success = map_qcp_->savePng(fileName, width, height, scale, quality);
    return success;
}

bool MapData::saveJpg(const QString &fileName, int width, int height, double scale, int quality)
{
    map_qcp_->setBackground(Qt::white);
    if (!map_qcp_->hasPlottable(map_)){
        map_qcp_->addPlottable(map_);
    }
    map_qcp_->replot();
    bool success = map_qcp_->saveJpg(fileName, width, height, scale, quality);
    map_qcp_->setBackground(Qt::transparent);
    map_qcp_->replot();
    return success;

}

bool MapData::saveTiff(const QString &fileName, int width, int height, double scale, int quality)
{
    if (!map_qcp_->hasPlottable(map_)){
        map_qcp_->addPlottable(map_);
    }
    map_qcp_->replot();
    bool success = map_qcp_->saveRastered(fileName, width, height, scale, "TIF", quality);
    return success;
}
