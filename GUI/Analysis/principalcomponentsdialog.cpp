/*******************************************************************************
    Copyright (C) 2015 Wright State University - All Rights Reserved
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
#include "GUI/Analysis/principalcomponentsdialog.h"
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
    component_selector_ = findChild<QSpinBox *>("componentSpinBox");
    color_selector_ = findChild<QComboBox *>("gradientComboBox");
    recalculate_box_ = findChild<QCheckBox *>("recalculateCheckBox");
    name_box_ = findChild<QLineEdit*>("nameLineEdit");
    map_check_box_ = findChild<QCheckBox*>("mapCheckBox");
    data_index_ = row;

    component_selector_->setEnabled(false);
    color_selector_->setEnabled(false);
    recalculate_box_->setEnabled(false);
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

    QProgressDialog progress(this);
    progress.setWindowModality(Qt::WindowModal);
    progress.setLabelText("Interpolating Spectra...");
    progress.setCancelButton(0);
    progress.setRange(0,0);
    progress.exec();
    if (map_check_box_->isChecked()){
        int component = component_selector_->value();
        QString name = name_box_->text();
        bool recalculate = recalculate_box_->isChecked();
        int gradient_index = color_selector_->currentIndex();
        try{
            data_->PrincipalComponents(component, name, gradient_index, recalculate);
        }
        catch(exception e){
            progress.close();
            workspace->main_window()->DisplayExceptionWarning(e);
        }
    }
    else{
        try{
            data_->PrincipalComponents();
        }catch(exception e){
            progress.close();
            workspace->main_window()->DisplayExceptionWarning(e);
        }
    }
    progress.close();
    close();
    data_.clear();
}

///
/// \brief PrincipalComponentsDialog::on_buttonBox_rejected
/// Close window when user selects "Cancel"
void PrincipalComponentsDialog::on_buttonBox_rejected()
{
    close();
    data_.clear();
}

void PrincipalComponentsDialog::on_mapCheckBox_stateChanged(int arg1)
{
    component_selector_->setEnabled(arg1);
    color_selector_->setEnabled(arg1);
    recalculate_box_->setEnabled(arg1);
}
