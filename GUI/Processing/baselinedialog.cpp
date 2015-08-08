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
#include "GUI/Processing/baselinedialog.h"
#include "ui_baselinedialog.h"

///
/// \brief BaselineDialog::BaselineDialog
/// \param parent Parent widget, required for QDialog
/// \param ws The current workspace
/// \param row The currently selected row in the dataset list widget
///
BaselineDialog::BaselineDialog(QWidget *parent, VespucciWorkspace *ws, int row) :
    QDialog(parent),
    ui(new Ui::BaselineDialog)
{
    ui->setupUi(this);
    workspace = ws;
    dataset_ = workspace->DatasetAt(row);

    param_0_spin_box_ = findChild<QSpinBox *>("param_0SpinBox");
    method_box_ = findChild<QComboBox *>("methodComboBox");
    param_1_spin_box_ = findChild<QSpinBox *>("param_1SpinBox");
    param_2_double_spin_box_ = findChild<QDoubleSpinBox *>("param_2DoubleSpinBox");
    param_3_double_spin_box_ = findChild<QDoubleSpinBox *>("param_3DoubleSpinBox");

    param_0_label_ = findChild<QLabel *>("param_0Label");
    param_1_label_ = findChild<QLabel *>("param_1Label");
    param_2_label_ = findChild<QLabel *>("param_2Label");
    param_3_label_ = findChild<QLabel *>("param_3Label");

    param_0_label_->setText("Window Size");
    param_1_label_->setText("Iterations");
    param_0_label_->setVisible(true);
    param_1_label_->setVisible(true);
    param_0_spin_box_->setVisible(true);
    param_1_spin_box_->setVisible(true);

    param_2_double_spin_box_->setVisible(false);
    param_3_double_spin_box_->setVisible(false);
    param_2_label_->setVisible(false);
    param_3_label_->setVisible(false);

    workspace = ws;
}

BaselineDialog::~BaselineDialog()
{
    delete ui;
}

///
/// \brief BaselineDialog::on_buttonBox_accepted
/// When the user clicks "Ok", the baseline method of the dataset is called to
/// correct the baseline of each spectrum.
void BaselineDialog::on_buttonBox_accepted()
{

    QString method = method_box_->currentText();

    if (method == "Median Filter"){
        int window_size = param_0_spin_box_->value();
        int iterations = param_1_spin_box_->value();
        try{
            dataset_->Baseline(method, window_size, iterations);
        }
        catch(exception e){
            workspace->main_window()->DisplayExceptionWarning(e);
        }
    }
    else if (method == "Vancouver Raman Algorithm (IModPoly)"){
        int poly_order = param_0_spin_box_->value();
        int max_it = param_1_spin_box_->value();
        double threshold = param_2_double_spin_box_->value();
        try{
            dataset_->IModPolyBaseline(poly_order, max_it, threshold);
        }catch(exception e){
            workspace->main_window()->DisplayExceptionWarning(e);
        }
    }
    else if (method == "CWT"){
        int lambda = param_0_spin_box_->value();
        int penalty_order = param_1_spin_box_->value();
        double SNR_threshold = param_2_double_spin_box_->value();
        double peak_shape_threshold = param_3_double_spin_box_->value();
        try{
            dataset_->CWTBaseline(lambda, penalty_order, SNR_threshold, peak_shape_threshold);
        }catch(exception e){
            workspace->main_window()->DisplayExceptionWarning(e);
        }
    }
    else{
        QMessageBox::warning(this, "Error", "An Error Occurred in BaselineDialog");
    }

    close();
    dataset_.clear();
}

///
/// \brief BaselineDialog::on_buttonBox_rejected
/// Window closes when user clicks "Cancel"
void BaselineDialog::on_buttonBox_rejected()
{
    close();
    dataset_.clear();
}

void BaselineDialog::on_methodComboBox_currentTextChanged(const QString &arg1)
{
    if (arg1 == "Median Filter"){
        param_0_label_->setText("Window Size");
        param_1_label_->setText("Iterations");
        param_0_label_->setVisible(true);
        param_1_label_->setVisible(true);
        param_0_spin_box_->setVisible(true);
        param_1_spin_box_->setVisible(true);

        param_2_double_spin_box_->setVisible(false);
        param_3_double_spin_box_->setVisible(false);
        param_2_label_->setVisible(false);
        param_3_label_->setVisible(false);
    }
    else if (arg1 == "Vancouver Raman Algorithm (IModPoly)"){
        param_0_label_->setText("Polynomial Order");
        param_0_label_->setVisible(true);
        param_0_spin_box_->setVisible(true);

        param_1_label_->setText("Maximum Iterations");
        param_1_label_->setVisible(true);
        param_1_spin_box_->setVisible(true);

        param_2_label_->setText("Threshold");
        param_2_label_->setVisible(true);
        param_2_double_spin_box_->setVisible(true);

        param_3_label_->setVisible(false);
        param_3_double_spin_box_->setVisible(false);
    }
    else if (arg1 == "CWT"){
        param_0_label_->setText("Whittaker Lambda");
        param_0_label_->setVisible(true);
        param_0_spin_box_->setVisible(true);

        param_1_label_->setText("Whittaker Penalty Order");
        param_1_label_->setVisible(true);
        param_1_spin_box_->setVisible(true);

        param_2_label_->setText("Peak SNR Threshold");
        param_2_label_->setVisible(true);
        param_2_double_spin_box_->setVisible(true);

        param_3_lable_->setText("Peak Shape Threshold");
        param_3_label_->setVisible(false);
        param_3_double_spin_box_->setVisible(false);
    }
}
