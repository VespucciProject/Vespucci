/*******************************************************************************
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
*******************************************************************************/
#include "scalebardialog.h"
#include "ui_scalebardialog.h"

ScaleBarDialog::ScaleBarDialog(QWidget *parent, MapData *map_data) :
    QDialog(parent),
    ui(new Ui::ScaleBarDialog)
{
    ui->setupUi(this);
    width_box_ = this->findChild<QDoubleSpinBox *>("widthDoubleSpinBox");
    height_box_ = this->findChild<QDoubleSpinBox *>("heightDoubleSpinBox");
    units_box_ = this->findChild<QLineEdit *>("unitsLineEdit");
    color_button_ = this->findChild<QPushButton *>("colorPushButton");
    position_box_ = this->findChild<QComboBox *>("positionComboBox");
    font_box_ = this->findChild<QFontComboBox *>("fontComboBox");
    font_size_box_ = this->findChild<QSpinBox *>("fontSizeSpinBox");
    color_ = Qt::black;
    map_data_ = map_data;
}

ScaleBarDialog::~ScaleBarDialog()
{
    delete ui;
}

void ScaleBarDialog::on_colorPushButton_clicked()
{
    color_ = QColorDialog::getColor(color_, this, "Select Color");
}

void ScaleBarDialog::on_buttonBox_accepted()
{
    QMessageBox::information(this, "Scale Bar", "Click on the map to show the scale bar.");
    double width = width_box_->value();
    double height = height_box_->value();
    QString units = units_box_->text();
    QString position = position_box_->currentText();
    QFont font = font_box_->currentFont();
    int font_size = font_size_box_->value();
    font.setPointSize(font_size);
    font.setBold(true);


    map_data_->DrawScaleBar(width,
                            height,
                            units,
                            color_,
                            position,
                            font);
}
