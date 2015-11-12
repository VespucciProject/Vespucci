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

#include "GUI/Processing/dataextractordialog.h"
#include "ui_dataextractordialog.h"

///
/// \brief DataExtractorDialog::DataExtractorDialog
/// \param parent Parent QWidget
/// \param map The map from which the new dataset is created
/// \param dataset The current dataset from which the new dataset is created
/// \param main_window The main window of the program.
///
DataExtractorDialog::DataExtractorDialog(QWidget *parent, MapData *map, QSharedPointer<VespucciDataset> dataset, MainWindow *main_window) :
    QDialog(parent),
    ui(new Ui::DataExtractorDialog)
{
    ui->setupUi(this);
    main_window_ = main_window;
    workspace = main_window->workspace_ptr();
    name_line_edit_ = findChild<QLineEdit*>("nameLineEdit");
    upper_box_ = findChild<QDoubleSpinBox*>("upperDoubleSpinBox");
    lower_box_ = findChild<QDoubleSpinBox*>("lowerDoubleSpinBox");
    map_ = map;
    condition_ = map_->results_;
    dataset_ = dataset;
    data_range_ = map->dataRange();
    upper_box_->setValue(data_range_.upper);
    lower_box_->setValue(data_range_.lower);
    upper_box_->setRange(data_range_.lower, data_range_.upper);
    lower_box_->setRange(data_range_.lower, data_range_.upper);
    name_line_edit_->setText(dataset_->name() + "-" + map_->name());
    method_combo_box_ = findChild<QComboBox *>("methodComboBox");

}

DataExtractorDialog::DataExtractorDialog(QWidget *parent,
                                         vec data,
                                         QSharedPointer<VespucciDataset> dataset,
                                         MainWindow *main_window,
                                         QString name) :
    QDialog(parent),
    ui(new Ui::DataExtractorDialog)
{
    ui->setupUi(this);

    condition_ = data;

    main_window_ = main_window;
    workspace = main_window->workspace_ptr();
    name_line_edit_ = findChild<QLineEdit*>("nameLineEdit");
    filename_line_edit_ = findChild<QLineEdit *>("filenameLineEdit");
    upper_box_ = findChild<QDoubleSpinBox*>("upperDoubleSpinBox");
    lower_box_ = findChild<QDoubleSpinBox*>("lowerDoubleSpinBox");
    method_combo_box_ = findChild<QComboBox *>("methodComboBox");
    dataset_ = dataset;

    data_range_.lower = data.min();
    data_range_.upper = data.max();

    upper_box_->setValue(data_range_.upper);
    lower_box_->setValue(data_range_.lower);
    upper_box_->setRange(data_range_.lower, data_range_.upper);
    lower_box_->setRange(data_range_.lower, data_range_.upper);

    name_line_edit_->setText(dataset_->name() + "-" + name);
}

DataExtractorDialog::~DataExtractorDialog()
{
    delete ui;
}

///
/// \brief DataExtractorDialog::on_buttonBox_accepted
/// Calls constructor for new dataset object when user clicks "Ok"
void DataExtractorDialog::on_buttonBox_accepted()
{
    uvec indices;
    QString name = name_line_edit_->text();
    mat index_mat;

    if (method_combo_box_->currentText() == "Index File"){
        string filename = filename_line_edit_->text().toStdString();
        uword index;
        index_mat.load(filename);
        indices.set_size(index_mat.n_elem);
        for (uword i = 0; i < index_mat.n_elem; ++i){
            index = index_mat(i);
            indices(i) = index;
        }
    }
    else{
        double upper = upper_box_->value();
        double lower = lower_box_->value();
        indices = arma::find(condition_ >= lower && condition_ <= upper);
    }

    QFile *log_file = workspace->CreateLogFile(name);
    QSharedPointer<VespucciDataset> new_dataset;
    try{
        new_dataset = QSharedPointer<VespucciDataset>(new VespucciDataset(name,
                                                      main_window_,
                                                      workspace->directory_ptr(),
                                                      log_file,
                                                      dataset_,
                                                      indices));
        workspace->AddDataset(new_dataset);
    }
    catch(exception e){
        main_window_->DisplayExceptionWarning(e);
    }

    close();
    dataset_.clear();
}

///
/// \brief DataExtractorDialog::on_buttonBox_rejected
/// Closes the window when the user selects "Cancel"
void DataExtractorDialog::on_buttonBox_rejected()
{
    close();
    dataset_.clear();
}

void DataExtractorDialog::on_methodComboBox_currentIndexChanged(const QString &arg1)
{
    if (arg1 == "Index File"){
        upper_box_->setDisabled(true);
        lower_box_->setDisabled(true);
        filename_line_edit_->setDisabled(false);
    }
    else{
        upper_box_->setDisabled(false);
        lower_box_->setDisabled(false);
        filename_line_edit_->setDisabled(true);
    }
}

void DataExtractorDialog::on_browsePushButton_clicked()
{
    QString filename =
            QFileDialog::getOpenFileName(this,
                                         "Select Index File",
                                         workspace->directory(),
                                         "Data Files (*.txt *.csv *.arma)");
    filename_line_edit_->setText(filename);
}
