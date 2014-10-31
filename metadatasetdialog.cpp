/*******************************************************************************
    Copyright (C) 2014 Wright State University - All Rights Reserved
    Daniel P. Foose - Author

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
#include "metadataset.h"
MetaDatasetDialog::MetaDatasetDialog(QWidget *parent, VespucciWorkspace *ws) :
    QDialog(parent),
    ui(new Ui::MetaDatasetDialog)
{
    ui->setupUi(this);

    dataset_list_view_ = findChild<QListView*>("datasetListView");
    workspace = ws;
    dataset_list_model_ = ws->dataset_list_model();
    dataset_list_view_->setModel(dataset_list_model_);
    dataset_list_view_->setSelectionMode(QAbstractItemView::MultiSelection);
    method_selection_box_ = findChild<QComboBox *>("methodComboBox");
    name_line_edit_ = findChild<QLineEdit *>("nameLineEdit");
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
    QList<QSharedPointer<VespucciDataset> > parent_datasets;
    QModelIndexList selected_indices = dataset_list_view_->selectionModel()->selectedRows();
    for (int i = 0; i < selected_indices.size(); ++i){
        parent_datasets.append(dataset_list_model_->DatasetAt(selected_indices[i].row()));
    }

    QString method_description = method_selection_box_->currentText();
    VespucciMetaMethod method;
    switch(method_selection_box_->currentIndex()){
    case 0:
        method = ConcatenateDatasets;
    case 1: default:
        method = AverageSpectra;
    }

    QString name = name_line_edit_->text();
    QFile *log_file = workspace->CreateLogFile(name);

    QSharedPointer<MetaDataset> new_dataset(new MetaDataset(name, workspace->main_window(), log_file, workspace->directory_ptr(), method_description, method, parent_datasets));
    workspace->AddDataset(new_dataset);
}
