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
#include "GUI/Processing/filterdialog.h"
#include "ui_filterdialog.h"

///
/// \brief FilterDialog::FilterDialog
/// \param parent Parent QWidget
/// \param ws Current workspace
/// \param row Row of current dataset
///
FilterDialog::FilterDialog(QWidget *parent, QSharedPointer<VespucciWorkspace> ws, const QString &dataset_key) :
    QDialog(parent),
    ui(new Ui::FilterDialog)
{
    ui->setupUi(this);
    workspace_ = ws;
    dataset_ = workspace_->GetDataset(dataset_key);
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
    if ((index != 4) && ui->epsilonSpinBox->isEnabled())
        ui->epsilonSpinBox->setEnabled(false);
    if ((index == 4) && !ui->epsilonSpinBox->isEnabled())
        ui->epsilonSpinBox->setEnabled(true);
    if (((index == 0) || (index == 1))  && !ui->windowSpinBox->isEnabled())
        ui->windowSpinBox->setEnabled(true);
    if ((index!=2) && (ui->derivativeSpinBox->isEnabled()))
        ui->derivativeSpinBox->setEnabled(false);
    if ((index == 2) && (!ui->derivativeSpinBox->isEnabled()))
        ui->derivativeSpinBox->setEnabled(true);
    if ((index == 2) && (!ui->windowSpinBox->isEnabled()))
        ui->windowSpinBox->setEnabled(true);
    if ((index != 2) && (ui->polynomialSpinBox->isEnabled()))
        ui->polynomialSpinBox->setEnabled(false);
    if ((index == 2)&&(!ui->polynomialSpinBox->isEnabled()))
        ui->polynomialSpinBox->setEnabled(true);
    if ((index != 3))
        ui->singularValuesSpinBox->setDisabled(true);
    if (index == 3){
        ui->singularValuesSpinBox->setEnabled(true);
        ui->windowSpinBox->setDisabled(true);
    }
}

///
/// \brief FilterDialog::on_buttonBox_accepted
/// Trigger relevant filtering method when "Ok" is selected
void FilterDialog::on_buttonBox_accepted()
{
    int SVD_rank;
    switch (ui->methodComboBox->currentIndex())
    {
    case 0:
        try{
            dataset_->MedianFilter(ui->windowSpinBox->value());
        }
        catch(exception e){
            workspace_->main_window()->DisplayExceptionWarning(e);
        }
        break;

    case 1:
        try{
            dataset_->LinearMovingAverage(ui->windowSpinBox->value());
        }
        catch(exception e){
            workspace_->main_window()->DisplayExceptionWarning(e);
        }
        break;
    case 2:
        try{
            dataset_->SavitzkyGolay(ui->derivativeSpinBox->value(),
                                 ui->polynomialSpinBox->value(),
                                 ui->windowSpinBox->value());
        }
        catch(exception e){
            workspace_->main_window()->DisplayExceptionWarning(e);
        }
        break;
    case 3:
        try{
            dataset_->SingularValue(ui->singularValuesSpinBox->value());
        }
        catch(exception e){
            workspace_->main_window()->DisplayExceptionWarning(e);
        }
        break;
    case 4:
        try{
            SVD_rank = dataset_->QUIC_SVD(ui->epsilonSpinBox->value());
        }
        catch(exception e){
            workspace_->main_window()->DisplayExceptionWarning(e);
            break;
        }
        QMessageBox::information(this,
                                 "QUIC_SVD",
                                 "The rank of the approximation is " +
                                 QString::number(SVD_rank) + ".");
        break;
    default:
        return;
    }
    close();
    dataset_.clear(); //let go of pointer to dataset

}

///
/// \brief FilterDialog::on_buttonBox_rejected
/// Close window when "Cancel" selected.
void FilterDialog::on_buttonBox_rejected()
{
    close();
    dataset_.clear(); //let go of pointer to dataset
}
