/************************************************************************************
    Copyright (C) 2014 Daniel P. Foose - All Rights Reserved

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
***************************************************************************************/

#include "loaddataset.h"
#include "ui_loaddataset.h"
#include <QFileDialog>
#include "specmap.h"
#include "vespucciworkspace.h"

LoadDataset::LoadDataset(QWidget *parent, VespucciWorkspace *ws) :
    QDialog(parent),
    ui(new Ui::LoadDataset)
{
    ui->setupUi(this);
    workspace = ws;
    directory_ = ws->directory_ptr();
    QLineEdit *name_box = this->findChild<QLineEdit *>("nameBox");
    QString name = "Dataset" + QString::number(workspace->dataset_loading_count());
    name_box->setText(name); //puts in default name

    QDialogButtonBox *button_box = this->findChild<QDialogButtonBox *>("buttonBox");
    QPushButton *ok_button = button_box->button(QDialogButtonBox::Ok);
    ok_button->setDisabled(true);
}

LoadDataset::~LoadDataset()
{
    delete ui;
}

void LoadDataset::on_browseButton_clicked()
{
    QString filename;
    QLineEdit *filename_line_edit = this->findChild<QLineEdit *>("filenameBox");

    filename = QFileDialog::getOpenFileName(this, tr("Open Data File"),
                                            workspace->directory(),
                                            tr("Text Files (*.txt);; SPC Files (*.spc);;"));
    filename_line_edit->setText(filename);

}

void LoadDataset::on_buttonBox_accepted()
{

    QLineEdit *filename_line_edit = this->findChild<QLineEdit *>("filenameBox");
    QLineEdit *name_box = this->findChild<QLineEdit *>("nameBox");

    QString name = name_box->text();
    QString filename = filename_line_edit->text();
    QFileInfo file_info(filename);

    bool remove_at_position = false;
    int temp_position=0;
    int position;

    QStringList names_list = workspace->dataset_names();
    QStringList::iterator i;

    int warning_response=QMessageBox::Ok;
    //Warning message pops up when user enters a name that already exists
    for (i=names_list.begin(); i != names_list.end(); ++i){
        if (name==(*i)){
            QMessageBox warning;
            warning.setText("There is already a dataset with this name in the workspace.");
            warning.setInformativeText("Importing a dataset with this name will overwrite existing dataset.  OK to overwrite previous dataset.");
            warning.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
            warning.setDefaultButton(QMessageBox::Cancel);
            warning_response = warning.exec();
            if (warning_response==QMessageBox::Ok){
                remove_at_position = true;
                position = temp_position;
            }
        }
        ++temp_position;
    }

    if (remove_at_position==true){
        workspace->RemoveDatasetAt(position);
    }

    if (warning_response == QMessageBox::Ok && file_info.exists()){
        QFile inputfile(filename);
        inputfile.open(QIODevice::ReadOnly);
        QTextStream inputstream(&inputfile);
        SpecMap data(inputstream, workspace->main_window(), directory_);
        inputfile.close();

        data.SetName(name);

        workspace->AddDataset(data);
        workspace->set_directory(file_info.dir().absolutePath());

    }

}



void LoadDataset::on_filenameBox_textChanged(const QString &arg1)
{
    QLabel *file_size_label = this->findChild<QLabel *>("fileSize");
    QFileInfo file_info(arg1);
    QDialogButtonBox *button_box = this->findChild<QDialogButtonBox *>("buttonBox");
    QPushButton *ok_button = button_box->button(QDialogButtonBox::Ok);

    if(file_info.exists()){
        double file_size = file_info.size()/1048576;
        QString file_size_string = QString::number(file_size, 'f', 3);
        file_size_label->setText(file_size_string);
        ok_button->setEnabled(true);
    }

    else{
        file_size_label->setText("File does not exist!");
        ok_button->setDisabled(true);
    }

}
