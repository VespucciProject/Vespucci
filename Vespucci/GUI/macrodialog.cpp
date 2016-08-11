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
#include "macrodialog.h"
#include "ui_macrodialog.h"
#include "Data/Dataset/macroparser.h"

MacroDialog::MacroDialog(MainWindow *parent, QSharedPointer<VespucciWorkspace> ws) :
    QDialog(parent),
    ui(new Ui::MacroDialog)
{
    ui->setupUi(this);
    workspace_ = ws;
    dataset_valid_ = false;
}

MacroDialog::~MacroDialog()
{
    delete ui;
}

void MacroDialog::SetActiveDataset(QSharedPointer<VespucciDataset> dataset)
{
    dataset_ = dataset;
    dataset_valid_ = true;
}

void MacroDialog::MacroRequested(const QStringList &macro)
{
    QString operations = ui->plainTextEdit->toPlainText() + "\n";
    for (auto line: macro){
        operations = operations + line + "\n";
    }
    ui->plainTextEdit->setPlainText(operations);
}

void MacroDialog::DatasetSelectionChanged(QString dataset_key)
{
    dataset_ = workspace_->GetDataset(dataset_key);
    ui->datasetLabel->setText("Dataset: " + dataset_key);
}

void MacroDialog::DatasetToBeRemoved(const QString &dataset_key)
{
    if (dataset_->name() == dataset_key){
        dataset_valid_ = false;
        dataset_ = QSharedPointer<VespucciDataset>(NULL);
        ui->datasetLabel->setText("Dataset: N/A");
    }
}

void MacroDialog::closeEvent(QCloseEvent *ev)
{
    QDialog::closeEvent(ev);
    emit SetActionChecked(false);
}

void MacroDialog::on_openToolButton_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, "Load Macro",
                                                    workspace_->directory(),
                                                    "Text files (*.txt)");
    QFileInfo file_info(filename);
    if (file_info.exists() && file_info.isFile()){
        QFile file(filename);
        file.open(QFile::ReadOnly);
        QTextStream stream(&file);
        QString text = stream.readAll();
        file.close();
        ui->plainTextEdit->clear();
        ui->plainTextEdit->setPlainText(text);
    }
}

void MacroDialog::on_saveToolButton_clicked()
{
    QString filename = QFileDialog::getSaveFileName(this, "Save Macro",
                                                    workspace_->directory(),
                                                    "Text files (*.txt)");
    QFile file(filename);
    file.open(QFile::WriteOnly);
    QTextStream stream(&file);
    QString text = ui->plainTextEdit->toPlainText();
    stream << text;
    file.close();
}

void MacroDialog::on_runToolButton_clicked()
{
    if (!dataset_.isNull()){
        MacroParser parser(dataset_);
        QString macro = ui->plainTextEdit->toPlainText();
        parser.LoadMacro(macro);
        bool ok = false;
        try{
            ok = parser.ExecuteMacro();
        }catch(exception e){
            workspace_->main_window()->DisplayExceptionWarning(e);
            return;
        }

        if (!ok){
            int error_line, error_param;
            parser.Error(error_line, error_param);
            QString description = "Error occured at parameter "
                    + QString::number(error_param)
                    + " in line " + QString::number(error_line);
            QMessageBox::warning(this, "Syntax Error", description);
        }
    }
}

void MacroDialog::on_helpToolButton_clicked()
{
    QDesktopServices::openUrl(QUrl("http://vespucciproject.org/Vespucci-docs/adv/macros.html"));
}

void MacroDialog::on_runAllToolButton_clicked()
{
    QString macro = ui->plainTextEdit->toPlainText();
    for (auto dataset_key: workspace_->dataset_names()){
        QSharedPointer<VespucciDataset> dataset = workspace_->GetDataset(dataset_key);
        MacroParser parser(dataset);
        parser.LoadMacro(macro);
        bool ok = false;
        try{
            ok = parser.ExecuteMacro();
        }catch(exception e){
            workspace_->main_window()->DisplayExceptionWarning(e);
            return;
        }

        if (!ok){
            int error_line, error_param;
            parser.Error(error_line, error_param);
            QString description = "Error occured at parameter "
                    + QString::number(error_param)
                    + " in line " + QString::number(error_line);
            QMessageBox::warning(this, "Syntax Error", description);
            return;
        }
    }

}
