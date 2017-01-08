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
#include "mapresizedialog.h"
#include "ui_mapresizedialog.h"
#include "GUI/Display/mapplot.h"

MapResizeDialog::MapResizeDialog(QWidget *parent, MapPlot *plot) :
    QDialog(parent),
    ui(new Ui::MapResizeDialog)
{
    ui->setupUi(this);
    plot_ = plot;
    ui->xDoubleSpinBox->setValue(double(plot_->axisRect()->size().width()));
    ui->yDoubleSpinBox->setValue(double(plot_->axisRect()->size().height()));
}

MapResizeDialog::~MapResizeDialog()
{
    delete ui;
}

void MapResizeDialog::on_unitComboBox_currentTextChanged(const QString &arg1)
{
    if (arg1 == "px"){
        ui->xDoubleSpinBox->setValue(double(plot_->axisRect()->size().width()));
        ui->yDoubleSpinBox->setValue(double(plot_->axisRect()->size().height()));
        //do stuff for pixels
    }
    else if (arg1 == "in"){
        //do stuff for inches
    }
    else if (arg1 == "cm"){
        //do stuff for centimeters
    }
}
