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
#ifndef DATAWIDGET_H
#define DATAWIDGET_H

#include <QWidget>
#include <GUI/QAbstractItemModel/vespuccitablemodel.h>

namespace Ui {
class DataWidget;
}

class DataWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DataWidget(QWidget *parent, VespucciTableModel *table_model);
    void SetTableModel(VespucciTableModel *table_model);
    VespucciTableModel *GetTableModel();
    ~DataWidget();

private slots:
    void on_forwardPushButton_clicked();

    void on_backPushButton_clicked();

private:
    Ui::DataWidget *ui;
    VespucciTableModel *table_model_;
    uword matrix_columns_;
    uword subviews_;
    uword current_start_column_;

};

#endif // DATAWIDGET_H
