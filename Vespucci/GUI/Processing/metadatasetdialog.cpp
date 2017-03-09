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
#include "GUI/Processing/metadatasetdialog.h"
#include "ui_metadatasetdialog.h"
#include "Data/Dataset/metadataset.h"
MetaDatasetDialog::MetaDatasetDialog(QWidget *parent, QSharedPointer<VespucciWorkspace> ws) :
    QDialog(parent),
    ui(new Ui::MetaDatasetDialog)
{
    ui->setupUi(this);
    workspace_ = ws;
    dataset_list_model_ = new DatasetListModel(this, workspace_->dataset_names());
    ui->datasetListView->setModel(dataset_list_model_);
    ui->datasetListView->setSelectionMode(QAbstractItemView::MultiSelection);
}

MetaDatasetDialog::~MetaDatasetDialog()
{
    delete ui;
}

void MetaDatasetDialog::on_buttonBox_rejected()
{
    close();
}

void MetaDatasetDialog::on_buttonBox_accepted()
{
    QList<QSharedPointer<VespucciDataset> > parent_datasets;
    QModelIndexList selected_indices = ui->datasetListView->selectionModel()->selectedRows();
    if (selected_indices.size() <= 0){
        return;
    }
    for (int i = 0; i < selected_indices.size(); ++i){
        parent_datasets.append(workspace_->GetDataset(dataset_list_model_->DatasetName(selected_indices[i].row())));
    }

    QString method_description = ui->methodComboBox->currentText();
    MetaMethod::Method method;
    switch(ui->methodComboBox->currentIndex()){
    case 0:
        cout << method_description.toStdString() << "\n";

        cout << "0\n";

        method = MetaMethod::ConcatenateDatasets;
        break;
    case 1: default:
        cout << method_description.toStdString() << "\n";

        cout << "1 or default\n";

        method = MetaMethod::AverageSpectra;
        break;
    }

    QString name = ui->nameLineEdit->text();
    QSharedPointer<MetaDataset> new_dataset;
    try{
        new_dataset = QSharedPointer<MetaDataset>(new MetaDataset(name, workspace_->main_window(), workspace_->directory_ptr(), method_description, method, parent_datasets));
    }
    catch(exception e){
        cerr << "Exception thrown\n";

        cerr << e.what() << "\n";

        QMessageBox::warning(this, "Exception Occured", "An exception was thrown in the MetaDataset constructor");
        return;
    }
    workspace_->AddDataset(new_dataset);

}
