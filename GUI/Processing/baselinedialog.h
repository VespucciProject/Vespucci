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
#ifndef BASELINEDIALOG_H
#define BASELINEDIALOG_H

#include <QDialog>
#include "Global/vespucciworkspace.h"
class VespucciWorkspace;
class VespucciDataset;
namespace Ui {
class BaselineDialog;
}

///
/// \brief The BaselineDialog class
/// The dialog that allows the user to baseline-correct the data.
class BaselineDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BaselineDialog(QWidget *parent, VespucciWorkspace *ws, int row);
    ~BaselineDialog();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

    void on_methodComboBox_currentTextChanged(const QString &arg1);

    void on_pushButton_clicked();

private:
    Ui::BaselineDialog *ui;

    ///
    /// \brief dataset_
    /// Current dataset
    QSharedPointer<VespucciDataset> dataset_;

    ///
    /// \brief workspace
    /// Current workspace
    VespucciWorkspace *workspace;

    ///
    /// \brief iterations_box_
    /// Number of iterations to perform filtering to find baseline
    QSpinBox *param_1_spin_box_;

    ///
    /// \brief method_box_
    /// User chooses the method here
    QComboBox *method_box_;

    ///
    /// \brief window_size_box_
    /// User chooses the window size (if using filtering to find baseline) here.
    QSpinBox *param_0_spin_box_;

    QDoubleSpinBox *param_2_double_spin_box_;
    QDoubleSpinBox *param_3_double_spin_box_;

    QLabel *param_0_label_;
    QLabel *param_1_label_;
    QLabel *param_2_label_;
    QLabel *param_3_label_;

    QCustomPlot *spectrum_plot_;

    QVector<double> abscissa_q_;
    QVector<double> spectrum_q_;
    arma::vec abscissa_;
    arma::vec spectrum_;
};

#endif // BASELINEDIALOG_H
