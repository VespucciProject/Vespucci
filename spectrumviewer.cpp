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
#include "spectrumviewer.h"
#include "ui_spectrumviewer.h"

SpectrumViewer::SpectrumViewer(MapViewer *parent,
                               MapData *map_data,
                               QVector<double> wavelength,
                               QVector<double> initial_data,
                               const QString x_axis_description,
                               const QString y_axis_description) :
    QDialog(parent),
    ui(new Ui::SpectrumViewer)
{
    ui->setupUi(this);
    map_data_ = map_data;
    spectrum_plot_ = this->findChild<QCustomPlot *>("spectrum");
    spectrum_plot_->addGraph();
    spectrum_plot_->xAxis->setLabel(x_axis_description);
    spectrum_plot_->yAxis->setLabel(y_axis_description);
    spectrum_plot_->graph(0)->setData(wavelength, initial_data);
    spectrum_plot_->replot();
}

SpectrumViewer::~SpectrumViewer()
{
    delete ui;
}

void SpectrumViewer::SetPlot(QVector<double> wavelength,
                             QVector<double> intensity)
{
    spectrum_plot_->graph(0)->setData(wavelength, intensity);
}
