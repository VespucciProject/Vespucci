/*******************************************************************************
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
*******************************************************************************/
#ifndef SPECTRUMVIEWER_H
#define SPECTRUMVIEWER_H

#include <QDialog>
#include "mapviewer.h"
#include "dataviewer.h"
class MapViewer;
class MapData;
class SpecMap;
class DataViewer;

namespace Ui {
class SpectrumViewer;
}


class SpectrumViewer : public QDialog
{
    Q_OBJECT

public:
    SpectrumViewer(MapViewer *parent,
                   MapData *map_data,
                   const QString x_axis_description,
                   const QString y_axis_description, SpecMap *dataset, QSize widget_size, QString directory);
    SpectrumViewer(DataViewer *parent,
                   SpecMap *dataset,
                   int endmember,
                   QString directory,
                   QString type);

    SpectrumViewer(QVector<double> abcissa,
                   QVector<double> intensities,
                   const QString x_axis_description,
                   const QString y_axis_description);

    ~SpectrumViewer();
    void SetPlot(QVector<double> wavelength,
                 QVector<double> intensity);

    void SetSecondPlot(QVector<double> abcissa, QVector<double> intensities);
    void SetSecondPlot(QVector<double> first_abcissa, QVector<double> second_abcissa,
                       QVector<double> first_intensities, QVector<double> second_intensities);
private slots:
    void MapClicked(QCPAbstractPlottable *plottable, QMouseEvent *event);
    void on_pushButton_clicked();

private:
    Ui::SpectrumViewer *ui;
    QCustomPlot *spectrum_plot_;
    MapViewer *parent_;
    MapData *map_data_;
    SpecMap *dataset_;
    QSize widget_size_;
    QString directory_;
    int current_index_;
    double current_x_;
    double current_y_;
    QLabel *coordinate_label_;
    QLabel *value_label_;
    bool linked_to_map_;
};

#endif // SPECTRUMVIEWER_H
