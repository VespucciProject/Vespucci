/*******************************************************************************
    Copyright (C) 2015 Wright State University - All Rights Reserved
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
#ifndef KMEANSDIALOG_H
#define KMEANSDIALOG_H

#include <QDialog>
#include "Global/vespucciworkspace.h"

namespace Ui {
class KMeansDialog;
}

///
/// \brief The KMeansDialog class
/// Allows the user to create a k-means clustering map
class KMeansDialog : public QDialog
{
    Q_OBJECT

public:
    explicit KMeansDialog(QWidget *parent, VespucciWorkspace *ws, int row);
    ~KMeansDialog();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

    void on_predictionCheckBox_clicked(bool checked);
private:
    Ui::KMeansDialog *ui;
    ///
    /// \brief workspace
    /// Current workspace
    VespucciWorkspace *workspace;

    ///
    /// \brief cluster_spin_box_
    /// User selects the number of clusters to compute here
    QSpinBox *cluster_spin_box_;

    ///
    /// \brief prediction_check_box_
    /// User selects whether or not to allow prediction of dimensionality
    QCheckBox *prediction_check_box_;


    ///
    /// \brief name_line_edit_
    /// User enters the name of the map here
    QLineEdit *name_line_edit_;

    ///
    /// \brief metric_combo_box_
    /// Selector for distance metric
    QComboBox *metric_combo_box_;

    ///
    /// \brief data_index_
    /// Index of the selected dataset in the master lists
    int data_index_;

    ///
    /// \brief data_
    /// The current dataset
    QSharedPointer<VespucciDataset> data_;
};

#endif // KMEANSDIALOG_H
