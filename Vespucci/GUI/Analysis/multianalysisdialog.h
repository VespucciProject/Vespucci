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
#ifndef MULTIANALYSISDIALOG_H
#define MULTIANALYSISDIALOG_H

#include <QDialog>
#include "Global/vespucciworkspace.h"

namespace Ui {
class MultiAnalysisDialog;
}

class MultiAnalysisDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MultiAnalysisDialog(QWidget *parent, QSharedPointer<VespucciWorkspace> ws);
    ~MultiAnalysisDialog();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::MultiAnalysisDialog *ui;
    QSharedPointer<VespucciWorkspace> workspace_;
};

#endif // MULTIANALYSISDIALOG_H
