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
#include "GUI/Display/dataviewer.h"
#include "ui_dataviewer.h"
#include "GUI/Display/spectrumviewer.h"
#include "GUI/Display/datawidget.h"
///
/// \brief DataViewer::DataViewer
/// \param parent Usually MainWindow, because ther
/// \param ws The "global" workspace, provides access to other dialogs
///
DataViewer::DataViewer(MainWindow *parent)
    :QDialog(parent),
     ui(new Ui::DataViewer)
{
    ui->setupUi(this);
    ui->tabWidget->addTab(new DataWidget(this, new VespucciTableModel(this, empty_matrix_)), "Data");
    QObject::connect(ui->tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(RemoveTab(int)));

}

DataViewer::~DataViewer()
{
    delete ui;
}

void DataViewer::RemoveTab(int index)
{
    //if we wanted to warn the user about closing a tab, we'd put it here
    ui->tabWidget->removeTab(index);
}

void DataViewer::AddTab(const mat &object, const QString &name)
{
    VespucciTableModel *new_model = new VespucciTableModel(this, object);
    DataWidget *new_widget = new DataWidget(this, new_model);
    ui->tabWidget->addTab(new_widget, name);
}

const mat & DataViewer::EmptyMatrix()
{
    return empty_matrix_;
}

