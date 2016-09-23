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
#include "datasetextractordialog.h"
#include "ui_datasetextractordialog.h"

DatasetExtractorDialog::DatasetExtractorDialog(QWidget *parent, QSharedPointer<VespucciWorkspace> ws, const QStringList &matrix_keys) :
    QDialog(parent),
    ui(new Ui::DatasetExtractorDialog)
{
    ui->setupUi(this);
    matrix_keys_ = matrix_keys;
    workspace_ = ws;
    ui->columnSpinBox->setMaximum(workspace_->GetMatrix(matrix_keys_).n_cols);
    ui->columnSpinBox->setMinimum(1);
    ui->valueDoubleSpinBox->setMinimum(workspace_->GetMatrix(matrix_keys_).min());
    ui->valueDoubleSpinBox->setMaximum(workspace_->GetMatrix(matrix_keys_).max());
    ui->nameLineEdit->setText(matrix_keys_.first() + " (" + matrix_keys_.last() + ")");
}

DatasetExtractorDialog::~DatasetExtractorDialog()
{
    delete ui;
}

void DatasetExtractorDialog::on_buttonBox_accepted()
{
    QString op = ui->operatorComboBox->currentText();
    QString name = ui->nameLineEdit->text();
    uword column =  ui->columnSpinBox->value() - 1;
    double value = ui->valueDoubleSpinBox->value();
    mat spectra;
    vec x, y, abscissa;
    uvec indices;
    vec matrix = workspace_->GetMatrix(matrix_keys_).col(column);
    try{
        QSharedPointer<VespucciDataset> dataset = workspace_->GetDataset(matrix_keys_.first());
        if (dataset.isNull()) throw runtime_error("Dataset not found");
        if (dataset->spectra_ref().n_cols != matrix.n_rows) throw invalid_argument("Dimension mismatch");
        if (op == "=") indices = arma::find(matrix == value);
        else if (op == ">") indices = arma::find(matrix > value);
        else if (op == "<") indices = arma::find(matrix < value);
        else if (op == "≠") indices = arma::find(matrix != value);
        if (indices.n_elem == 0) throw runtime_error("No values with specified relationship");
        spectra = dataset->spectra(indices);
        x = dataset->x_ref().rows(indices);
        y = dataset->y_ref().rows(indices);
        abscissa = dataset->abscissa();
        QSharedPointer<VespucciDataset> new_dataset(new VespucciDataset(name,
                                                                        workspace_->main_window(),
                                                                        workspace_->directory_ptr()));
        new_dataset->SetData(spectra, abscissa, x, y);
        workspace_->AddDataset(new_dataset);
    }catch(exception e){
        workspace_->main_window()->DisplayExceptionWarning(e);
    }
    close();
}
