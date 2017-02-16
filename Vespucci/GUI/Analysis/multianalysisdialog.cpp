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
#include "multianalysisdialog.h"
#include "ui_multianalysisdialog.h"
#include "GUI/Analysis/bandratiodialog.h"
#include "GUI/Analysis/kmeansdialog.h"
#include "GUI/Analysis/plsdialog.h"
#include "GUI/Analysis/principalcomponentsdialog.h"
#include "GUI/Analysis/univariatedialog.h"
#include "GUI/Analysis/vcadialog.h"
#include "GUI/Analysis/ahcadialog.h"
#include "Data/Analysis/multianalyzer.h"

MultiAnalysisDialog::MultiAnalysisDialog(QWidget *parent, QSharedPointer<VespucciWorkspace> ws) :
    QDialog(parent),
    ui(new Ui::MultiAnalysisDialog),
    workspace_(ws)
{
    ui->setupUi(this);
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
    QSharedPointer<MultiAnalyzer> analyzer(new MultiAnalyzer(workspace_, dataset_keys));

    try{
        if (analysis_description == "Univariate Analysis"){
            UnivariateDialog *dialog = new UnivariateDialog(this, workspace_, analyzer);
            dialog->setAttribute(Qt::WA_DeleteOnClose);
            dialog->show();
        }
        else if (analysis_description == "Band Ratio Analysis"){
            BandRatioDialog *dialog = new BandRatioDialog(this, workspace_, analyzer);
            dialog->setAttribute(Qt::WA_DeleteOnClose);
            dialog->show();
        }
        else if (analysis_description == "Principal Component Analysis"){
            PrincipalComponentsDialog *dialog = new PrincipalComponentsDialog(this, workspace_, analyzer);
            dialog->setAttribute(Qt::WA_DeleteOnClose);
            dialog->show();
        }
        else if (analysis_description == "Principal Component Analysis (mlpack)"){
            PrincipalComponentsDialog *dialog = new PrincipalComponentsDialog(this, workspace_, analyzer);
            dialog->setAttribute(Qt::WA_DeleteOnClose);
            dialog->show();
        }
        else if (analysis_description == "Vertex Component Analysis"){
            VCADialog *dialog = new VCADialog(this, workspace_, analyzer);
            dialog->setAttribute(Qt::WA_DeleteOnClose);
            dialog->show();
        }
        else if (analysis_description == "Partial Least Squares (Classification)"){
            PLSDialog *dialog = new PLSDialog(this, workspace_, analyzer);
            dialog->setAttribute(Qt::WA_DeleteOnClose);
            dialog->show();
        }
        else if (analysis_description == "k-Means Clustering"){
            KMeansDialog *dialog = new KMeansDialog(this, workspace_, analyzer);
            dialog->setAttribute(Qt::WA_DeleteOnClose);
            dialog->show();
        }
        else if (analysis_description == "Hierarchical Clustering"){
            AHCADialog *dialog = new AHCADialog(this, workspace_, analyzer);
            dialog->setAttribute(Qt::WA_DeleteOnClose);
            dialog->show();
        }
        else if (analysis_description == "SNV Normalization"){
            bool ok;
            double offset = QInputDialog::getDouble(this, "Enter Offset", "Offset",
                                              0, -2147483647, 2147483647, 4, &ok);

            if (ok){
                analyzer->SNVNormalize(offset);
            }
        }
        else if (analysis_description == "QUIC-SVD Denoise"){
            bool ok;
            double epsilon = QInputDialog::getDouble(this, "Enter Error Tolerance Fraction",
                                                     "Fraction", 0.03, 0.00001, 0.99999, 4, &ok);
            if (ok){
                analyzer->QUIC_SVD(epsilon);
            }
        }
    }catch(exception e){
        workspace_->main_window()->DisplayExceptionWarning(e);
    }
}
