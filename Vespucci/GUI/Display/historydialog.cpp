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
#include "historydialog.h"
#include "ui_historydialog.h"
#include "Global/vespucciworkspace.h"

HistoryDialog::HistoryDialog(QWidget *parent, QSharedPointer<VespucciWorkspace> ws) :
    QDialog(parent),
    ui(new Ui::HistoryDialog)
{
    ui->setupUi(this);
    workspace_ = ws;
}

HistoryDialog::~HistoryDialog()
{
    delete ui;
}

void HistoryDialog::DatasetSelectionChanged(QString key)
{
    dataset_key_ = key;
    QSharedPointer<VespucciDataset> dataset = workspace_->GetDataset(key);
    QStringList actions = dataset->operations();
    ui->actionsListWidget->clear();
    ui->actionsListWidget->addItems(actions);
    ui->datasetLabel->setText("History for " + key);
}

void HistoryDialog::on_savePushButton_clicked()
{
    QString filename = QFileDialog::getSaveFileName(this, "Save macro",
                                                    workspace_->directory(),
                                                    "Text files (*.txt)");
    if (filename.isEmpty()) return;
    QFile outfile(filename);
    outfile.open(QFile::WriteOnly);
    QTextStream outstream(&outfile);
    for (int i = 0; i < ui->actionsListWidget->count(); ++i)
        outstream << ui->actionsListWidget->item(i)->text() << "\n";
    outfile.close();
}

void HistoryDialog::on_sendPushButton_clicked()
{
    QStringList macro;
    for (int i = 0; i < ui->actionsListWidget->count(); ++i)
        macro << ui->actionsListWidget->item(i)->text();
    emit MacroRequested(macro);
}

void HistoryDialog::on_refreshPushButton_clicked()
{
    ui->actionsListWidget->clear();
    if (workspace_->dataset_names().contains(dataset_key_)){
        QSharedPointer<VespucciDataset> dataset = workspace_->GetDataset(dataset_key_);
        QStringList operations = dataset->operations();
        ui->actionsListWidget->addItems(operations);
    }
}

void HistoryDialog::closeEvent(QCloseEvent *event)
{
    workspace_->main_window()->SetHistoryDialogActionChecked(false);
    event->accept();
}

void HistoryDialog::showEvent(QShowEvent *event)
{
    workspace_->main_window()->SetHistoryDialogActionChecked(true);
    event->accept();
}
