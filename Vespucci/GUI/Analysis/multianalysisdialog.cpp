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
#include "multianalysisdialog.h"
#include "ui_multianalysisdialog.h"
#include "GUI/Analysis/bandratiodialog.h"
#include "GUI/Analysis/kmeansdialog.h"
#include "GUI/Analysis/plsdialog.h"
#include "GUI/Analysis/principalcomponentsdialog.h"
#include "GUI/Analysis/univariatedialog.h"
#include "GUI/Analysis/vcadialog.h"
#include "GUI/Analysis/ahcadialog.h"

MultiAnalysisDialog::MultiAnalysisDialog(QWidget *parent, QSharedPointer<VespucciWorkspace> ws) :
    QDialog(parent),
    ui(new Ui::MultiAnalysisDialog)
{
    ui->setupUi(this);
    workspace_ = ws;
    ui->datasetListWidget->addItems(workspace_->dataset_names());
}

MultiAnalysisDialog::~MultiAnalysisDialog()
{
    delete ui;
}

void MultiAnalysisDialog::on_buttonBox_accepted()
{
    QList<QListWidgetItem*> selected_items = ui->datasetListWidget->selectedItems();
    QStringList dataset_keys;
    for (auto item: selected_items) dataset_keys.append(item->text());
    QString analysis_description = ui->typeComboBox->currentText();
    if (analysis_description == "Univariate Analysis"){
        UnivariateDialog *dialog = new UnivariateDialog(workspace_, dataset_keys);
        dialog->setAttribute(Qt::WA_DeleteOnClose);
        dialog->show();
    }
    else if (analysis_description == "Band Ratio Analysis"){
        BandRatioDialog *dialog = new BandRatioDialog(workspace_, dataset_keys);
        dialog->setAttribute(Qt::WA_DeleteOnClose);
        dialog->show();
    }
    else if (analysis_description == "Principal Component Analysis"){
        PrincipalComponentsDialog *dialog = new PrincipalComponentsDialog(workspace_, dataset_keys);
        dialog->setAttribute(Qt::WA_DeleteOnClose);
        dialog->show();
    }
    else if (analysis_description == "Principal Component Analysis (mlpack)"){
        PrincipalComponentsDialog *dialog = new PrincipalComponentsDialog(workspace_, dataset_keys);
        dialog->setAttribute(Qt::WA_DeleteOnClose);
        dialog->show();
    }
    else if (analysis_description == "Vertex Component Analysis"){
        VCADialog *dialog = new VCADialog(workspace_, dataset_keys);
        dialog->setAttribute(Qt::WA_DeleteOnClose);
        dialog->show();
    }
    else if (analysis_description == "Partial Least Squares (Classification)"){
        PLSDialog *dialog = new PLSDialog(workspace_, dataset_keys);
        dialog->setAttribute(Qt::WA_DeleteOnClose);
        dialog->show();
    }
    else if (analysis_description == "k-Means Clustering"){
        KMeansDialog *dialog = new KMeansDialog(workspace_, dataset_keys);
        dialog->setAttribute(Qt::WA_DeleteOnClose);
        dialog->show();
    }
    else if (analysis_description == "Hierarchical Clustering"){
        AHCADialog *dialog = new AHCADialog(workspace_, dataset_keys);
        dialog->setAttribute(Qt::WA_DeleteOnClose);
        dialog->show();
    }
    else{
        close();
        return;
    }
    close();
    return;

}
