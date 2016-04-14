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
#ifndef DATAVIEWER_H
#define DATAVIEWER_H

#include <QDialog>
#include "Global/vespucciworkspace.h"
#include "GUI/QAbstractItemModel/vespuccitablemodel.h"
#include "Data/Dataset/vespuccidataset.h"
#include "GUI/mainwindow.h"
class MainWindow;
class VespucciDataset;
class VespucciWorkspace;
class UnivariateData;
class VespucciTableModel;
class DataWidget;
namespace Ui {
class DataViewer;
}

///
/// \brief The DataViewer class
/// Window that displays dataset elements in a QTableView widget inside a QTabWidget
class DataViewer : public QDialog
{
    Q_OBJECT

public:
    explicit DataViewer(MainWindow *parent = 0);
    ~DataViewer();
    void AddTab(const mat &object, const QString &name);

public slots:
    void RemoveTab(int index);
private:
    Ui::DataViewer *ui;
    QTabWidget *tab_widget_;
    mat empty_matrix_;
};

#endif // DATAVIEWER_H





