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
#ifndef VCADIALOG_H
#define VCADIALOG_H

#include <QDialog>
#include "Global/vespucciworkspace.h"

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
    explicit VCADialog(QWidget *parent, QSharedPointer<VespucciWorkspace> ws, const QString &dataset_key);
    ~VCADialog();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

    void on_predictionCheckBox_clicked(bool checked);

private:
    Ui::VCADialog *ui;
    ///
    /// \brief workspace
    /// The current workspace
    QSharedPointer<VespucciWorkspace> workspace_;

    ///
    /// \brief data_index_
    /// Index of the current dataset in the relevant lists
    int data_index_;

    ///
    /// \brief dataset_
    /// The current dataset
    QSharedPointer<VespucciDataset> dataset_;
};

#endif // VCADIALOG_H
