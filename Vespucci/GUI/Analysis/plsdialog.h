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
#ifndef PLSDIALOG_H
#define PLSDIALOG_H

#include <QDialog>
#include "Global/vespucciworkspace.h"

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
    explicit PLSDialog(QWidget *parent, QSharedPointer<VespucciWorkspace> ws, const QString &dataset_key);
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
    QSharedPointer<VespucciWorkspace> workspace_;

    ///
    /// \brief image_component_selector_spin_box_
    /// Which component number to create the image from
    QSpinBox *image_component_selector_spin_box_;

    ///
    /// \brief components_spin_box_
    /// User selects how many components to calculate
    QSpinBox *components_spin_box_;

    ///
    /// \brief color_selector_combo_box_
    /// User selects color scheme here
    QComboBox *color_selector_combo_box_;

    ///
    /// \brief name_line_edit_
    /// User sets map name here
    QLineEdit *name_line_edit_;

    ///
    /// \brief recalculate_check_box_
    /// Whether or not to repeat PLS analysis
    QCheckBox *recalculate_check_box_;


    ///
    /// \brief prediction_check_box_
    /// User selects whether or not to allow prediction of dimensionality
    QCheckBox *prediction_check_box_;

    QCheckBox *map_check_box_;

    ///
    /// \brief data_index_
    /// Index of the dataset in the various lists
    int data_index_;


    QPushButton *browse_push_button_;
    QLabel *dataset_label_;
    QLabel *file_label_;
    QLineEdit *file_line_edit_;
    QComboBox *dataset_combo_box_;
    ///
    /// \brief dataset_
    /// Current dataset
    QSharedPointer<VespucciDataset> dataset_;
};

#endif // PLSDIALOG_H
