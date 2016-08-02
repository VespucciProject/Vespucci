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
#include "GUI/Display/datawidget.h"
#include "Global/global.h"
///
/// \brief DataViewer::DataViewer
/// \param parent the main window
/// \param ws The "global" workspace, provides access to the data model.
///
DataViewer::DataViewer(MainWindow *parent, QSharedPointer<VespucciWorkspace> ws)
    :QDockWidget(parent),
     ui(new Ui::DataViewer)
{
    ui->setupUi(this);
    workspace_ = ws;
    connect(ui->tabWidget, &QTabWidget::tabCloseRequested,
            this, &DataViewer::RemoveTab);
}

DataViewer::~DataViewer()
{
    delete ui;
}

void DataViewer::RemoveTab(int index)
{
    //if we wanted to warn the user about closing a tab, we'd put it here
    //but we don't have to because they can always get it back as long as the
    //matrix exists
    ui->tabWidget->removeTab(index);
}

///
/// \brief DataViewer::DatasetRemoved
/// \param name
/// The signal that triggers this slot must be issued BEFORE a dataset is removed
/// from the model, or else a segfault could occur with open tabs containing the
/// data (because the table model holds a reference and the reference becomes invalid
/// when the dataset is removed)
void DataViewer::DatasetToBeRemoved(QString name)
{
    //iterate backwards through widgets and remove any widgets that represent
    //matrices coming from the dataset about to be removed.
    int i = ui->tabWidget->count();
    while (i--){
        DataWidget *widget = qobject_cast<DataWidget *>(ui->tabWidget->widget(i));
        if (widget->GetTableModel()->data_keys().first() == name)
            ui->tabWidget->removeTab(i);
    }
}

///
/// \brief DataViewer::MatrixToBeRemoved
/// \param keys
/// The signal that triggers this slot must be emitted BEFORE the matrix is removed
/// if not, bad things will happen.
void DataViewer::MatrixToBeRemoved(QStringList keys)
{
    int i = ui->tabWidget->count();
    while (i--){
        DataWidget *widget = qobject_cast<DataWidget *>(ui->tabWidget->widget(i));
        QStringList data_keys = widget->GetTableModel()->data_keys();
        if (Vespucci::KeysAreEqual(data_keys, keys))
            ui->tabWidget->removeTab(i);
    }
}

void DataViewer::AddTab(QStringList keys)
{
    VespucciTableModel *new_model = new VespucciTableModel(this,
                                                           workspace_->GetMatrix(keys),
                                                           keys);
    DataWidget *widget = new DataWidget(this, new_model);
    ui->tabWidget->addTab(widget, keys.last());
    ui->tabWidget->setCurrentIndex(ui->tabWidget->count() - 1);
}

void DataViewer::closeEvent(QCloseEvent *ev)
{
    QDockWidget::closeEvent(ev);
    emit SetActionChecked(false);
}

