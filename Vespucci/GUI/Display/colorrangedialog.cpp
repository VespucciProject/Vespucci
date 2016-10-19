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
#include "colorrangedialog.h"
#include "ui_colorrangedialog.h"

ColorRangeDialog::ColorRangeDialog(GlobalGradientDialog *parent) :
    QDialog(parent),
    ui(new Ui::ColorRangeDialog)
{
    ui->setupUi(this);
    parent_ = parent;
    ui->minDoubleSpinBox->setRange(std::numeric_limits<double>::min(),
                                   std::numeric_limits<double>::max());
    ui->maxDoubleSpinBox->setRange(std::numeric_limits<double>::min(),
                                   std::numeric_limits<double>::max());
    ui->minDoubleSpinBox->setValue(0);
    ui->maxDoubleSpinBox->setValue(1000);
}

void ColorRangeDialog::SetGradientNames(QStringList gradient_names)
{
    ui->gradientComboBox->addItems(gradient_names);
}

void ColorRangeDialog::SetRange(double lower, double upper)
{
    ui->minDoubleSpinBox->setValue(lower);
    ui->maxDoubleSpinBox->setValue(upper);
}

void ColorRangeDialog::SetName(QString name, bool editable)
{
    ui->nameLineEdit->setText(name);
    ui->nameLineEdit->setEnabled(editable);
    ui->nameLabel->setEnabled(editable);
}

ColorRangeDialog::~ColorRangeDialog()
{
    delete ui;
}

void ColorRangeDialog::on_buttonBox_accepted()
{
    double lower = ui->minDoubleSpinBox->value();
    double upper = ui->maxDoubleSpinBox->value();
    QString name = ui->nameLineEdit->text();
    QString gradient_key = ui->gradientComboBox->currentText();
    if (upper <= lower){
        QMessageBox::warning(this, "Range Invalid",
                             "The range entered is not valid",
                             QMessageBox::Ok, 0);
        return;
    }

    parent_->AddGradient(name, gradient_key, lower, upper);
    close();
}

void ColorRangeDialog::on_buttonBox_rejected()
{
   close();
}
