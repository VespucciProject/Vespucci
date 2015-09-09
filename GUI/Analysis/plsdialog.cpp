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
#include "GUI/Analysis/plsdialog.h"
#include "ui_plsdialog.h"

PLSDialog::PLSDialog(QWidget *parent, VespucciWorkspace *ws, int row) :
    QDialog(parent),
    ui(new Ui::PLSDialog)
{
    ui->setupUi(this);
    workspace = ws;
    data_ = workspace->DatasetAt(row);
    image_component_selector_ = findChild<QSpinBox *>("imageComponentSpinBox");
    components_selector_ = findChild<QSpinBox *>("componentsSpinBox");
    color_selector_ = findChild<QComboBox *>("gradientComboBox");
    recalculate_box_ = findChild<QCheckBox *>("recalculateCheckBox");
    prediction_box_ = findChild<QCheckBox *>("predictionCheckBox");
    name_box_ = findChild<QLineEdit*>("mapNameLineEdit");
    map_check_box_ = findChild<QCheckBox *>("mapCheckBox");
    data_index_ = row;

    name_box_->setEnabled(false);
    recalculate_box_->setEnabled(false);
    color_selector_->setEnabled(false);
    image_component_selector_->setEnabled(false);



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
    int components = components_selector_->value();
    if (map_check_box_->isChecked()){
        int image_component;
        if (prediction_box_->isChecked())
            image_component = 0;
        else
            image_component = image_component_selector_->value();


        QString name = name_box_->text();
        bool recalculate = recalculate_box_->isChecked();
        int gradient_index = color_selector_->currentIndex();

        try{
            data_->PartialLeastSquares(components, image_component, name, gradient_index, recalculate);
        }
        catch(exception e){
            workspace->main_window()->DisplayExceptionWarning(e);
        }
    }
    else{
        try{
            data_->PartialLeastSquares(components);
        }catch(exception e){
            workspace->main_window()->DisplayExceptionWarning(e);
        }
    }
    close();
    data_.clear();
}

///
/// \brief PLSDialog::on_buttonBox_rejected
/// Close the window when the user clicks "Cancel"
void PLSDialog::on_buttonBox_rejected()
{
    close();
    data_.clear();
}

void PLSDialog::on_predictionCheckBox_clicked(bool checked)
{
    components_selector_->setVisible(!checked);
}

void PLSDialog::on_mapCheckBox_stateChanged(int arg1)
{
    name_box_->setEnabled(arg1);
    recalculate_box_->setEnabled(arg1);
    color_selector_->setEnabled(arg1);
    image_component_selector_->setEnabled(arg1);

}
