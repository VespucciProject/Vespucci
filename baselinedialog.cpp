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
#include "baselinedialog.h"
#include "ui_baselinedialog.h"

BaselineDialog::BaselineDialog(QWidget *parent, VespucciWorkspace *ws, int row) :
    QDialog(parent),
    ui(new Ui::BaselineDialog)
{
    ui->setupUi(this);
    workspace = ws;
    dataset_ = workspace->DatasetAt(row);

    window_size_box_ = this->findChild<QSpinBox *>("windowSpinBox");
    method_box_ = this->findChild<QComboBox *>("methodComboBox");
    iterations_box_ = this->findChild<QSpinBox *>("iterationsSpinBox");
}

BaselineDialog::~BaselineDialog()
{
    delete ui;
}

void BaselineDialog::on_buttonBox_accepted()
{
    int window_size = window_size_box_->value();
    QString method = method_box_->currentText();
    int iterations = iterations_box_->value();

    for (int i = 0; i < iterations; ++i)
        dataset_->Baseline(method, window_size);
    this->close();
}

void BaselineDialog::on_buttonBox_rejected()
{
    this->close();
}
