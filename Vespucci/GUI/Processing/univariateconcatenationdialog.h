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
#ifndef UNIVARIATECONCATENATIONDIALOG_H
#define UNIVARIATECONCATENATIONDIALOG_H
#include "Global/vespucciworkspace.h"

#include <QDialog>

namespace Ui {
class UnivariateConcatenationDialog;
}

class UnivariateConcatenationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UnivariateConcatenationDialog(QWidget *parent, QSharedPointer<VespucciWorkspace> ws, const QString &dataset_key);
    ~UnivariateConcatenationDialog();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::UnivariateConcatenationDialog *ui;
    QSharedPointer<VespucciDataset> dataset_;
    QSharedPointer<VespucciWorkspace> workspace_;
};

#endif // UNIVARIATECONCATENATIONDIALOG_H
