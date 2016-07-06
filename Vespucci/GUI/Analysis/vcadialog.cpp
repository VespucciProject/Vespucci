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
#include "GUI/Analysis/vcadialog.h"
#include "ui_vcadialog.h"

///
/// \brief VCADialog::VCADialog
/// \param parent See QDialog
/// \param ws Current workspace
/// \param row Currently selected row in dataset list widget
///
VCADialog::VCADialog(QWidget *parent, QSharedPointer<VespucciWorkspace> ws, const QString &dataset_key) :
    QDialog(parent),
    ui(new Ui::VCADialog)
{
    ui->setupUi(this);
    workspace_ = ws;
    dataset_ = workspace_->GetDataset(dataset_key);
    components_selector_spin_box_ = findChild<QSpinBox *>("endmembersSpinBox");
    prediction_check_box_ = findChild<QCheckBox *>("predictionCheckBox");
    name_line_edit_ = findChild<QLineEdit*>("nameLineEdit");
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
    QString name = name_line_edit_->text();
    if (prediction_check_box_->isChecked())
        endmembers = 0;
    else
        endmembers = components_selector_spin_box_->value();

    try{
        dataset_->VertexComponents(name, endmembers);
    }catch(std::exception e){
        workspace_->main_window()->DisplayExceptionWarning(e);
    }

    close();
    dataset_.clear();
}

///
/// \brief VCADialog::on_buttonBox_rejected
/// Closes window when "Cancel" clicked.
void VCADialog::on_buttonBox_rejected()
{
    close();
    dataset_.clear();
}

void VCADialog::on_predictionCheckBox_clicked(bool checked)
{
    components_selector_spin_box_->setEnabled(!checked);

}
