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

PrincipalComponentsDialog::PrincipalComponentsDialog(QWidget *parent, VespucciWorkspace *ws, int row) :
    QDialog(parent),
    ui(new Ui::PrincipalComponentsDialog)
{
    ui->setupUi(this);
    workspace = ws;
    component_selector_ = this->findChild<QSpinBox *>("componentSpinBox");
    color_selector_ = this->findChild<QComboBox *>("gradientComboBox");
    name_box_ = this->findChild<QLineEdit*>("nameLineEdit");
    data_ = workspace->DatasetAt(row);
    data_index_ = row;
}

PrincipalComponentsDialog::~PrincipalComponentsDialog()
{
    delete ui;
}

void PrincipalComponentsDialog::on_buttonBox_accepted()
{
    cout << "PrincipalComponentsDialog::on_buttonBox_accepted()" << endl;
    cout << "get value from component selector" << endl;
    int component = component_selector_->value();
    //cout << "get value of negative score checkbox" << endl;
    //bool negative_scores = negative_score_box_->isChecked();
    bool negative_scores = true;
    cout << "get name of map" << endl;
    QString name = name_box_->text();
    cout << "get gradient index" << endl;
    int gradient_index = color_selector_->currentIndex();
    cout << "call to SpecMap::PrincipalComponents" << endl;
    data_->PrincipalComponents(component, negative_scores, name, gradient_index);
}
