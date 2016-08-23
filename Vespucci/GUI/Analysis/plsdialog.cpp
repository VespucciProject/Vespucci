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
#include "Data/Analysis/multianalyzer.h"

PLSDialog::PLSDialog(QWidget *parent, QSharedPointer<VespucciWorkspace> ws, const QString &dataset_key) :
    QDialog(parent),
    ui(new Ui::PLSDialog)
{
    ui->setupUi(this);
    workspace_ = ws;
    dataset_ = workspace_->GetDataset(dataset_key);
    ui->matrixDisplayLabel->setVisible(false);
    ui->matrixLabel->setVisible(false);
    ui->selectPushButton->setVisible(false);
    matrix_selection_dialog_ = new MatrixSelectionDialog(this, workspace_->dataset_tree_model());
    connect(matrix_selection_dialog_, &MatrixSelectionDialog::MatrixSelected,
            this, &PLSDialog::MatrixSelected);
}

PLSDialog::PLSDialog(QSharedPointer<VespucciWorkspace> ws, const QStringList &dataset_keys)
    : QDialog(ws->main_window()),
      ui(new Ui::PLSDialog)
{
    ui->setupUi(this);
    workspace_ = ws;
    dataset_keys_ = dataset_keys;
    if (dataset_keys_.isEmpty()) {
        close();
        return;
    }
    dataset_ = workspace_->GetDataset(dataset_keys.first());
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
    if (!dataset_keys_.isEmpty() || !dataset_.isNull()){
        int components = ui->componentsSpinBox->value();
        QString name = ui->nameLineEdit->text();
        QString type = ui->analysisTypeComboBox->currentText();

        if (!dataset_keys_.isEmpty()){
            try{
                MultiAnalyzer analyzer(workspace_, dataset_keys_);
                if (type == "Classification (Principal Components)")
                    analyzer.PartialLeastSquares(name, components);
                else if (type == "Calibration")
                    analyzer.PLSCalibration(name, control_keys_);
                else if (type == "Train PLS-DA")
                    analyzer.TrainPLSDA(name, control_keys_);
            }catch(exception e){
                workspace_->main_window()->DisplayExceptionWarning(e);
            }
        }
        else{
            try{
                if (type == "Classification (Principal Components)")
                    dataset_->PartialLeastSquares(name, components);
                else if (type == "Calibration")
                    dataset_->PLSCalibration(name, control_keys_);
                else if (type == "Train PLS-DA")
                    dataset_->TrainPLSDA(name, control_keys_);
            }catch(exception e){
                workspace_->main_window()->DisplayExceptionWarning(e);
            }
        }
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

void PLSDialog::on_analysisTypeComboBox_currentIndexChanged(const QString &arg1)
{
    if (arg1 == "Classification (Principal Components)"){
        ui->matrixDisplayLabel->setVisible(false);
        ui->matrixLabel->setVisible(false);
        ui->selectPushButton->setVisible(false);
        ui->predictionCheckBox->setVisible(true);
        ui->componentsLabel->setVisible(true);
        ui->componentsSpinBox->setVisible(true);
    }
    else if (arg1 == "Calibration"){
        ui->matrixDisplayLabel->setVisible(true);
        ui->matrixLabel->setVisible(true);
        ui->selectPushButton->setVisible(true);
        ui->predictionCheckBox->setVisible(false);
        ui->componentsLabel->setVisible(false);
        ui->componentsSpinBox->setVisible(false);
    }
    else if (arg1 == "Train PLS-DA"){
        ui->matrixDisplayLabel->setVisible(true);
        ui->matrixLabel->setVisible(true);
        ui->selectPushButton->setVisible(true);
        ui->predictionCheckBox->setVisible(false);
        ui->componentsLabel->setVisible(false);
        ui->componentsSpinBox->setVisible(false);
    }
}

void PLSDialog::on_selectPushButton_clicked()
{
    matrix_selection_dialog_->show();
}

void PLSDialog::MatrixSelected(QStringList keys)
{
    control_keys_ = keys;
    ui->matrixDisplayLabel->setText(control_keys_.last());
    raise();
}
