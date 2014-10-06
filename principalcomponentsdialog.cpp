/************************************************************************************
    Copyright (C) 2014 Daniel P. Foose - All Rights Reserved

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
***************************************************************************************/
#include "principalcomponentsdialog.h"
#include "ui_principalcomponentsdialog.h"

///
/// \brief PrincipalComponentsDialog::PrincipalComponentsDialog
/// \param parent see QWidget
/// \param ws Current workspace
/// \param row Currently selected row
///
PrincipalComponentsDialog::PrincipalComponentsDialog(QWidget *parent, VespucciWorkspace *ws, int row) :
    QDialog(parent),
    ui(new Ui::PrincipalComponentsDialog)
{
    ui->setupUi(this);
    workspace = ws;
    data_ = workspace->DatasetAt(row);
    if(data_->non_spatial()){
        QMessageBox::warning(this,
                             "Non-spatial or Non-contiguous Dataset",
                             "Images cannot be created from non-spatial or "
                             "non-contiguous datasets.");
        this->close();
    }
    component_selector_ = this->findChild<QSpinBox *>("componentSpinBox");
    color_selector_ = this->findChild<QComboBox *>("gradientComboBox");
    recalculate_box_ = this->findChild<QCheckBox *>("recalculateCheckBox");
    name_box_ = this->findChild<QLineEdit*>("nameLineEdit");
    data_index_ = row;
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
    int component = component_selector_->value();
    QString name = name_box_->text();
    bool recalculate = recalculate_box_->isChecked();
    int gradient_index = color_selector_->currentIndex();
    data_->PrincipalComponents(component, name, gradient_index, recalculate);
    this->close();
}

///
/// \brief PrincipalComponentsDialog::on_buttonBox_rejected
/// Close window when user selects "Cancel"
void PrincipalComponentsDialog::on_buttonBox_rejected()
{
    this->close();
}
