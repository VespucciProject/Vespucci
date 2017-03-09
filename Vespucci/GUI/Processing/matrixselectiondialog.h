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
#ifndef MATRIXSELECTIONDIALOG_H
#define MATRIXSELECTIONDIALOG_H

#include <QDialog>
#include "GUI/QAbstractItemModel/datasettreemodel.h"
class DatasetTreeModel;
class TreeItem;

namespace Ui {
class MatrixSelectionDialog;
}

class MatrixSelectionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MatrixSelectionDialog(QWidget *parent, DatasetTreeModel *model);
    ~MatrixSelectionDialog();
    void SetModel(DatasetTreeModel *model);
    TreeItem *GetSelectedItem();
    QStringList GetDataKeys();
    bool accepted();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();
signals:
    void MatrixSelected(QStringList keys);
    void ItemSelected(TreeItem *item);

private:
    Ui::MatrixSelectionDialog *ui;
    bool accepted_;
    QStringList data_keys_;
    DatasetTreeModel *tree_model_;
    TreeItem *selected_item_;
};

#endif // MATRIXSELECTIONDIALOG_H
