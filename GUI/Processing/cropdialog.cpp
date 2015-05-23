/*******************************************************************************
    Copyright (C) 2014 Wright State University - All Rights Reserved
    Daniel P. Foose - Author

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
#include "GUI/Processing/cropdialog.h"
#include "ui_cropdialog.h"
#include "Global/vespucciworkspace.h"
#include "Data/Dataset/vespuccidataset.h"

///
/// \brief CropDialog::CropDialog
/// \param parent Parent QWidget
/// \param ws The current workspace
/// \param row Currently selected row
///
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
    wl_min_box_ = this->findChild<QDoubleSpinBox*>("wlMinDoubleSpinBox");
    wl_max_box_ = this->findChild<QDoubleSpinBox*>("wlMaxDoubleSpinBox");

    if (dataset_->non_spatial()){
        x_min_box_->setDisabled(true);
        x_max_box_->setDisabled(true);
        y_min_box_->setDisabled(true);
        y_max_box_->setDisabled(true);
    }

    double wl_min = dataset_->wavelength().min();
    double wl_max = dataset_->wavelength().max();

    QCPRange key_range = dataset_->KeyRange();
    QCPRange value_range = dataset_->ValueRange();

    wl_min_box_->setMinimum(wl_min);
    wl_min_box_->setMaximum(wl_max);
    wl_max_box_->setMinimum(wl_min);
    wl_max_box_->setMaximum(wl_max);

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


///
/// \brief CropDialog::on_buttonBox_accepted
/// Calls crop method of the dataset when user clicks "Ok".
void CropDialog::on_buttonBox_accepted()
{
    double x_min = x_min_box_->value();
    double x_max = x_max_box_->value();
    double y_min = y_min_box_->value();
    double y_max = y_max_box_->value();
    double wl_min = wl_min_box_->value();
    double wl_max = wl_max_box_->value();

    if (dataset_->non_spatial()){
        x_min = std::nan("");
        x_max = std::nan("");
        y_min = std::nan("");
        y_max = std::nan("");
    }
    if ((!std::isnan(x_max)) && ((x_max - x_min) <= 0 || (y_max - y_min <= 0))){
        QMessageBox::warning(this, "Error", "Minimum is larger than maximum!");
        return;
    }

    else{
        dataset_->CropSpectra(x_min, x_max, y_min, y_max, wl_min, wl_max);
    }
    this->close();
    dataset_.clear();

}

///
/// \brief CropDialog::on_buttonBox_rejected
/// Closes window when user clicks "Cancel"
void CropDialog::on_buttonBox_rejected()
{
    this->close();
    dataset_.clear();
}
