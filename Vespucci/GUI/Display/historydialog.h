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
#ifndef HISTORYDIALOG_H
#define HISTORYDIALOG_H

#include <QDialog>
#include "Global/vespucciworkspace.h"

namespace Ui {
class HistoryDialog;
}

class HistoryDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HistoryDialog(QWidget *parent, QSharedPointer<VespucciWorkspace> ws);
    ~HistoryDialog();
public slots:
    void DatasetSelectionChanged(QString key);
signals:
    void MacroRequested(const QStringList &macro);
private slots:
    void on_savePushButton_clicked();

    void on_sendPushButton_clicked();

    void on_refreshPushButton_clicked();
protected:
    void closeEvent(QCloseEvent *event);
    void showEvent(QShowEvent *event);
private:
    Ui::HistoryDialog *ui;
    QSharedPointer<VespucciWorkspace> workspace_;
    QString dataset_key_;
};

#endif // HISTORYDIALOG_H
