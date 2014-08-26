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
#ifndef SCALEBARDIALOG_H
#define SCALEBARDIALOG_H

#include <QDialog>
#include "mapviewer.h"

class MapData;
class MapViewer;

namespace Ui {
class ScaleBarDialog;
}

class ScaleBarDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ScaleBarDialog(QWidget *parent, MapData *map_data);
    ~ScaleBarDialog();

private slots:
    void on_colorPushButton_clicked();

    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::ScaleBarDialog *ui;
    QDoubleSpinBox *width_box_;
    QDoubleSpinBox *height_box_;
    QLineEdit *units_box_;
    QPushButton *color_button_;
    QComboBox *position_box_;
    QFontComboBox *font_box_;
    QSpinBox *font_size_box_;
    QColor color_;
    MapData *map_data_;
};

#endif // SCALEBARDIALOG_H
