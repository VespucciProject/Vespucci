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
#include "GUI/Analysis/kmeansdialog.h"
#include "ui_kmeansdialog.h"
#include "Data/Analysis/multianalyzer.h"

///
/// \brief KMeansDialog::KMeansDialog
/// \param parent Parent QWidget
/// \param ws Current workspace
/// \param row Row of current dataset
///
KMeansDialog::KMeansDialog(QWidget *parent, QSharedPointer<VespucciWorkspace> ws, const QString &dataset_key) :
    QDialog(parent),
    ui(new Ui::KMeansDialog)
{
    ui->setupUi(this);
    workspace_ = ws;
    dataset_ = workspace_->GetDataset(dataset_key);
    ui->clustersSpinBox->setRange(0, dataset_->spectra_ptr()->n_cols);
}

KMeansDialog::KMeansDialog(QSharedPointer<VespucciWorkspace> ws, const QStringList &dataset_keys)
    :QDialog(ws->main_window()),
      ui(new Ui::KMeansDialog)
{
    ui->setupUi(this);
    dataset_keys_ = dataset_keys;
    if (dataset_keys_.isEmpty()){
        close();
        return;
    }
    ui->predictionCheckBox->setEnabled(false);

    workspace_ = ws;
    dataset_ = workspace_->GetDataset(dataset_keys_.first());
}

KMeansDialog::~KMeansDialog()
{
    delete ui;
}

///
/// \brief KMeansDialog::on_buttonBox_accepted
/// Triggers K-means method of dataset when "Ok" selected
void KMeansDialog::on_buttonBox_accepted()
{

    if (dataset_keys_.isEmpty() && dataset_.isNull()){
        close();
        return;
    }
    QString metric_text = ui->metricComboBox->currentText();
    int clusters;
    QString name = ui->nameLineEdit->text();
    if (ui->predictionCheckBox->isChecked())
        clusters = 0;
    else
        clusters = ui->clustersSpinBox->value();
    if (!dataset_keys_.isEmpty()){
        try{
            MultiAnalyzer analyzer(workspace_, dataset_keys_);
            analyzer.KMeans(clusters, metric_text, name);
        }catch(exception e){
            workspace_->main_window()->DisplayExceptionWarning(e);
            dataset_.clear();
            close();
            return;
        }
        dataset_.clear();
        close();
        return;
    }
    try{
        dataset_->KMeans(name, clusters, metric_text);
    }catch(exception e){
        workspace_->main_window()->DisplayExceptionWarning(e);
    }

    dataset_.clear();
    close();
}

///
/// \brief KMeansDialog::on_buttonBox_rejected
/// Closes window when "Cancel" selected.
void KMeansDialog::on_buttonBox_rejected()
{
    close();
    dataset_.clear();
}

void KMeansDialog::on_predictionCheckBox_clicked(bool checked)
{
    ui->clustersSpinBox->setEnabled(!checked);
}
