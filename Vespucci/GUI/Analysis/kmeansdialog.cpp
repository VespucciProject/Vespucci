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
KMeansDialog::KMeansDialog(QWidget *parent, QSharedPointer<VespucciWorkspace> ws, QSharedPointer<AbstractDataAnalyzer> analyzer) :
    QDialog(parent),
    ui(new Ui::KMeansDialog),
    workspace_(ws),
    analyzer_(analyzer)
{
    ui->setupUi(this);
    workspace_ = ws;
    analyzer_ = analyzer;
    ui->clustersSpinBox->setRange(0, analyzer_->columns());
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
    QString name = ui->nameLineEdit->text();
    QString metric_text = ui->metricComboBox->currentText();
    size_t clusters = ui->clustersSpinBox->value();
    try{
        analyzer_->KMeans(name, metric_text, clusters);
    }catch(exception e){
        workspace_->main_window()->DisplayExceptionWarning(e);
    }
    analyzer_.clear();
    close();
}

///
/// \brief KMeansDialog::on_buttonBox_rejected
/// Closes window when "Cancel" selected.
void KMeansDialog::on_buttonBox_rejected()
{
    close();
    analyzer_.clear();
}

void KMeansDialog::on_predictionCheckBox_clicked(bool checked)
{
    ui->clustersSpinBox->setEnabled(!checked);
}
