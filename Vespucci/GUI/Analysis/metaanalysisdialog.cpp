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
#include "metaanalysisdialog.h"
#include "ui_metaanalysisdialog.h"
#include "Data/Analysis/matrixanalyzer.h"
#include "GUI/Analysis/bandratiodialog.h"
#include "GUI/Analysis/kmeansdialog.h"
#include "GUI/Analysis/plsdialog.h"
#include "GUI/Analysis/principalcomponentsdialog.h"
#include "GUI/Analysis/univariatedialog.h"
#include "GUI/Analysis/vcadialog.h"
#include "GUI/Analysis/ahcadialog.h"

MetaAnalysisDialog::MetaAnalysisDialog(QWidget *parent,
                                       QSharedPointer<VespucciWorkspace> ws,
                                       const QStringList &data_keys) :
    QDialog(parent),
    ui(new Ui::MetaAnalysisDialog),
    workspace_(ws),
    data_keys_(data_keys)
{
    ui->setupUi(this);
}

MetaAnalysisDialog::~MetaAnalysisDialog()
{
    delete ui;
    delete matrix_selection_dialog_;//should be deleted anyway?
}

void MetaAnalysisDialog::on_buttonBox_accepted()
{
    QString analysis_description = ui->typeComboBox->currentText();
    bool transpose = ui->transposeCheckBox->isChecked();
    QSharedPointer<MatrixAnalyzer> analyzer(new MatrixAnalyzer(workspace_, data_keys_, transpose));

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
    }catch (exception e){
        workspace_->main_window()->DisplayExceptionWarning(e);
    }

    close();
}

void MetaAnalysisDialog::on_buttonBox_rejected()
{
    close();
}
