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
#include <QGuiApplication>
#include <QScreen>
#include <limits>

MapResizeDialog::MapResizeDialog(QWidget *parent, MapPlot *plot) :
    QDialog(parent),
    ui(new Ui::MapResizeDialog)
{
    ui->setupUi(this);
    plot_ = plot;
    current_size_ = plot_->axisRect()->size();
    ui->xDoubleSpinBox->setMaximum(std::numeric_limits<double>::max());
    ui->yDoubleSpinBox->setMaximum(std::numeric_limits<double>::max());
    ui->xDoubleSpinBox->setValue(double(current_size_.width()));
    ui->yDoubleSpinBox->setValue(double(current_size_.height()));
    current_units_ = "px";
    screen_ = QGuiApplication::primaryScreen();
}

MapResizeDialog::~MapResizeDialog()
{
    delete ui;
}

void MapResizeDialog::on_unitComboBox_currentTextChanged(const QString &arg1)
{
    double length = ui->xDoubleSpinBox->value();
    double height = ui->yDoubleSpinBox->value();
    double new_length = length;
    double new_height = height;
    if (arg1 == "px"){
        if (current_units_ == "in"){
            new_length = length * screen_->logicalDotsPerInchX();
            new_height = height * screen_->logicalDotsPerInchY();
        }
        else if (current_units_ == "cm"){
            new_length = (length / 2.54) * screen_->logicalDotsPerInchX();
            new_height = (height / 2.54) * screen_->logicalDotsPerInchY();
        }
        new_length = std::round(new_length);
        new_height = std::round(new_height);
    }
    else if (arg1 == "in"){
        if (current_units_ == "px"){
            new_length = length / screen_->logicalDotsPerInchX();
            new_height = length / screen_->logicalDotsPerInchY();
        }
        else if (current_units_ == "cm"){
            new_length = length / 2.54;
            new_height = height / 2.54;
        }
    }
    else if (arg1 == "cm"){
       if (current_units_ == "in"){
           new_length = length * 2.54;
           new_height = height * 2.54;
       }
       else if (current_units_ == "px"){
           new_length = (length / screen_->logicalDotsPerInchX()) * 2.54;
           new_height = (height / screen_->logicalDotsPerInchY()) * 2.54;
       }
    }
    ui->xDoubleSpinBox->setValue(new_length);
    ui->yDoubleSpinBox->setValue(new_height);
    current_units_ = arg1;
}

void MapResizeDialog::on_buttonBox_accepted()
{
    double length = ui->xDoubleSpinBox->value();
    double height = ui->yDoubleSpinBox->value();
    int length_px, height_px;

    if (current_units_ == "px"){
        length_px = length;
        height_px = height;
    }
    else if (current_units_ == "in"){
        length_px = std::round(length * screen_->logicalDotsPerInchX());
        height_px = std::round(height * screen_->logicalDotsPerInchY());
    }
    else if (current_units_ == "cm"){
        length_px = std::round((length / 2.54) * screen_->logicalDotsPerInchX());
        height_px = std::round((height / 2.54) * screen_->logicalDotsPerInchY());
    }
    else{
        return;
    }

    plot_->axisRect()->setMinimumSize(length_px, height_px);
    plot_->axisRect()->setMaximumSize(length_px, height_px);
}
