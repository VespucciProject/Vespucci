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
#include "GUI/Analysis/principalcomponentsdialog.h"
#include "ui_principalcomponentsdialog.h"
#include "Data/Analysis/multianalyzer.h"

///
/// \brief PrincipalComponentsDialog::PrincipalComponentsDialog
/// \param parent see QWidget
/// \param ws Current workspace
/// \param row Currently selected row
///
PrincipalComponentsDialog::PrincipalComponentsDialog(QWidget *parent, QSharedPointer<VespucciWorkspace> ws, const QString &dataset_key) :
    QDialog(parent),
    ui(new Ui::PrincipalComponentsDialog)
{
    ui->setupUi(this);
    workspace_ = ws;
    dataset_ = workspace_->GetDataset(dataset_key);
}

PrincipalComponentsDialog::PrincipalComponentsDialog(QSharedPointer<VespucciWorkspace> ws, const QStringList &dataset_keys)
    :QDialog(ws->main_window()),
    ui(new Ui::PrincipalComponentsDialog)
{
    workspace_ = ws;
    dataset_keys_ = dataset_keys;
    if (dataset_keys_.isEmpty()) close();

}

PrincipalComponentsDialog::~PrincipalComponentsDialog()
{
    delete ui;
}

///
/// \brief PrincipalComponentsDialog::on_buttonBox_accepted
/// Trigger appropriate method of dataset when user clicks "Ok"
void PrincipalComponentsDialog::on_buttonBox_accepted()
{
    QString name = ui->nameLineEdit->text();
    if (!dataset_keys_.isEmpty() || !dataset_.isNull()){
        if (!dataset_keys_.isEmpty()){
            try{
                MultiAnalyzer analyzer(workspace_, dataset_keys_);
                analyzer.PrincipalComponents(name);
            }catch(exception e){
                workspace_->main_window()->DisplayExceptionWarning(e);
            }
        }
        else{
            try{
                dataset_->PrincipalComponents(name);
            }catch(exception e){
                workspace_->main_window()->DisplayExceptionWarning(e);
            }
        }
    }
    close();
    dataset_.clear();
    return;
}

///
/// \brief PrincipalComponentsDialog::on_buttonBox_rejected
/// Close window when user selects "Cancel"
void PrincipalComponentsDialog::on_buttonBox_rejected()
{
    close();
    dataset_.clear();
}
