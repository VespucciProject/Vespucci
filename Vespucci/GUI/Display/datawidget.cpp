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
#include "datawidget.h"
#include "ui_datawidget.h"

DataWidget::DataWidget(QWidget *parent, VespucciTableModel *table_model) :
    QWidget(parent),
    ui(new Ui::DataWidget)
{
    ui->setupUi(this);
    table_model_ = table_model;
    ui->tableView->setModel(table_model);
    matrix_columns_=table_model->MatrixColumns();
    subviews_ = std::ceil(matrix_columns_/15);
    current_start_column_ = 1; //user sees indexing start at 1

    ui->backPushButton->setEnabled(false);
    ui->forwardPushButton->setEnabled(subviews_ > 1);
    uint last_column = (matrix_columns_ > 15 ? 15 : matrix_columns_);
    QString label = "Showing columns 1–" + QString::number(last_column);
    ui->columnLabel->setText(label);
}

void DataWidget::SetTableModel(VespucciTableModel *table_model)
{
    ui->tableView->setModel(table_model);
    table_model_ = table_model;
}

DataWidget::~DataWidget()
{
    delete ui;
}

void DataWidget::on_forwardPushButton_clicked()
{
    //user sees indexing start at one, table model indexing starts at 0
    uint next_start_column = current_start_column_ + 15;
    uint next_end_column = next_start_column + 15;
    next_end_column = (next_end_column < matrix_columns_ ? next_end_column : matrix_columns_);
    ui->backPushButton->setEnabled(true);
    ui->forwardPushButton->setEnabled(next_end_column - 1 < matrix_columns_);
    current_start_column_ = next_start_column;
    table_model_ = new VespucciTableModel(ui->tableView, table_model_->GetMatrix(), next_start_column-1);
    ui->tableView->setModel(table_model_);
    QString label = "Showing columns " + QString::number(current_start_column_)
            + "–" + QString::number(next_end_column);
    ui->columnLabel->setText(label);
}

void DataWidget::on_backPushButton_clicked()
{
    uint next_start_column = current_start_column_ - 15;
    //since it's unsigned, next_start_column evaluates false if current_start_column_
    //is less than or equal to 15, which means we're at the first view, which
    //means we aren't able to go back any more views
    ui->backPushButton->setEnabled(next_start_column);
    ui->forwardPushButton->setEnabled(true);//we can always move forward from the
    //previous page, because in order for the back button to even be active,
    //a viewable next page must be availible
    current_start_column_ = next_start_column;
    uint next_end_column = current_start_column_ - 15;
    table_model_ = new VespucciTableModel(ui->tableView, table_model_->GetMatrix(), next_start_column-1);
    ui->tableView->setModel(table_model_);
    QString label = "Showing columns " + QString::number(current_start_column_)
            + "–" + QString::number(next_end_column);
    ui->columnLabel->setText(label);
}
