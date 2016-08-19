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
#include "Data/Analysis/metaanalyzer.h"

MetaAnalysisDialog::MetaAnalysisDialog(QWidget *parent,
                                       QSharedPointer<VespucciWorkspace> ws,
                                       const QStringList &data_keys) :
    QDialog(parent),
    ui(new Ui::MetaAnalysisDialog)
{
    ui->setupUi(this);
    workspace_ = ws;
    data_keys_ = data_keys;
    ui->linkageComboBox->setVisible(false);
    ui->linkageLabel->setVisible(false);
    ui->metricComboBox->setVisible(false);
    ui->metricLabel->setVisible(false);
    ui->componentsLabel->setVisible(false);
    ui->componentsSpinBox->setVisible(false);
}

MetaAnalysisDialog::~MetaAnalysisDialog()
{
    delete ui;
}

void MetaAnalysisDialog::on_typeComboBox_currentTextChanged(const QString &arg1)
{
    if (arg1 == "Principal Component Analysis"){
        ui->linkageComboBox->setVisible(false);
        ui->linkageLabel->setVisible(false);
        ui->metricComboBox->setVisible(false);
        ui->metricLabel->setVisible(false);
        ui->componentsLabel->setVisible(false);
        ui->componentsSpinBox->setVisible(false);
    }
    else if (arg1 == "Vertex Component Analysis"){
        ui->linkageComboBox->setVisible(false);
        ui->linkageLabel->setVisible(false);
        ui->metricComboBox->setVisible(false);
        ui->metricLabel->setVisible(false);
        ui->componentsLabel->setVisible(true);
        ui->componentsSpinBox->setVisible(true);
    }
    else if (arg1 == "Partial Least Squares (Classification)"){
        ui->linkageComboBox->setVisible(false);
        ui->linkageLabel->setVisible(false);
        ui->metricComboBox->setVisible(false);
        ui->metricLabel->setVisible(false);
        ui->componentsLabel->setVisible(true);
        ui->componentsSpinBox->setVisible(true);
    }
    else if (arg1 == "k-Means Clustering"){
        ui->linkageComboBox->setVisible(false);
        ui->linkageLabel->setVisible(false);
        ui->metricComboBox->setVisible(true);
        ui->metricLabel->setVisible(true);
        ui->componentsLabel->setVisible(true);
        ui->componentsSpinBox->setVisible(true);
        QStringList metrics({"Squared Euclidean",
                             "Euclidean",
                             "Manhattan",
                             "Chebyshev"});
        ui->metricComboBox->clear();
        ui->metricComboBox->addItems(metrics);
    }
    else if (arg1 == "Hierarchical Clustering"){
        ui->linkageComboBox->setVisible(true);
        ui->linkageLabel->setVisible(true);
        ui->metricComboBox->setVisible(true);
        ui->metricLabel->setVisible(true);
        ui->componentsLabel->setVisible(false);
        ui->componentsSpinBox->setVisible(false);
        QStringList metrics({"Squared Euclidean",
                            "Euclidean",
                            "Manhattan",
                            "Chebyshev",
                            "Cosine",
                            "Correlation"});
        QStringList linkages({"Ward",
                             "Average",
                             "Centroid",
                             "Single",
                             "Complete"});
        ui->metricComboBox->clear();
        ui->metricComboBox->addItems(metrics);
        ui->linkageComboBox->clear();
        ui->linkageComboBox->addItems(linkages);
    }
    else{
        return;
    }
}

void MetaAnalysisDialog::on_buttonBox_accepted()
{
    QString type = ui->typeComboBox->currentText();
    QString name = ui->nameLineEdit->text();
    bool transpose = ui->transposeCheckBox->isChecked();
    MetaAnalyzer analyzer(workspace_, data_keys_, transpose);
    try{
        if (type == "Principal Component Analysis"){
            analyzer.PrincipalComponents(name);
        }
        else if (type == "Vertex Component Analysis"){
            uword components = ui->componentsSpinBox->value();
            analyzer.VertexComponents(name, components);
        }
        else if (type == "Partial Least Squares (Classification)"){
            uword components = ui->componentsSpinBox->value();
            analyzer.PartialLeastSquares(name, components);
        }
        else if (type == "k-Means Clustering"){
            uword components = ui->componentsSpinBox->value();
            QString metric = ui->metricComboBox->currentText();
            analyzer.KMeans(components, metric, name);
        }
        else if (type == "Hierarchical Clustering"){
            QString metric = ui->metricComboBox->currentText().toLower();
            QString linkage = ui->linkageComboBox->currentText().toLower();
            metric.remove("\\s");
            linkage.remove("\\s");
            analyzer.AgglomerativeClustering(name, metric, linkage);
        }
        else{
            close();
            return;
        }
    }catch (exception e){
        workspace_->main_window()->DisplayExceptionWarning(e);
        return;
    }
    close();
}

void MetaAnalysisDialog::on_buttonBox_rejected()
{
    close();
}
