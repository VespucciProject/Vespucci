/*******************************************************************************
    Copyright (C) 2014-2017 Wright State University - All Rights Reserved
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
CropDialog::CropDialog(QWidget *parent, QSharedPointer<VespucciWorkspace> ws, const QString &dataset_key) :
    QDialog(parent),
    ui(new Ui::CropDialog)
{
    ui->setupUi(this);
    workspace_ = ws;
    dataset_ = workspace_->GetDataset(dataset_key);

    if (dataset_->non_spatial()){
        ui->xMinDoubleSpinBox->setDisabled(true);
        ui->xMaxDoubleSpinBox->setDisabled(true);
        ui->yMinDoubleSpinBox->setDisabled(true);
        ui->yMaxDoubleSpinBox->setDisabled(true);
    }

    double wl_min = dataset_->wavelength().min();
    double wl_max = dataset_->wavelength().max();

    QCPRange key_range = dataset_->KeyRange();
    QCPRange value_range = dataset_->ValueRange();

    ui->wlMinDoubleSpinBox->setMinimum(wl_min);
    ui->wlMinDoubleSpinBox->setMaximum(wl_max);
    ui->wlMaxDoubleSpinBox->setMinimum(wl_min);
    ui->wlMaxDoubleSpinBox->setMaximum(wl_max);

    ui->xMinDoubleSpinBox->setMinimum(key_range.lower);
    ui->xMinDoubleSpinBox->setMaximum(key_range.upper);
    ui->xMaxDoubleSpinBox->setMinimum(key_range.lower);
    ui->xMaxDoubleSpinBox->setMaximum(key_range.upper);

    ui->yMinDoubleSpinBox->setMinimum(value_range.lower);
    ui->yMinDoubleSpinBox->setMaximum(value_range.upper);
    ui->yMaxDoubleSpinBox->setMinimum(value_range.lower);
    ui->yMaxDoubleSpinBox->setMaximum(value_range.upper);

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
    double x_min = ui->xMinDoubleSpinBox->value();
    double x_max = ui->xMaxDoubleSpinBox->value();
    double y_min = ui->yMinDoubleSpinBox->value();
    double y_max = ui->yMaxDoubleSpinBox->value();
    double wl_min = ui->wlMinDoubleSpinBox->value();
    double wl_max = ui->wlMaxDoubleSpinBox->value();

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
    close();
    dataset_.clear();

}

///
/// \brief CropDialog::on_buttonBox_rejected
/// Closes window when user clicks "Cancel"
void CropDialog::on_buttonBox_rejected()
{
    close();
    dataset_.clear();
}
