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
#include "mapdialog.h"
#include "ui_mapdialog.h"

MapDialog::MapDialog(MainWindow *parent, QStringList data_keys, QSharedPointer<VespucciWorkspace> ws) :
    QDialog(parent),
    ui(new Ui::MapDialog)
{
    ui->setupUi(this);

    if (data_keys.size() > 3 || data_keys.size() < 2){
        main_window_->DisplayWarning("Cannot map this object", "Can't map object");
        close();
    }
    main_window_ = parent;
    data_keys_ = data_keys;
    workspace_ = ws;
    uword col_count;
    try{
        col_count = workspace_->GetMatrix(data_keys_).n_cols;
    }catch(exception e){
        main_window_->DisplayExceptionWarning("MapDialog constructor, VespucciWorkspace::GetMatrix", e);
        close();
    }

    try{
        dataset_ = workspace_->data_model()->GetDataset(data_keys_[0]);
    }catch(exception e){
        main_window_->DisplayExceptionWarning("MapDialog constructor, DataModel::GetDataset", e);
        close();
    }

    ui->columnSpinBox->setRange(1, col_count);
    ui->nameLineEdit->setText(data_keys_.last());
    QStringList color_list = workspace_->GradientNames();
    ui->gradientComboBox->addItems(color_list);
}

MapDialog::~MapDialog()
{
    delete ui;
}

void MapDialog::on_buttonBox_accepted()
{
    uint gradient_index = ui->gradientComboBox->currentIndex();
    uint column_index = ui->columnSpinBox->value() - 1;
    QString map_name = ui->nameLineEdit->text();
    vec data;
    try{
        data = workspace_->data_model()->GetMatrix(data_keys_).col(column_index);
    }catch(exception e){
        main_window_->DisplayExceptionWarning("DataModel::GetMatrix or mat::col", e);
        close();
    }

    QCPColorGradient gradient;
    uint tick_count;

    if (ui->gradientComboBox->currentText() == "ColorBrewer Cluster"){
        vec unique_values = unique(data);
        tick_count = unique_values.n_rows;
        gradient = dataset_->GetClusterGradient(tick_count);
    }
    else{
        tick_count = 6;
        gradient = dataset_->GetGradient(gradient_index);
    }

    try{
        if (data_keys_.size() == 2)
            dataset_->CreateMap(map_name,
                                data_keys_[1],
                                column_index,
                                gradient,
                                tick_count);
        else
            dataset_->CreateMap(map_name,
                                data_keys_[1],
                                data_keys_[2],
                                column_index,
                                gradient,
                                tick_count);
    }catch(exception e){
        main_window_->DisplayExceptionWarning("VespucciDataset::CreateMap()", e);
        close();
    }
    close();
}
