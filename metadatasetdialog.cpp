/*******************************************************************************
    Copyright (C) 2014 Daniel P. Foose - All Rights Reserved

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

#include "metadatasetdialog.h"
#include "ui_metadatasetdialog.h"

MetaDatasetDialog::MetaDatasetDialog(QWidget *parent, VespucciWorkspace *ws) :
    QDialog(parent),
    ui(new Ui::MetaDatasetDialog)
{
    ui->setupUi(this);

    dataset_list_widget_ = this->findChild<QListWidget*>("datasetListWidget");
    workspace = ws;
    dataset_list_widget_->addItems(workspace->dataset_names());
    method_selection_box_ = this->findChild<QComboBox *>("methodComboBox");
    endmember_selection_box_ = this->findChild<QLineEdit *>("componentLineEdit");
}

MetaDatasetDialog::~MetaDatasetDialog()
{
    delete ui;
}

void MetaDatasetDialog::on_buttonBox_rejected()
{
    this->close();
}

void MetaDatasetDialog::on_buttonBox_accepted()
{
    QList<QListWidgetItem *> widget_item_list = dataset_list_widget_->selectedItems();
    QList<unsigned int> rows;
    for (int i = 0; i < widget_item_list.size(); ++i){
    }
}
