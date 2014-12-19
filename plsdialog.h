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
#ifndef PLSDIALOG_H
#define PLSDIALOG_H

#include <QDialog>
#include "vespucciworkspace.h"

namespace Ui {
class PLSDialog;
}

///
/// \brief The PLSDialog class
/// Dialog that allows the user to perform PLS determinant analysis
class PLSDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PLSDialog(QWidget *parent, VespucciWorkspace *ws, int row);
    ~PLSDialog();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

    void on_predictionCheckBox_clicked(bool checked);

private:
    Ui::PLSDialog *ui;
    ///
    /// \brief workspace
    /// Current workspace
    VespucciWorkspace *workspace;

    ///
    /// \brief image_component_selector_
    /// Which component number to create the image from
    QSpinBox *image_component_selector_;

    ///
    /// \brief components_selector_
    /// User selects how many components to calculate
    QSpinBox *components_selector_;

    ///
    /// \brief color_selector_
    /// User selects color scheme here
    QComboBox *color_selector_;

    ///
    /// \brief name_box_
    /// User sets map name here
    QLineEdit *name_box_;

    ///
    /// \brief recalculate_box_
    /// Whether or not to repeat PLS analysis
    QCheckBox *recalculate_box_;


    ///
    /// \brief prediction_box_
    /// User selects whether or not to allow prediction of dimensionality
    QCheckBox *prediction_box_;


    ///
    /// \brief data_index_
    /// Index of the dataset in the various lists
    int data_index_;

    ///
    /// \brief data_
    /// Current dataset
    QSharedPointer<VespucciDataset> data_;
};

#endif // PLSDIALOG_H
