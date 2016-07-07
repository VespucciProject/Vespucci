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
#ifndef PRINCIPALCOMPONENTSDIALOG_H
#define PRINCIPALCOMPONENTSDIALOG_H

#include <QDialog>
#include "Global/vespucciworkspace.h"
#include "Global/global.h"


namespace Ui {
class PrincipalComponentsDialog;
}

///
/// \brief The PrincipalComponentsDialog class
/// Dialog for performing principal components analysis
class PrincipalComponentsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PrincipalComponentsDialog(QWidget *parent, QSharedPointer<VespucciWorkspace> ws, const QString &dataset_key);
    ~PrincipalComponentsDialog();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::PrincipalComponentsDialog *ui;

    ///
    /// \brief workspace
    /// The current dataset
    QSharedPointer<VespucciWorkspace> workspace_;


    ///
    /// \brief data_index_
    /// Index of current dataset in relevant lists
    int data_index_;

    ///
    /// \brief dataset_
    /// Current dataset
    QSharedPointer<VespucciDataset> dataset_;
};

#endif // PRINCIPALCOMPONENTSDIALOG_H
