/*******************************************************************************
    Copyright (C) 2014-2017 Wright State University - All Rights Reserved
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
#include "GUI/Display/scalebardialog.h"
#include "ui_scalebardialog.h"

///
/// \brief ScaleBarDialog::ScaleBarDialog
/// \param parent See QWidget
/// \param map_data MapData associated with the MapViewer that launches this dialog
///
ScaleBarDialog::ScaleBarDialog(QWidget *parent, QSharedPointer<MapData> map_data) :
    QDialog(parent),
    ui(new Ui::ScaleBarDialog)
{
    ui->setupUi(this);
    color_ = Qt::black;
    map_data_ = map_data;
}

ScaleBarDialog::~ScaleBarDialog()
{
    delete ui;
}

///
/// \brief ScaleBarDialog::on_colorPushButton_clicked
/// Opens a QColorDialog for the user to select a color
void ScaleBarDialog::on_colorPushButton_clicked()
{
    color_ = QColorDialog::getColor(color_, this, "Select Color");
}

///
/// \brief ScaleBarDialog::on_buttonBox_accepted
/// Draw the scale bar when user clicks "Ok"
void ScaleBarDialog::on_buttonBox_accepted()
{
    QMessageBox::information(this, "Scale Bar", "Click on the map to show the scale bar.");
    double width = ui->widthDoubleSpinBox->value();
    double height = ui->heightDoubleSpinBox->value();
    QString units = ui->unitsLineEdit->text();
    QString position = ui->positionComboBox->currentText();
    QFont font = ui->fontComboBox->currentFont();
    int font_size = ui->fontSizeSpinBox->value();
    font.setPointSize(font_size);
    font.setBold(true);


    map_data_->DrawScaleBar(width,
                            height,
                            units,
                            color_,
                            position,
                            font);
    close();

}

///
/// \brief ScaleBarDialog::on_buttonBox_rejected
/// Close the window when user clicks "Cancel"
void ScaleBarDialog::on_buttonBox_rejected()
{
    close();
}
