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
#include "GUI/Analysis/analysisdialog.h"
#include "GUI/Analysis/ui_analysisdialog.h"

AnalysisDialog::AnalysisDialog(QWidget *parent, VespucciWorkspace *ws, int row) :
    QDialog(parent),
    ui(new Ui::AnalysisDialog)
{
    ui->setupUi(this);
    method_selection_box_ = findChild<QComboBox*>("methodComboBox");
    components_box_ = findChild<QSpinBox*>("componentsSpinBox");
    prediction_box_ = this->findChild<QCheckBox *>("predictionCheckBox");
    dataset_ = ws->DatasetAt(row);
    workspace = ws;
}

AnalysisDialog::~AnalysisDialog()
{
    delete ui;
}

void AnalysisDialog::on_comboBox_currentIndexChanged(int index)
{
    if(index==0 && components_box_->isEnabled())
        components_box_->setEnabled(false);
    else if(index!=0 && !components_box_->isEnabled())
        components_box_->setEnabled(true);
}

void AnalysisDialog::on_buttonBox_rejected()
{
    dataset_.clear();
    close();
}

void AnalysisDialog::on_buttonBox_accepted()
{
    int components;
    if (prediction_box_->isChecked())
        components = 0;
    else
        components = components_box_->value();

    switch(method_selection_box_->currentIndex()){
    case 0:
        try{
            dataset_->PrincipalComponents();
        }catch(exception e){
            workspace->main_window()->DisplayExceptionWarning(e);
        }

        break;
    case 1:
        try{
            dataset_->PrincipalComponents(true);
        }catch(exception e){
            workspace->main_window()->DisplayExceptionWarning(e);
        }

        break;
    case 2:
        try{
            dataset_->PartialLeastSquares(components);
        }catch(exception e){
            workspace->main_window()->DisplayExceptionWarning(e);
        }

        break;
    case 3:
        try{
            dataset_->VertexComponents(components);
        }catch(exception e){
            workspace->main_window()->DisplayExceptionWarning(e);
        }

        break;
    case 4:
        try{
            dataset_->KMeans(components);
        }catch(exception e){
            workspace->main_window()->DisplayExceptionWarning(e);
        }

        break;
    case 5:
        try{
            dataset_->FindPeaks(nan(""), std::nan(""), 5, 7);
        }catch(exception e){
            workspace->main_window()->DisplayExceptionWarning(e);
        }
    default:
        break;
    }
    dataset_.clear();
    close();
}

void AnalysisDialog::on_predictionCheckBox_clicked(bool checked)
{
    components_box_->setEnabled(!checked);
}
