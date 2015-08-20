/*******************************************************************************
    Copyright (C) 2015 Wright State University - All Rights Reserved
    Daniel P. Foose - Author

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
#include "GUI/Processing/loaddataset.h"
#include "ui_loaddataset.h"
#include <QFileDialog>
#include "Data/Dataset/vespuccidataset.h"
#include "Global/vespucciworkspace.h"

///
/// \brief LoadDataset::LoadDataset
/// \param parent QWidget parent (see QDialog)
/// \param ws Current workspace
///
LoadDataset::LoadDataset(QWidget *parent, VespucciWorkspace *ws) :
    QDialog(parent),
    ui(new Ui::LoadDataset)
{
    ui->setupUi(this);
    workspace = ws;
    directory_ = ws->directory_ptr();
    QLineEdit *name_box = findChild<QLineEdit *>("nameBox");
    QString name = "Dataset" + QString::number(workspace->dataset_loading_count());
    name_box->setText(name); //puts in default name

    QDialogButtonBox *button_box = findChild<QDialogButtonBox *>("buttonBox");
    QPushButton *ok_button = button_box->button(QDialogButtonBox::Ok);
    ok_button->setDisabled(true);


    swap_check_box_ = findChild<QCheckBox *>("swapCheckBox");
    filename_line_edit_ = findChild<QLineEdit *>("filenameBox");
    name_box_ = findChild<QLineEdit *>("nameBox");
    y_description_box_ = findChild<QLineEdit *>("yDescription");
    x_description_box_ = findChild<QLineEdit *>("xDescription");
    y_units_box_ = findChild<QLineEdit *>("yUnits");
    x_units_box_ = findChild<QLineEdit *>("xUnits");
    data_format_box_ = findChild<QComboBox *>("dataFormatBox");
    QObject::connect(filename_line_edit_,
                     SIGNAL(textChanged(QString)),
                     this,
                     SLOT(FilenameChanged(QString)));
}

LoadDataset::~LoadDataset()
{
    delete ui;
}

void LoadDataset::FilenameChanged(QString new_filename)
{
    QFileInfo info(new_filename);
    QString basename = info.baseName();
    name_box_->setText(basename);
}

///
/// \brief LoadDataset::on_browseButton_clicked
/// Triggers dialog to open input file
void LoadDataset::on_browseButton_clicked()
{
    QString filename;
    QLineEdit *filename_line_edit = findChild<QLineEdit *>("filenameBox");

    filename = QFileDialog::getOpenFileName(this, tr("Open Data File"),
                                            workspace->directory(),
                                            tr("Text Files (*.txt);;"
                                               //"SPC Files (*.spc);;"
                                               "Vespucci Dataset Files (*.vds);;"));
    filename_line_edit->setText(filename);

}

///
/// \brief LoadDataset::on_buttonBox_accepted
/// Loads dataset from the file into a new dataset object by triggering appropriate
/// constructor.
void LoadDataset::on_buttonBox_accepted()
{
    QString y_description = y_description_box_->text() + " (" + y_units_box_->text() + ")";
    QString x_description = x_description_box_->text() + " (" + x_units_box_->text() + ")";

    QString name = name_box_->text();
    QString filename = filename_line_edit_->text();
    QFileInfo file_info(filename);
    QString extension = file_info.suffix();

    std::string format;
    const QString data_format_string = data_format_box_->currentText();
    if (extension == "vds" || (data_format_string == "Vespucci Dataset" && extension != "txt"))
        format = "VespucciBinary";
    else if (data_format_string == "Wide Text")
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
    bool swap = swap_check_box_->isChecked();

    QStringList names_list = workspace->dataset_names();
    QStringList::iterator i;

    int warning_response=QMessageBox::Yes;
    //Warning message pops up when user enters a name that already exists
    for (i=names_list.begin(); i != names_list.end(); ++i){
        if (name==(*i)){
            warning_response =
                    QMessageBox::question(this,
                                          "Duplicate Name",
                                          "There is already a dataset with this"
                                          " name in the workspace. Datasets are"
                                          " not indexed by name, but using two"
                                          " datasets with the same name may lead"
                                          " to confusion. Are you sure you wish"
                                          " to continue with this name?",
                                          QMessageBox::No,
                                          QMessageBox::Yes);
        }
    }


    if (warning_response == QMessageBox::Yes && file_info.exists()){
        QFile *log_file = workspace->CreateLogFile(name);

        if (format == "VespucciBinary"){
            try{
                QSharedPointer<VespucciDataset> dataset(new VespucciDataset(filename,
                                                            workspace->main_window(),
                                                            directory_,
                                                            log_file,
                                                            name,
                                                            x_description,
                                                            y_description));
                    workspace->AddDataset(dataset);
                    workspace->set_directory(file_info.dir().absolutePath());
                    //dataset.clear(); //dataset should be copied to list
            }
            catch(exception e){
                workspace->main_window()->DisplayExceptionWarning(e);
                delete log_file;
                return;
            }
        }

        else{
            try{
                QSharedPointer<VespucciDataset> dataset(new VespucciDataset(filename,
                                                         workspace->main_window(),
                                                         directory_,
                                                         log_file,
                                                         name,
                                                         x_description,
                                                         y_description,
                                                         swap,
                                                         format));
                if (!dataset->ConstructorCancelled()){
                    workspace->AddDataset(dataset);
                    workspace->set_directory(file_info.dir().absolutePath());
                    dataset.clear();
                }
            }
            catch(exception e){
                workspace->main_window()->DisplayExceptionWarning(e);
                delete log_file;
                return;
            }
        }

    }
    close();

}


///
/// \brief LoadDataset::on_filenameBox_textChanged
/// \param arg1
/// Changes file info displays when file name is changed
void LoadDataset::on_filenameBox_textChanged(const QString &arg1)
{
    QLabel *file_size_label = findChild<QLabel *>("fileSize");
    QFileInfo file_info(arg1);
    QDialogButtonBox *button_box = findChild<QDialogButtonBox *>("buttonBox");
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

///
/// \brief LoadDataset::on_buttonBox_rejected
/// Closes window when "Cancel" is selected.
void LoadDataset::on_buttonBox_rejected()
{
    close();
}
