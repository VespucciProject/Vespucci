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
#include "matrixselectiondialog.h"
#include "ui_matrixselectiondialog.h"

MatrixSelectionDialog::MatrixSelectionDialog(QWidget *parent, DatasetTreeModel *model) :
    QDialog(parent),
    ui(new Ui::MatrixSelectionDialog)
{
    ui->setupUi(this);
    tree_model_ = model;
    accepted_ = false;
    ui->treeView->setModel(tree_model_);
}

MatrixSelectionDialog::~MatrixSelectionDialog()
{
    delete ui;
}

TreeItem *MatrixSelectionDialog::GetSelectedItem()
{
    return selected_item_;
}

QStringList MatrixSelectionDialog::GetDataKeys()
{
    return data_keys_;
}

bool MatrixSelectionDialog::accepted()
{
    return accepted_;
}

void MatrixSelectionDialog::on_buttonBox_accepted()
{
    selected_item_ = tree_model_->getItem(ui->treeView->currentIndex());
    data_keys_ = selected_item_->keys();
    accepted_ = true;
}

void MatrixSelectionDialog::on_buttonBox_rejected()
{
    accepted_ = false;
    close();
}
