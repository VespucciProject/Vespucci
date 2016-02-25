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
    name_line_edit_ = findChild<QLineEdit *>("nameLineEdit");
    QString name = "Dataset" + QString::number(workspace->dataset_loading_count());
    name_line_edit_->setText(name); //puts in default name

    QDialogButtonBox *button_box = findChild<QDialogButtonBox *>("buttonBox");
    QPushButton *ok_button = button_box->button(QDialogButtonBox::Ok);
    ok_button->setDisabled(true);

    file_size_label_ = findChild<QLabel *>("fileSizeLabel");
    swap_check_box_ = findChild<QCheckBox *>("swapCheckBox");
    filename_line_edit_ = findChild<QLineEdit *>("filenameLineEdit");
    name_line_edit_ = findChild<QLineEdit *>("nameLineEdit");
    y_description_line_edit_ = findChild<QLineEdit *>("yDescriptionLineEdit");
    x_description_line_edit_ = findChild<QLineEdit *>("xDescriptionLineEdit");
    y_units_line_edit_ = findChild<QLineEdit *>("yUnitsLineEdit");
    x_units_line_edit_ = findChild<QLineEdit *>("xUnitsLineEdit");
    data_format_combo_box_ = findChild<QComboBox *>("dataFormatComboBox");
    button_box_ = findChild<QDialogButtonBox *>("buttonBox");


    ws->settings()->beginGroup("specdata");
    QString abs_label = ws->settings()->value("absLabel").toString();
    QString abs_units = ws->settings()->value("absUnits").toString();
    QString ord_label = ws->settings()->value("ordLabel").toString();
    QString ord_units = ws->settings()->value("ordUnits").toString();
    ws->settings()->endGroup();

    y_description_line_edit_->setText(ord_label);
    y_units_line_edit_->setText(ord_units);
    x_description_line_edit_->setText(abs_label);
    x_units_line_edit_->setText(abs_units);

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
    QString dataset_name = basename;
    int i = 1;
    while (workspace->dataset_names().contains(dataset_name)){
        dataset_name = basename + " (" + QString::number(i) + ")";
        ++i;
    }
    name_line_edit_->setText(dataset_name);
}

///
/// \brief LoadDataset::on_browseButton_clicked
/// Triggers dialog to open input file
void LoadDataset::on_browseButton_clicked()
{
    QString filename;

    filename = QFileDialog::getOpenFileName(this, tr("Open Data File"),
                                            workspace->directory(),
                                            tr("Text Files (*.txt);;"
                                               "Vespucci Dataset Files (*.vds);;"));
    filename_line_edit_->setText(filename);

}

///
/// \brief LoadDataset::on_buttonBox_accepted
/// Loads dataset from the file into a new dataset object by triggering appropriate
/// constructor.
void LoadDataset::on_buttonBox_accepted()
{
    QString y_description = y_description_line_edit_->text() + " (" + y_units_line_edit_->text() + ")";
    QString x_description = x_description_line_edit_->text() + " (" + x_units_line_edit_->text() + ")";

    QString name = name_line_edit_->text();
    QString filename = filename_line_edit_->text();
    QFileInfo file_info(filename);
    QString extension = file_info.suffix();

    if (workspace->dataset_names().contains(name)){
        QMessageBox::warning(this, "Name Exists", "A dataset with this name already exists.");
        return;
    }

    std::string format;
    const QString data_format_string = data_format_combo_box_->currentText();
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
/// \brief LoadDataset::on_buttonBox_rejected
/// Closes window when "Cancel" is selected.
void LoadDataset::on_buttonBox_rejected()
{
    close();
}

void LoadDataset::on_filenameLineEdit_textChanged(const QString &arg1)
{
    QFileInfo file_info(arg1);
    QPushButton *ok_button = button_box_->button(QDialogButtonBox::Ok);

    if(file_info.exists()){
        double file_size = file_info.size()/1048576;
        QString file_size_string = QString::number(file_size, 'f', 3);
        file_size_label_->setText(file_size_string);
        ok_button->setEnabled(true);
    }

    else{
        file_size_label_->setText("File does not exist!");
        ok_button->setDisabled(true);
    }
}
