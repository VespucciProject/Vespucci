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
#ifndef CROPDIALOG_H
#define CROPDIALOG_H

#include <QDialog>
#include "Global/vespucciworkspace.h"
#include "Data/Dataset/vespuccidataset.h"

namespace Ui {
class CropDialog;
}

///
/// \brief The CropDialog class
/// A dialog that allows the user to "Crop" the dataset (delete all spectra that
/// are outside of a chosen spatial range).
class CropDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CropDialog(QWidget *parent, VespucciWorkspace *ws, const QModelIndex &dataset_index);
    ~CropDialog();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::CropDialog *ui;

    QDoubleSpinBox *x_min_box_;
    QDoubleSpinBox *x_max_box_;
    QDoubleSpinBox *y_min_box_;
    QDoubleSpinBox *y_max_box_;
    QDoubleSpinBox *wl_max_box_;
    QDoubleSpinBox *wl_min_box_;

    VespucciWorkspace *workspace;
    QSharedPointer<VespucciDataset> dataset_;
};

#endif // CROPDIALOG_H
