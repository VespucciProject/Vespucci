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
#include "cropdialog.h"
#include "ui_cropdialog.h"

CropDialog::CropDialog(QWidget *parent, VespucciWorkspace *ws, int row) :
    QDialog(parent),
    ui(new Ui::CropDialog)
{
    ui->setupUi(this);
    workspace = ws;
    dataset_ = workspace->DatasetAt(row);

    x_min_box_ = this->findChild<QDoubleSpinBox*>("xMinDoubleSpinBox");
    x_max_box_ = this->findChild<QDoubleSpinBox*>("xMaxDoubleSpinBox");
    y_min_box_ = this->findChild<QDoubleSpinBox*>("yMinDoubleSpinBox");
    y_max_box_ = this->findChild<QDoubleSpinBox*>("yMaxDoubleSpinBox");

    QCPRange key_range = dataset_->KeyRange();
    QCPRange value_range = dataset_->ValueRange();

    x_min_box_->setMinimum(key_range.lower);
    x_min_box_->setMaximum(key_range.upper);
    x_max_box_->setMinimum(key_range.lower);
    x_max_box_->setMaximum(key_range.upper);

    y_min_box_->setMinimum(value_range.lower);
    y_min_box_->setMaximum(value_range.upper);
    y_max_box_->setMinimum(value_range.lower);
    y_max_box_->setMaximum(value_range.upper);

}


CropDialog::~CropDialog()
{
    delete ui;
}

void CropDialog::on_buttonBox_accepted()
{

    double x_min = x_min_box_->value();
    double x_max = x_max_box_->value();
    double y_min = y_min_box_->value();
    double y_max = y_max_box_->value();

    if ((x_max - x_min) <= 0 || (y_max - y_min <= 0)){
        QMessageBox::warning(this, "Error", "Minimum is larger than maximum!");
        return;
    }

    else{
        dataset_->CropSpectra(x_min, x_max, y_min, y_max);
    }
    this->close();

}

void CropDialog::on_buttonBox_rejected()
{
    this->close();
}
