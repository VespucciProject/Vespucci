/*******************************************************************************
    Copyright (C) 2014-2017 Wright State University - All Rights Reserved
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
#include "dimensionalityestimationdialog.h"
#include "ui_dimensionalityestimationdialog.h"
#include <Math/DimensionReduction/dimensionreduction.h>

DimensionalityEstimationDialog::DimensionalityEstimationDialog(QWidget *parent, QSharedPointer<VespucciWorkspace> workspace, const QStringList &matrix_keys) :
    QDialog(parent),
    ui(new Ui::DimensionalityEstimationDialog)
{
    ui->setupUi(this);
    matrix_keys_ = matrix_keys;
    workspace_ = workspace;
}

DimensionalityEstimationDialog::~DimensionalityEstimationDialog()
{
    delete ui;
}

void DimensionalityEstimationDialog::on_buttonBox_accepted()
{
    size_t rank;
    arma::mat subspace;
    try{
        arma::mat matrix = workspace_->GetMatrix(matrix_keys_);
        QSharedPointer<VespucciDataset> parent_dataset = workspace_->GetDataset(matrix_keys_.first());
        if (ui->methodComboBox->currentText() == "SVD")
            rank = arma::rank(matrix);
        else if (ui->methodComboBox->currentText() == "HySime"){
            rank = Vespucci::Math::DimensionReduction::HySime(matrix, subspace);
            parent_dataset->AddAuxiliaryMatrix(matrix_keys_.last() + " HySime subspace", subspace);
        }
        else{
            workspace_->main_window()->DisplayWarning("Error", "Invalid type!");
            return;
        }
        arma::vec rank_mat({double(rank)});
        parent_dataset->AddAuxiliaryMatrix(matrix_keys_.last() + " Estimated Rank", rank_mat);
    }catch(std::exception e){
        workspace_->main_window()->DisplayExceptionWarning(e);
    }
    close();
}
