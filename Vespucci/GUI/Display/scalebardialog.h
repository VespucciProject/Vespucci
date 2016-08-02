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
#ifndef SCALEBARDIALOG_H
#define SCALEBARDIALOG_H

#include <QDialog>
#include "GUI/Display/mapviewer.h"

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
    explicit ScaleBarDialog(QWidget *parent, QSharedPointer<MapData> map_data);
    ~ScaleBarDialog();

private slots:
    void on_colorPushButton_clicked();

    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::ScaleBarDialog *ui;

    ///
    /// \brief color_
    /// The color selected by the user
    QColor color_;

    ///
    /// \brief map_data_
    /// The MapData object associated with this image
    QSharedPointer<MapData> map_data_;
};

#endif // SCALEBARDIALOG_H
