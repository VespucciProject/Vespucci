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
#include "vcadialog.h"
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
    if(data_->non_spatial()){
        QMessageBox::warning(this,
                             "Non-spatial or Non-contiguous Dataset",
                             "Images cannot be created from non-spatial or "
                             "non-contiguous datasets.");
        this->close();
        data_.clear();
    }
    components_selector_ = this->findChild<QSpinBox *>("endmembersSpinBox");
    image_component_selector_ = this->findChild<QSpinBox *>("componentSpinBox");
    color_selector_ = this->findChild<QComboBox *>("gradientComboBox");
    recalculate_box_ = this->findChild<QCheckBox *>("recalculateCheckBox");
    name_box_ = this->findChild<QLineEdit*>("nameLineEdit");
    data_index_ = row;
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
    int image_component = image_component_selector_->value();
    int endmembers = components_selector_->value();
    if (image_component > endmembers)
        image_component = endmembers;

    QString name = name_box_->text();
    bool recalculate = recalculate_box_->isChecked();
    int gradient_index = color_selector_->currentIndex();

    try{
        data_->VertexComponents(endmembers,
                                image_component,
                                name,
                                gradient_index,
                                recalculate);
    }
    catch(exception e){
        workspace->main_window()->DisplayExceptionWarning(e);
    }
    this->close();
    data_.clear();
}

///
/// \brief VCADialog::on_buttonBox_rejected
/// Closes window when "Cancel" clicked.
void VCADialog::on_buttonBox_rejected()
{
    this->close();
    data_.clear();
}
