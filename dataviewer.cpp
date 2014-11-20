/*******************************************************************************
    Copyright (C) 2014 Wright State University - All Rights Reserved
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
#include "dataviewer.h"
#include "ui_dataviewer.h"
#include "spectrumviewer.h"

///
/// \brief DataViewer::DataViewer
/// \param parent Parent QWidget
/// \param ws Current workspace
/// \param row Row of current dataset
///
DataViewer::DataViewer(QWidget *parent, VespucciWorkspace *ws, int row) :
    QDialog(parent),
    ui(new Ui::DataViewer)
{
    ui->setupUi(this);
    workspace = ws;
    dataset_ = workspace->DatasetAt(row);
    directory_ = workspace->directory();
    export_button_ = this->findChild<QPushButton *>("exportPushButton");
    plot_button_ = this->findChild<QPushButton *>("plotPushButton");
    data_selector_ = this->findChild<QComboBox *>("comboBox");
    plot_button_->setDisabled(true);
    export_button_->setDisabled(true);
    table_ = this->findChild<QTableView *>("tableView");

    QStringList object_list("Spectral Abcissa");
    object_list << "Spatial Data";

    if (dataset_->principal_components_calculated()){
        object_list << "PCA Coefficents" << "Eigenvalues of Covariance Matrix"
                    << "PCA t² Values";
    }
    if (dataset_->vertex_components_calculated()){
        object_list << "VCA Coefficients" << "VCA Endmembers" << "VCA Pure Pixel Indices";
        vca_endmembers_ = dataset_->vertex_components_data()->NumberComponents();
    }
    if (dataset_->partial_least_squares_calculated()){
        object_list << "PLS Variance" << "PLS Predictor Loading"
                    << "PLS Response Loading" << "PLS Predictor Scores"
                    << "PLS Response Scores" << "PLS Coefficients";
    }
    if (dataset_->k_means_calculated()){
        object_list << "K-means Assignments";
    }

    data_selector_->addItems(object_list);
    data_selector_->setCurrentIndex(0);
}

DataViewer::~DataViewer()
{
    delete ui;
}

///
/// \brief DataViewer::on_comboBox_currentTextChanged
/// \param arg1
/// Changes the table model when a user changes the selected data object
void DataViewer::on_comboBox_currentTextChanged(const QString &arg1)
{
    current_text_ = arg1;
    if (arg1 == "Spatial Data"){
        VespucciTableModel *table_model = new VespucciTableModel(this, dataset_, "spatial");
        table_->setModel(table_model);
        current_data_ = table_model->GetData();
        export_button_->setDisabled(true);
        plot_button_->setDisabled(true);
    }
    else if (arg1 == "Spectral Abcissa"){
        VespucciTableModel *table_model = new VespucciTableModel(this, dataset_->wavelength_ptr());
        current_data_ = table_model->GetData();
        table_->setModel(table_model);
        export_button_->setDisabled(false);
        plot_button_->setDisabled(true);
    }
    else if (arg1 == "PCA Coefficients"){
        current_data_ = dataset_->principal_components_data()->coeff();
        table_->setModel(new VespucciTableModel(this, current_data_));
        export_button_->setDisabled(false);
        plot_button_->setDisabled(true);
    }
    else if (arg1 == "Eigenvalues of Covariance Matrix"){
        current_data_ = (mat*) dataset_->principal_components_data()->latent();
        table_->setModel(new VespucciTableModel(this, current_data_));
        export_button_->setDisabled(false);
        plot_button_->setDisabled(true);
    }
    else if (arg1 == "PCA t² Values"){
        current_data_ = (mat*) dataset_->principal_components_data()->tsquared();
        table_->setModel(new VespucciTableModel(this, current_data_));
        export_button_->setDisabled(false);
        plot_button_->setDisabled(true);
    }
    else if (arg1 == "VCA Coefficients"){
        current_data_ = dataset_->vertex_components_data()->fractional_abundances();
        table_->setModel(new VespucciTableModel(this, current_data_));
        export_button_->setDisabled(false);
        plot_button_->setDisabled(true);
    }
    else if (arg1 == "VCA Endmembers"){
        current_data_ = dataset_->vertex_components_data()->endmember_spectra();
        table_->setModel(new VespucciTableModel(this, current_data_));
        export_button_->setDisabled(false);
        plot_button_->setDisabled(false);
    }
    else if (arg1 == "VCA Pure Pixel Indices"){
        current_data_ = dataset_->vertex_components_data()->indices();
        table_->setModel(new VespucciTableModel(this, current_data_));
        export_button_->setDisabled(false);
        plot_button_->setDisabled(true);
    }
    else if (arg1 == "K-means Assignments"){
        current_data_ = dataset_->k_means_data();
        table_->setModel(new VespucciTableModel(this, current_data_));
        export_button_->setDisabled(false);
        plot_button_->setDisabled(true);
    }
    else if (arg1 == "PLS Variance"){
        current_data_ = dataset_->partial_least_squares_data()->percent_variance();
        table_->setModel(new VespucciTableModel(this, current_data_));
        export_button_->setDisabled(false);
        plot_button_->setDisabled(true);
    }
    else if (arg1 == "PLS Predictor Loading"){
        current_data_ = dataset_->partial_least_squares_data()->X_loadings();
        table_->setModel(new VespucciTableModel(this, current_data_));
        export_button_->setDisabled(false);
        plot_button_->setDisabled(true);
    }
    else if (arg1 == "PLS Response Loading"){
        current_data_ = dataset_->partial_least_squares_data()->Y_loadings();
        table_->setModel(new VespucciTableModel(this, current_data_));
        export_button_->setDisabled(false);
        plot_button_->setDisabled(true);
    }
    else if (arg1 == "PLS Predictor Scores"){
        current_data_ = dataset_->partial_least_squares_data()->X_scores();
        table_->setModel(new VespucciTableModel(this, current_data_));
        export_button_->setDisabled(false);
        plot_button_->setDisabled(true);
    }
    else if (arg1 == "PLS Response Scores"){
        current_data_ = dataset_->partial_least_squares_data()->Y_scores();
        table_->setModel(new VespucciTableModel(this, current_data_));
        export_button_->setDisabled(false);
        plot_button_->setDisabled(true);
    }
    else if (arg1 == "PLS Coefficients"){
        current_data_ = dataset_->partial_least_squares_data()->coefficients();
        table_->setModel(new VespucciTableModel(this, current_data_));
        export_button_->setDisabled(false);
        plot_button_->setDisabled(true);
    }
    else{
        VespucciTableModel *table_model = new VespucciTableModel(this, dataset_->wavelength_ptr());
        current_data_ = table_model->GetData();
        table_->setModel(table_model);
        export_button_->setDisabled(false);
        plot_button_->setDisabled(true);
    }

    table_->resizeColumnsToContents();
    table_->resizeRowsToContents();
}

///
/// \brief DataViewer::on_plotPushButton_clicked
/// Triggers a dialog that allows the plotting of data
void DataViewer::on_plotPushButton_clicked()
{
    if (current_text_ == "VCA Endmembers"){
        bool ok;
        int endmember = QInputDialog::getInt(this,
                                             tr("Select Endmember"),
                                             tr("Endmember"), 1, 1,
                                             vca_endmembers_, 1, &ok, 0);
        endmember--; //fix indexing from human-intuitve 1 to programmer-intuitive 0

        if (ok){
            spectrum_viewer_ = new SpectrumViewer(this, dataset_, endmember, directory_, "VCA");
            spectrum_viewer_->show();
        }
    }
}

///
/// \brief DataViewer::on_exportPushButton_clicked
/// Triggers a save dialog to save the current dataset
void DataViewer::on_exportPushButton_clicked()
{
    QString filename = QFileDialog::getSaveFileName(this,
                                                    tr("Export As..."),
                                                    directory_,
                                                    tr("Comma-separated Variables (*.csv);;"
                                                       "Tab-delimited Text (*.txt);;"
                                                       "Armadillo Binary (*.arma);;"));
    QFileInfo file_info(filename);
    bool success = false;
    if (filename.isEmpty())
        return;

    QString extension = file_info.suffix();
    if (extension == "arma")
        success = current_data_->save(filename.toStdString(), arma_binary);
    else if (extension == "csv")
        success = current_data_->save(filename.toStdString(), csv_ascii);
    else
        success = current_data_->save(filename.toStdString(), raw_ascii);

    if (success)
        QMessageBox::information(this, "Success!", "File " + filename + " written successfully!");
    else
        QMessageBox::warning(this, "File Not Saved", "File was not written successfully.");

    return;
}
