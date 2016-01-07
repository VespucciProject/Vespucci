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
#include "GUI/Analysis/vcadialog.h"
#include "ui_vcadialog.h"

///
/// \brief VCADialog::VCADialog
/// \param parent See QDialog
/// \param ws Current workspace
/// \param row Currently selected row in dataset list widget
///
VCADialog::VCADialog(QWidget *parent, VespucciWorkspace *ws, int row) :
    QDialog(parent),
    ui(new Ui::VCADialog)
{
    ui->setupUi(this);
    workspace = ws;
    data_ = workspace->DatasetAt(row);
    components_selector_spin_box_ = findChild<QSpinBox *>("endmembersSpinBox");
    image_component_selector_spin_box_ = findChild<QSpinBox *>("componentSpinBox");
    color_selector_combo_box_ = findChild<QComboBox *>("gradientComboBox");
    recalculate_check_box_ = findChild<QCheckBox *>("recalculateCheckBox");
    prediction_check_box_ = findChild<QCheckBox *>("predictionCheckBox");
    name_line_edit_ = findChild<QLineEdit*>("nameLineEdit");
    map_check_box_ = findChild<QCheckBox *>("mapCheckBox");
    data_index_ = row;

    image_component_selector_spin_box_->setEnabled(false);
    recalculate_check_box_->setEnabled(false);
    color_selector_combo_box_->setEnabled(false);
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
    if (prediction_check_box_->isChecked())
        endmembers = 0;
    else
        endmembers = components_selector_spin_box_->value();

    if (map_check_box_->isChecked()){
        int image_component = image_component_selector_spin_box_->value();

        QString name = name_line_edit_->text();
        bool recalculate = recalculate_check_box_->isChecked();
        int gradient_index = color_selector_combo_box_->currentIndex();

        try{
            data_->VertexComponents(endmembers,
                                    image_component,
                                    name,
                                    gradient_index,
                                    recalculate);
        }
        catch(std::exception e){
            workspace->main_window()->DisplayExceptionWarning(e);
        }
    }
    else{
        try{
            data_->VertexComponents(endmembers);
        }catch(std::exception e){
            workspace->main_window()->DisplayExceptionWarning(e);
        }
    }


    close();
    data_.clear();
}

///
/// \brief VCADialog::on_buttonBox_rejected
/// Closes window when "Cancel" clicked.
void VCADialog::on_buttonBox_rejected()
{
    close();
    data_.clear();
}

void VCADialog::on_predictionCheckBox_clicked(bool checked)
{
    components_selector_spin_box_->setEnabled(!checked);

}

void VCADialog::on_mapCheckBox_stateChanged(int arg1)
{
    image_component_selector_spin_box_->setEnabled(arg1);
    recalculate_check_box_->setEnabled(arg1);
    color_selector_combo_box_->setEnabled(arg1);
}
