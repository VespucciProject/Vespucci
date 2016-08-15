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
#include "univariateconcatenationdialog.h"
#include "ui_univariateconcatenationdialog.h"


UnivariateConcatenationDialog::UnivariateConcatenationDialog(QWidget *parent, QSharedPointer<VespucciWorkspace> ws, const QString &dataset_key) :
    QDialog(parent),
    ui(new Ui::UnivariateConcatenationDialog)
{
    ui->setupUi(this);
    workspace_ = ws;
    dataset_ = workspace_->GetDataset(dataset_key);
    QStringList results_keys = dataset_->AnalysisResultsKeys();
    QStringList univariate_keys;
    //Check type, if can't be cast, ignore
    for (auto results_key: results_keys){
        QSharedPointer<AnalysisResults> results = dataset_->GetAnalysisResult(results_key);
        QSharedPointer<UnivariateData> unidata = results.dynamicCast<UnivariateData>();
        if (!unidata.isNull()) univariate_keys << results_key;
    }
    univariate_keys.sort();
    ui->resultsListWidget->addItems(univariate_keys);
}

UnivariateConcatenationDialog::~UnivariateConcatenationDialog()
{
    delete ui;
}

void UnivariateConcatenationDialog::on_buttonBox_accepted()
{
    QStringList selected_keys;
    QList<QListWidgetItem*> items = ui->resultsListWidget->selectedItems();
    for (auto item: items) selected_keys << item->text();
    selected_keys.sort();
    QSharedPointer<AnalysisResults> first_results = dataset_->GetAnalysisResult(selected_keys.first());
    QSharedPointer<AnalysisResults> new_results = first_results->Replicate();
    new_results->SetName(ui->nameLineEdit->text());
    selected_keys.removeFirst();
    for (auto key: selected_keys){
        bool ok = new_results->Concatenate(dataset_->GetAnalysisResult(key));
        if (!ok){
            QMessageBox::warning(this, "Error", "Could not concatenate results");
            return;
        }
    }
    //for every dataset the first result is a member of, add the new result and
    //requisite subsets.
    QStringList subkeys({ "Peak Centers",
                          "Peak Intensities",
                          "Adjusted Peak Intensities",
                          "Total Area",
                          "Adjusted Area",
                          "Area Between Inflection Points",
                          "Adjusted Area Between Inflection Points",
                          "Empirical Full-Width at Half-Maximum"});
    QMap<QString, uvec> parent_rows = first_results->parent_rows();
    for (auto key: parent_rows.keys()){
        QSharedPointer<VespucciDataset> current_dataset = workspace_->GetDataset(key);
        if (!current_dataset.isNull()){
            current_dataset->AddAnalysisResult(new_results,
                                               parent_rows.value(key)(0),
                                               parent_rows.value(key)(1));
            try{
                current_dataset->AddAnalysisResult(new_results->Subset(subkeys,
                                                                   parent_rows.value(key)(0),
                                                                   parent_rows.value(key)(1)));
            }catch (...){
                //the subset does not appear if it is not valid
            }
        }
    }
    close();
}

void UnivariateConcatenationDialog::on_buttonBox_rejected()
{
    close();
}
