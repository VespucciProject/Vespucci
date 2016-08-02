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
#include "GUI/Analysis/plsdialog.h"
#include "ui_plsdialog.h"

PLSDialog::PLSDialog(QWidget *parent, QSharedPointer<VespucciWorkspace> ws, const QString &dataset_key) :
    QDialog(parent),
    ui(new Ui::PLSDialog)
{
    ui->setupUi(this);
    workspace_ = ws;
    dataset_ = workspace_->GetDataset(dataset_key);

    ui->nameLineEdit->setEnabled(false);

    ui->browsePushButton->setVisible(false);
    ui->datasetLabel->setVisible(false);
    ui->fileLabel->setVisible(false);
    ui->fileLineEdit->setVisible(false);
    ui->datasetComboBox->setVisible(false);

}

PLSDialog::~PLSDialog()
{
    delete ui;
}

///
/// \brief PLSDialog::on_buttonBox_accepted
/// Call the appropriate function in VespucciDataset to perform PLS based on what
/// the user has entered when the user clicks "Ok"
void PLSDialog::on_buttonBox_accepted()
{
    int components = ui->componentsSpinBox->value();
    QString name = ui->nameLineEdit->text();
    try{
        dataset_->PartialLeastSquares(name, components);
    }catch(exception e){
        workspace_->main_window()->DisplayExceptionWarning(e);
    }
    close();
    dataset_.clear();
}

///
/// \brief PLSDialog::on_buttonBox_rejected
/// Close the window when the user clicks "Cancel"
void PLSDialog::on_buttonBox_rejected()
{
    close();
    dataset_.clear();
}

void PLSDialog::on_predictionCheckBox_clicked(bool checked)
{
    ui->componentsSpinBox->setVisible(!checked);
}
