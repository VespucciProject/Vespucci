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
#include "GUI/Analysis/vcadialog.h"
#include "ui_vcadialog.h"
#include "Data/Analysis/multianalyzer.h"

///
/// \brief VCADialog::VCADialog
/// \param parent See QDialog
/// \param ws Current workspace
/// \param row Currently selected row in dataset list widget
///
VCADialog::VCADialog(QWidget *parent, QSharedPointer<VespucciWorkspace> ws, QSharedPointer<AbstractDataAnalyzer> analyzer) :
    QDialog(parent),
    ui(new Ui::VCADialog),
    workspace_(ws),
    analyzer_(analyzer)
{
    ui->setupUi(this);
}

VCADialog::~VCADialog()
{
    delete ui;
}

///
/// \brief VCADialog::on_buttonBox_accepted
/// Calls appropriate dataset method when "Ok" is clicked.
void VCADialog::on_buttonBox_accepted()
{
    int endmembers;
    QString name = ui->nameLineEdit->text();
    if (name.isEmpty()) name = "VCA";
    endmembers = ui->endmembersSpinBox->value();

    if (!analyzer_.isNull()){
        try{
            analyzer_->VertexComponents(name, endmembers);
        }catch(std::exception e){
            workspace_->main_window()->DisplayExceptionWarning(e);
        }
    }

    close();
    analyzer_.clear();
}

///
/// \brief VCADialog::on_buttonBox_rejected
/// Closes window when "Cancel" clicked.
void VCADialog::on_buttonBox_rejected()
{
    close();
    analyzer_.clear();
}
