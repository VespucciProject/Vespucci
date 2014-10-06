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

///
/// \brief The ScaleBarDialog class
/// Dialog for the user to create a scale bar on the image
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
    ///
    /// \brief width_box_
    /// User sets the width of the scale bar here
    QDoubleSpinBox *width_box_;
    ///
    /// \brief height_box_
    /// User sets the height of the scale bar here
    QDoubleSpinBox *height_box_;

    ///
    /// \brief units_box_
    /// User enters the spatial unit here
    QLineEdit *units_box_;

    ///
    /// \brief color_button_
    /// Pops up a dialog for the user to select the color
    QPushButton *color_button_;

    ///
    /// \brief position_box_
    /// User selects the position of the bar here
    QComboBox *position_box_;

    ///
    /// \brief font_box_
    /// User selects the font of the bar caption here
    QFontComboBox *font_box_;

    ///
    /// \brief font_size_box_
    /// User selects the size of the caption text here
    QSpinBox *font_size_box_;

    ///
    /// \brief color_
    /// The color selected by the user
    QColor color_;

    ///
    /// \brief map_data_
    /// The MapData object associated with this image
    MapData *map_data_;
};

#endif // SCALEBARDIALOG_H
