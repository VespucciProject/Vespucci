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
#include "GUI/Processing/datasetimportdialog.h"
#include "ui_datasetimportdialog.h"
#include <QFileDialog>
#include "Data/Dataset/vespuccidataset.h"
#include "Global/vespucciworkspace.h"

///
/// \brief DatasetImportDialog::DatasetImportDialog
/// \param parent QWidget parent (see QDialog)
/// \param ws Current workspace
///
DatasetImportDialog::DatasetImportDialog(QWidget *parent, QSharedPointer<VespucciWorkspace> ws) :
    QDialog(parent),
    ui(new Ui::DatasetImportDialog)
{
    ui->setupUi(this);
    workspace_ = ws;
    directory_ = ws->directory_ptr();
    QString name = "Dataset" + QString::number(workspace_->dataset_loading_count());
    ui->nameLineEdit->setText(name); //puts in default name

    QPushButton *ok_button = ui->buttonBox->button(QDialogButtonBox::Ok);
    ok_button->setDisabled(true);

    ws->settings()->beginGroup("specdata");
    QString abs_label = ws->settings()->value("absLabel").toString();
    QString abs_units = ws->settings()->value("absUnits").toString();
    QString ord_label = ws->settings()->value("ordLabel").toString();
    QString ord_units = ws->settings()->value("ordUnits").toString();
    ws->settings()->endGroup();

    ui->yDescriptionLineEdit->setText(ord_label);
    ui->yUnitsLineEdit->setText(ord_units);
    ui->xDescriptionLineEdit->setText(abs_label);
    ui->xUnitsLineEdit->setText(abs_units);

    connect(ui->filenameLineEdit,
            &QLineEdit::textChanged,
            this,
            &DatasetImportDialog::FilenameChanged);
}

DatasetImportDialog::~DatasetImportDialog()
{
    delete ui;
}

void DatasetImportDialog::FilenameChanged(QString new_filename)
{
    QFileInfo info(new_filename);
    QString basename = info.completeBaseName();
    QString dataset_name = basename;
    int i = 1;
    while (workspace_->dataset_names().contains(dataset_name))
        dataset_name = basename + " (" + QString::number(i++) + ")";

    ui->nameLineEdit->setText(dataset_name);
}

///
/// \brief DatasetImportDialog::on_browseButton_clicked
/// Triggers dialog to open input file
void DatasetImportDialog::on_browseButton_clicked()
{
    QString filename;

    filename = QFileDialog::getOpenFileName(this, tr("Open Data File"),
                                            workspace_->directory(),
                                            tr("Text Files (*.txt);;"
                                               "Vespucci Dataset Files (*.vds);;"));
    ui->filenameLineEdit->setText(filename);

}

///
/// \brief DatasetImportDialog::on_buttonBox_accepted
/// Loads dataset from the file into a new dataset object by triggering appropriate
/// constructor.
void DatasetImportDialog::on_buttonBox_accepted()
{
    QString y_description = ui->yDescriptionLineEdit->text() + " (" + ui->yUnitsLineEdit->text() + ")";
    QString x_description = ui->xDescriptionLineEdit->text() + " (" + ui->xUnitsLineEdit->text() + ")";

    QString name = ui->nameLineEdit->text();
    QString filename = ui->filenameLineEdit->text();
    QFileInfo file_info(filename);
    QString extension = file_info.suffix();

    if (workspace_->dataset_names().contains(name)){
        QMessageBox::warning(this, "Name Exists", "A dataset with this name already exists.");
        return;
    }

    std::string format;
    const QString data_format_string = ui->dataFormatComboBox->currentText();
    if (data_format_string == "Wide Text")
        format = "WideTabDel";
    else if (data_format_string == "Wide Text (CSV)")
        format = "WideCSV";
    else if (data_format_string == "Long Text")
        format = "LongTabDel";
    else if (data_format_string == "Long Text (CSV)")
        format = "LongCSV";
    else
        return;



    //bool remove_at_position = false;
    //int temp_position=0;
    //int position;
    bool swap = ui->swapCheckBox->isChecked();

    try{
        QSharedPointer<VespucciDataset> dataset(new VespucciDataset(filename,
                                                workspace_->main_window(),
                                                directory_,
                                                name,
                                                x_description,
                                                y_description,
                                                swap,
                                                format));
        if (!dataset->ConstructorCancelled()){
            workspace_->AddDataset(dataset);
            workspace_->set_directory(file_info.dir().absolutePath());
            dataset.clear();
            }
    }
    catch(exception e){
        workspace_->main_window()->DisplayExceptionWarning(e);
        return;
    }

    close();
}



///
/// \brief DatasetImportDialog::on_buttonBox_rejected
/// Closes window when "Cancel" is selected.
void DatasetImportDialog::on_buttonBox_rejected()
{
    close();
}

void DatasetImportDialog::on_filenameLineEdit_textChanged(const QString &arg1)
{
    QFileInfo file_info(arg1);
    QPushButton *ok_button = ui->buttonBox->button(QDialogButtonBox::Ok);

    if(file_info.exists()){
        double file_size = file_info.size()/1048576;
        QString file_size_string = QString::number(file_size, 'f', 3);
        ui->fileSizeLabel->setText(file_size_string);
        ok_button->setEnabled(true);
    }

    else{
        ui->fileSizeLabel->setText("File does not exist!");
        ok_button->setDisabled(true);
    }
}
