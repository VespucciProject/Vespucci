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

#ifndef VCADIALOG_H
#define VCADIALOG_H

#include <QDialog>
#include "vespucciworkspace.h"

namespace Ui {
class VCADialog;
}
///
/// \brief The VCADialog class
/// A dialog that allows the user to perform vertex components analysis
class VCADialog : public QDialog
{
    Q_OBJECT

public:
    explicit VCADialog(QWidget *parent, VespucciWorkspace *ws, int row);
    ~VCADialog();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::VCADialog *ui;
    ///
    /// \brief workspace
    /// The current workspace
    VespucciWorkspace *workspace;

    ///
    /// \brief image_component_selector_
    /// The VCA component assocciated with the map to be created is selected here
    QSpinBox *image_component_selector_;

    ///
    /// \brief components_selector_
    /// The user selects the number of components to compute here
    QSpinBox *components_selector_;

    ///
    /// \brief color_selector_
    /// The user selects the color gradient of the image here
    QComboBox *color_selector_;

    ///
    /// \brief name_box_
    /// User sets the name of the image to be created here
    QLineEdit *name_box_;

    ///
    /// \brief recalculate_box_
    /// User selects whether or not to recalculate VCA here
    QCheckBox *recalculate_box_;

    ///
    /// \brief data_index_
    /// Index of the current dataset in the relevant lists
    int data_index_;

    ///
    /// \brief data_
    /// The current dataset
    QSharedPointer<VespucciDataset> data_;
};

#endif // VCADIALOG_H
