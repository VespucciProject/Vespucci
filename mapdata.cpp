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
                 void *parent)
{
    map_display_ = new MapViewer;
    map_qcp_ = map_display_->findChild<QCustomPlot *>("mapView");
    map_ = new QCPColorMap(map_qcp_->xAxis, map_qcp_->yAxis);
    spectrum_display_ = new SpectrumViewer;
    spectrum_qcp_ = spectrum_display_->findChild<QCustomPlot *>("spectrum");
    //map_data_ = temp_map_data;

    //create the objects the pointers point to
    x_axis_description_ = x_axis_description;
    y_axis_description_ = y_axis_description;
    dataset_ = parent;  //parent will always be a specmap, but isn't explicitly cast as such until necessary.
}

void MapData::SetVariables(QString x_axis_description,
                           QString y_axis_description,
                           void *parent)
{
    x_axis_description_ = x_axis_description;
    y_axis_description_ = y_axis_description;
    dataset_ = parent;  //parent will always be a specmap, but isn't explicitly cast as such until necessary.
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

void MapData::set_name(QString name)
{
    name_ = name;
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
void MapData::SetMapData(QCPColorMapData* map_data)
{
    map_->setData(map_data);
}

void MapData::CreateImage(QCPColorGradient color_scheme)
{
    QCustomPlot *plot = map_display_->findChild<QCustomPlot *>("mapView");
    map_->setGradient(color_scheme);
    map_->rescaleDataRange(true);
    plot->addPlottable(map_);
    plot->rescaleAxes(true);

    QCPColorScale *color_scale = new QCPColorScale(plot);
    color_scale->setGradient(color_scheme);
    plot->plotLayout()->addElement(0, 1, color_scale);
    map_->setInterpolate(false);
    int key_size = map_->data()->keySize();
    int value_size = map_->data()->valueSize();
    key_size *= 16;
    value_size *= 16;
    plot->resize(key_size, value_size);
    QSize plot_size = plot->size();
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
//    return &map_data_;
//}
