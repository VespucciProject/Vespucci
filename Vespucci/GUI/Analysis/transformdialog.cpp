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
#include "transformdialog.h"
#include "ui_transformdialog.h"
#include "GUI/Processing/matrixselectiondialog.h"
#include <Math/Normalization/normalization.h>

TransformDialog::TransformDialog(QWidget *parent, QSharedPointer<VespucciWorkspace> ws, const QStringList keys) :
    QDialog(parent),
    ui(new Ui::TransformDialog)
{
    ui->setupUi(this);
    workspace_ = ws;
    data_keys_ = keys;
    ui->constantLineEdit->setValidator(new QDoubleValidator(-DBL_MAX, DBL_MAX, 5));
    matrix_selection_dialog_ = new MatrixSelectionDialog(this, workspace_->dataset_tree_model());
    connect(matrix_selection_dialog_, &MatrixSelectionDialog::MatrixSelected,
            this, &TransformDialog::MatrixSelected);
}

TransformDialog::~TransformDialog()
{
    delete ui;
    delete matrix_selection_dialog_;
}

void TransformDialog::MatrixSelected(QStringList keys)
{
    operand_keys_ = keys;
    ui->matrixNameLabel->setText(keys.last());
    raise();
}

void TransformDialog::on_selectPushButton_clicked()
{
    matrix_selection_dialog_->show();
}

void TransformDialog::on_buttonBox_accepted()
{
    QString operation = ui->operationComboBox->currentText();
    bool ok;
    double constant = ui->constantLineEdit->text().toDouble(&ok);
    if (!ok) constant = 0;
    mat matrix = workspace_->GetMatrix(data_keys_);
    mat operand = workspace_->GetMatrix(operand_keys_);
    mat new_matrix;
    QString name = ui->nameLineEdit->text();
    QSharedPointer<VespucciDataset> dataset = workspace_->GetDataset(data_keys_.first());

    try{
        if (operation == "Constant addition")
            new_matrix = matrix + constant * ones(matrix.n_rows, matrix.n_cols);
        else if (operation == "Constant subtraction")
            new_matrix = matrix + -1*constant*ones(matrix.n_rows, matrix.n_cols);
        else if (operation == "Constant division")
            new_matrix = (1.0/constant) * matrix;
        else if (operation == "Constant multiplication")
            new_matrix = constant * matrix;
        else if (operation == "Matrix addition")
           new_matrix = matrix + operand;
        else if (operation == "Matrix subtraction")
            new_matrix = matrix - operand;
        else if (operation == "Matrix multiplication (dot product)")
            new_matrix = matrix * operand;
        else if (operation == "Element-wise multiplication")
            new_matrix = matrix % operand;
        else if (operation == "Element-wise division")
            new_matrix = matrix / operand;
        else if (operation == "Minimum subtraction")
            new_matrix = matrix - matrix.min() * ones(matrix.n_rows, matrix.n_cols);
        else if (operation == "Maximum division")
            new_matrix = (1.0/matrix.max()) * matrix;
        else if (operation == "Min/Max normalization"){
            new_matrix = matrix;
            for (uword i = 0; i < new_matrix.n_cols; ++i){
                vec column = new_matrix.col(i);
                column = column - column.min() * ones(column.n_rows);
                column = column / column.max();
                new_matrix.col(i) = column;
            }
        }
        else if (operation == "SNV normalization")
            new_matrix = Vespucci::Math::Normalization::SNVNorm(matrix, 0, true);
        else if (operation == "Z-Score normalization")
            new_matrix = Vespucci::Math::Normalization::StandardScoreMat(matrix);
        else if (operation == "Normalize to 1-norm")
            new_matrix = normalise(matrix, 1);
        else if (operation == "Normalize to 2-norm")
            new_matrix = normalise(matrix, 2);
        else if (operation == "Calibration")
            ;//do nothing for now
        else
            return;
    }catch(...){
        QMessageBox::warning(this, "Cannot perform operation", "The operation could not be performed");
        close();
        return;
    }
    dataset->AddAuxiliaryMatrix(name, new_matrix);
    close();
}
