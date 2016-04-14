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
#include "GUI/Analysis/plsdialog.h"
#include "ui_plsdialog.h"

PLSDialog::PLSDialog(QWidget *parent, VespucciWorkspace *ws, const QString &dataset_key) :
    QDialog(parent),
    ui(new Ui::PLSDialog)
{
    ui->setupUi(this);
    workspace = ws;
    dataset_ = workspace->GetDataset(dataset_key);
    components_spin_box_ = findChild<QSpinBox *>("componentsSpinBox");
    recalculate_check_box_ = findChild<QCheckBox *>("recalculateCheckBox");
    prediction_check_box_ = findChild<QCheckBox *>("predictionCheckBox");
    name_line_edit_ = findChild<QLineEdit*>("mapNameLineEdit");
    map_check_box_ = findChild<QCheckBox *>("mapCheckBox");

    name_line_edit_->setEnabled(false);
    recalculate_check_box_->setEnabled(false);
    color_selector_combo_box_->setEnabled(false);
    image_component_selector_spin_box_->setEnabled(false);



    browse_push_button_ = findChild<QPushButton*>("browsePushButton");
    dataset_label_ = findChild<QLabel*>("datasetLabel");
    file_label_ = findChild<QLabel*>("fileLabel");
    file_line_edit_ = findChild<QLineEdit *>("fileLineEdit");
    dataset_combo_box_ = findChild<QComboBox*>("datasetComboBox");

    browse_push_button_->setVisible(false);
    dataset_label_->setVisible(false);
    file_label_->setVisible(false);
    file_line_edit_->setVisible(false);
    dataset_combo_box_->setVisible(false);

}

PLSDialog::~PLSDialog()
{
    delete ui;
}

///
/// \brief PLSDialog::on_buttonBox_accepted
/// Call the appropriate function in VespucciDataset to perform PLS based on what
/// the user has entered when the user clicks "Ok"
void PLSDialog::on_buttonBox_accepted()
{
    int components = components_spin_box_->value();
    QString name = name_line_edit_->text();
    try{
        dataset_->PartialLeastSquares(name, components);
    }catch(exception e){
        workspace->main_window()->DisplayExceptionWarning(e);
    }
    close();
    dataset_.clear();
}

///
/// \brief PLSDialog::on_buttonBox_rejected
/// Close the window when the user clicks "Cancel"
void PLSDialog::on_buttonBox_rejected()
{
    close();
    dataset_.clear();
}

void PLSDialog::on_predictionCheckBox_clicked(bool checked)
{
    components_spin_box_->setVisible(!checked);
}
