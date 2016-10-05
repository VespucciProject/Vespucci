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
#include "representativespectrumdialog.h"
#include "ui_representativespectrumdialog.h"

RepresentativeSpectrumDialog::RepresentativeSpectrumDialog(QWidget *parent, QSharedPointer<VespucciWorkspace> ws, const QString &dataset_key) :
    QDialog(parent),
    ui(new Ui::RepresentativeSpectrumDialog),
    workspace_(ws)
{
    ui->setupUi(this);
    dataset_ = ws->GetDataset(dataset_key);
}

RepresentativeSpectrumDialog::~RepresentativeSpectrumDialog()
{
    delete ui;
}

void RepresentativeSpectrumDialog::on_buttonBox_accepted()
{
    if (!dataset_.isNull()){
        QString name = ui->nameLineEdit->text();
        QString metric = ui->metricComboBox->currentText().toLower();
        metric.remove(" ");
        QString statistic = ui->statisticComboBox->currentText().toLower();
        try{
            dataset_->CalculateRepresentativeSpectrum(name, statistic, metric);
        }catch(exception e){
            workspace_->main_window()->DisplayExceptionWarning("VespucciDataset::CalculateRepresentativeSpectrum", e);
        }
    }
}
