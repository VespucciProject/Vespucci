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
#ifndef GLOBALGRADIENTDIALOG_H
#define GLOBALGRADIENTDIALOG_H

#include <QDialog>
#include "Global/vespucciworkspace.h"
#include "GUI/Display/colorrangedialog.h"
class ColorRangeDialog;
namespace Ui {
class GlobalGradientDialog;
}

class GlobalGradientDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GlobalGradientDialog(MainWindow *parent, QSharedPointer<VespucciWorkspace> workspace);
    ~GlobalGradientDialog();
    void AddGradient(QString name, QString gradient_key, double lower, double upper);
private slots:
    void on_addPushButton_clicked();

    void on_editPushButton_clicked();

    void on_removePushButton_clicked();

    void on_recalculatePushButton_clicked();

private:
    void RefreshListWidget();
    Ui::GlobalGradientDialog *ui;
    QSharedPointer<VespucciWorkspace> workspace_;
    ColorRangeDialog *color_range_dialog_;
};

#endif // GLOBALGRADIENTDIALOG_H
