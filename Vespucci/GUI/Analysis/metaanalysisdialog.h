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
#ifndef METAANALYSISDIALOG_H
#define METAANALYSISDIALOG_H

#include <QDialog>
#include "Global/vespucciworkspace.h"
#include "GUI/Processing/matrixselectiondialog.h"


namespace Ui {
class MetaAnalysisDialog;
}

class MetaAnalysisDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MetaAnalysisDialog(QWidget *parent, QSharedPointer<VespucciWorkspace> ws, const QStringList &data_keys);
    ~MetaAnalysisDialog();
public slots:
    void MatrixSelected(QStringList keys);
private slots:
    void on_typeComboBox_currentTextChanged(const QString &arg1);

    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

    void on_selectPushButton_clicked();

private:
    Ui::MetaAnalysisDialog *ui;
    QSharedPointer<VespucciWorkspace> workspace_;
    QStringList data_keys_;
    QStringList control_data_keys_;
    MatrixSelectionDialog *matrix_selection_dialog_;
};

#endif // METAANALYSISDIALOG_H
