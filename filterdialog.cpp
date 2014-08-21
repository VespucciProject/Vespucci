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
#include "filterdialog.h"
#include "ui_filterdialog.h"

FilterDialog::FilterDialog(QWidget *parent, VespucciWorkspace *ws, int row) :
    QDialog(parent),
    ui(new Ui::FilterDialog)
{
    ui->setupUi(this);
    workspace = ws;
    dataset_ = workspace->DatasetAt(row);
    method_box_ = this->findChild<QComboBox *>("methodComboBox");
    derivative_box_ = this->findChild<QSpinBox *>("derivativeSpinBox");
    polynomial_box_ = this->findChild<QSpinBox *>("polynomialSpinBox");
    window_box_ = this->findChild<QSpinBox *>("windowSpinBox");
}

FilterDialog::~FilterDialog()
{
    delete ui;
}

///
/// \brief FilterDialog::on_methodComboBox_currentIndexChanged
/// \param index
///enables and disables appropriate options based on method selected
void FilterDialog::on_methodComboBox_currentIndexChanged(int index)
{
    if ((index!=2) && (index!=3) &&(derivative_box_->isEnabled()))
        derivative_box_->setEnabled(false);
    if ((index == 2 || index == 3) && (!derivative_box_->isEnabled()))
        derivative_box_->setEnabled(true);
    if ((index != 2) && (index != 3) && (polynomial_box_->isEnabled()))
        polynomial_box_->setEnabled(false);
    if ((index == 2 || index == 3)&&(!polynomial_box_->isEnabled()))
        polynomial_box_->setEnabled(true);
}

void FilterDialog::on_buttonBox_accepted()
{
    switch (method_box_->currentIndex())
    {
    case 0:
        dataset_->MedianFilter(window_box_->value());
        break;
    case 1:
        dataset_->LinearMovingAverage(window_box_->value());
        break;
    case 2:
        dataset_->Derivatize(derivative_box_->value(),
                             polynomial_box_->value(),
                             window_box_->value());
        break;
    case 3:
        dataset_->SavitzkyGolay(polynomial_box_->value(), window_box_->value());
        break;
    case 4:
        dataset_->SingularValue(5);
    default:
        return;
    }
}
