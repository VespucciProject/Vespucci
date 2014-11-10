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
#include "filterdialog.h"
#include "ui_filterdialog.h"

///
/// \brief FilterDialog::FilterDialog
/// \param parent Parent QWidget
/// \param ws Current workspace
/// \param row Row of current dataset
///
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
    singular_values_box_ = this->findChild<QSpinBox *>("singularValuesSpinBox");
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
    if ((index!=2) && (derivative_box_->isEnabled()))
        derivative_box_->setEnabled(false);
    if ((index == 2) && (!derivative_box_->isEnabled()))
        derivative_box_->setEnabled(true);
    if ((index != 2) && (polynomial_box_->isEnabled()))
        polynomial_box_->setEnabled(false);
    if ((index == 2)&&(!polynomial_box_->isEnabled()))
        polynomial_box_->setEnabled(true);
    if ((index != 3))
        singular_values_box_->setDisabled(true);
    if (index == 3){
        singular_values_box_->setEnabled(true);
        window_box_->setDisabled(true);
    }
}

///
/// \brief FilterDialog::on_buttonBox_accepted
/// Trigger relevant filtering method when "Ok" is selected
void FilterDialog::on_buttonBox_accepted()
{
    switch (method_box_->currentIndex())
    {
    case 0:
        try{
            dataset_->MedianFilter(window_box_->value());
        }
        catch(exception e){
            workspace->main_window()->DisplayExceptionWarning(e);
        }
        break;

    case 1:
        try{
            dataset_->LinearMovingAverage(window_box_->value());
        }
        catch(exception e){
            workspace->main_window()->DisplayExceptionWarning(e);
        }
        break;
    case 2:
        try{
            dataset_->SavitzkyGolay(derivative_box_->value(),
                                 polynomial_box_->value(),
                                 window_box_->value());
        }
        catch(exception e){
            workspace->main_window()->DisplayExceptionWarning(e);
        }
        break;
    case 3:
        try{
            dataset_->SingularValue(singular_values_box_->value());
        }
        catch(exception e){
            workspace->main_window()->DisplayExceptionWarning(e);
        }
        break;
    default:
        return;
    }
    this->close();
    dataset_.clear(); //let go of pointer to dataset

}

///
/// \brief FilterDialog::on_buttonBox_rejected
/// Close window when "Cancel" selected.
void FilterDialog::on_buttonBox_rejected()
{
    this->close();
    dataset_.clear(); //let go of pointer to dataset
}
