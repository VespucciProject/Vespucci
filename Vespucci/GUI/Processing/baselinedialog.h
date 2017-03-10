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
    explicit BaselineDialog(QWidget *parent, QSharedPointer<VespucciWorkspace> ws, const QString &dataset_key);
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
    QSharedPointer<VespucciWorkspace> workspace_;

    QVector<double> abscissa_q_;
    QVector<double> spectrum_q_;
    arma::vec abscissa_;
    arma::vec spectrum_;
};

#endif // BASELINEDIALOG_H
