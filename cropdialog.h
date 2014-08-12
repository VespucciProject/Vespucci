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
#ifndef CROPDIALOG_H
#define CROPDIALOG_H

#include <QDialog>
#include "vespucciworkspace.h"
#include "specmap.h"

namespace Ui {
class CropDialog;
}

class CropDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CropDialog(QWidget *parent, VespucciWorkspace *ws, int row);
    ~CropDialog();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::CropDialog *ui;

    QDoubleSpinBox *x_min_box_;
    QDoubleSpinBox *x_max_box_;
    QDoubleSpinBox *y_min_box_;
    QDoubleSpinBox *y_max_box_;

    VespucciWorkspace *workspace;
    QSharedPointer<SpecMap> dataset_;
};

#endif // CROPDIALOG_H