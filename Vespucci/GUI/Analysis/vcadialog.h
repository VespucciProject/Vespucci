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
    explicit VCADialog(QWidget *parent, QSharedPointer<VespucciWorkspace> ws, QSharedPointer<AbstractDataAnalyzer> analyzer);
    ~VCADialog();

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

private:
    Ui::VCADialog *ui;
    ///
    /// \brief workspace
    /// The current workspace
    QSharedPointer<VespucciWorkspace> workspace_;

    ///
    /// \brief dataset_
    /// The current dataset
    QSharedPointer<AbstractDataAnalyzer> analyzer_;
};

#endif // VCADIALOG_H
