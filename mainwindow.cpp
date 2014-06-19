/*******************************************************************************
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
*******************************************************************************/

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "loaddataset.h"
#include "aboutdialog.h"
#include "citationdialog.h"
#include "univariatedialog.h"
#include "bandratiodialog.h"
#include "principalcomponentsdialog.h"
#include "filterdialog.h"
#include "cropdialog.h"
#include "baselinedialog.h"
#include "dataviewer.h"

///
/// \brief MainWindow::MainWindow
/// \param parent usually 0
/// \param ws the workspace of this instance of Vespucci
///Default constructor
MainWindow::MainWindow(QWidget *parent, VespucciWorkspace *ws) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    workspace = ws;
    ui->setupUi(this);
    map_list_widget_ = this->findChild<QListWidget *>("mapsListWidget");
    dataset_list_widget_ = this->findChild<QListWidget *>("datasetsListWidget");
}

MainWindow::~MainWindow()
{
    delete ui;
}

///
/// \brief MainWindow::closeEvent
/// \param event
///Exits the program
void MainWindow::closeEvent(QCloseEvent *event)
{
    int response = QMessageBox::question(this,
                                         "Exit?",
                                         "Are you sure you want to exit?");

    if (response == QMessageBox::Yes) {
        event->accept();
        qApp->exit();
    }

    else {
        event->ignore();
    }
}
///
/// \brief MainWindow::on_actionExit_triggered
///Exits the program.
void MainWindow::on_actionExit_triggered()
{
    int response = QMessageBox::question(this,
                                         "Exit?",
                                         "Are you sure you want to exit?");

    if (response == QMessageBox::Yes)
        qApp->exit();
}

///
/// \brief MainWindow::on_actionImport_Dataset_from_File_triggered
///Triggers the dialog to load a dataset.
void MainWindow::on_actionImport_Dataset_from_File_triggered()
{
    LoadDataset *load_dataset_window = new LoadDataset(this,workspace);
    load_dataset_window->show();
}

///
/// \brief MainWindow::on_actionAbout_Vespucci_triggered
///Triggers the "About Vespucci" window.
void MainWindow::on_actionAbout_Vespucci_triggered()
{
    AboutDialog *about_window = new AboutDialog(this);
    about_window->show();
}

///
/// \brief MainWindow::on_actionCiting_Vespucci_triggered
///Triggers a window containing citation information.
void MainWindow::on_actionCiting_Vespucci_triggered()
{
    CitationDialog *citation_window = new CitationDialog(this);
    citation_window->show();
}

///
/// \brief MainWindow::on_actionNew_Univariate_Map_triggered
///Triggers univariate dialog.
void MainWindow::on_actionNew_Univariate_Map_triggered()
{
    int row = dataset_list_widget_->currentRow();
    UnivariateDialog *univariate_dialog =
            new UnivariateDialog(this, workspace, row);
    univariate_dialog->show();
}

///
/// \brief MainWindow::on_actionNew_Band_Ratio_Map_triggered
///Triggers band ratio dialog.
void MainWindow::on_actionNew_Band_Ratio_Map_triggered()
{
    int row = dataset_list_widget_->currentRow();
    BandRatioDialog *band_ratio_dialog =
            new BandRatioDialog(this, workspace, row);
    band_ratio_dialog->show();

}

///
/// \brief MainWindow::on_actionPrincipal_Components_Analysis_triggered
///Triggers principal components dialog
void MainWindow::on_actionPrincipal_Components_Analysis_triggered()
{
    int row = dataset_list_widget_->currentRow();
    PrincipalComponentsDialog *principal_components_dialog =
            new PrincipalComponentsDialog(this, workspace, row);
    principal_components_dialog->show();
}

///
/// \brief MainWindow::on_actionNormalize_Standardize_triggered
///Normalizes data using Z-scores
void MainWindow::on_actionNormalize_Standardize_triggered()
{
    int row = dataset_list_widget_->currentRow();
    QSharedPointer<SpecMap> data = workspace->DatasetAt(row);
    QStringList methods;
    methods << tr("Min/Max") << tr("Unit Area") << tr("Z-score");
    bool ok;
    QString item = QInputDialog::getItem(this,
                                         tr("Normalization/Standardization"),
                                         tr("Method:"), methods, 0, false, &ok);
    if (ok && item == "Min/Max")
        data->MinMaxNormalize();

    else if (ok && item == "Unit Area")
        data->UnitAreaNormalize();

    else if (ok && item == "Z-score")
        data->ZScoreNormalize();

    else
        return;
}

///
/// \brief MainWindow::on_actionSubtract_Background_triggered
///Subtracts a background matrix (a saved armadillo binary matrix) from spectra
void MainWindow::on_actionSubtract_Background_triggered()
{
    int row = dataset_list_widget_->currentRow();
    QSharedPointer<SpecMap> data = workspace->DatasetAt(row);
    QString filename =
            QFileDialog::getOpenFileName(this,
                                         tr("Select Background File"),
                                         workspace->directory(),
                                         tr("Vespucci Spectrum Files (*.arma)"));
    mat input;
    bool success = input.load(filename.toStdString());
    if (!success){
        QMessageBox::warning(this, "File Open Error", "File cannot be opened");
        return;
    }
    else{
        data->SubtractBackground(input);
    }
}

///
/// \brief MainWindow::on_actionSpectra_triggered
///Saves the spectra matrix of selected dataset as binary, csv, or raw ascii.
void MainWindow::on_actionSpectra_triggered()
{
    bool success;
    int row = dataset_list_widget_->currentRow();
    QSharedPointer<SpecMap> dataset = workspace->DatasetAt(row);

    QString filename =
            QFileDialog::getSaveFileName(this,
                                         tr("Save Spectra Matrix"),
                                         workspace->directory(),
                                         tr("Vespucci Binary (*.arma);;"
                                            "Comma-separated Values (*.csv);;"
                                            "Tab-separated Txt (*.txt);;"));
    QFileInfo file_info(filename);

    if (file_info.suffix() == "arma")
        success = dataset->spectra().save(filename.toStdString(), arma_binary);
    else if (file_info.suffix() == "csv")
        success = dataset->spectra().save(filename.toStdString(), csv_ascii);
    else
        success = dataset->spectra().save(filename.toStdString(), raw_ascii);

    if (success)
        QMessageBox::information(this, "File Saved", "File written successfully!");
    else
        QMessageBox::warning(this, "File Not Saved", "Could not save file.");
}

void MainWindow::on_actionAverage_Spectra_triggered()
{
    bool success;
    int row = dataset_list_widget_->currentRow();
    QSharedPointer<SpecMap> dataset = workspace->DatasetAt(row);

    QString filename =
            QFileDialog::getSaveFileName(this,
                                         tr("Save Spectra Matrix"),
                                         workspace->directory(),
                                         tr("Vespucci Binary (*.arma);;"
                                            "Comma-separated Values (*.csv);;"
                                            "Tab-separated Txt (*.txt);;"));
    QFileInfo file_info(filename);

    if (file_info.suffix() == "arma")
        success = dataset->AverageSpectrum(false).save(filename.toStdString(), arma_binary);
    else if (file_info.suffix() == "csv")
        success = dataset->AverageSpectrum(true).save(filename.toStdString(), csv_ascii);
    else
        success = dataset->AverageSpectrum(true).save(filename.toStdString(), raw_ascii);

    if (success)
        QMessageBox::information(this, "File Saved", "File written successfully!");
    else
        QMessageBox::warning(this, "File Not Saved", "Could not save file.");

}

void MainWindow::on_actionAverage_Spectra_with_Abcissa_triggered()
{
    bool success;
    int row = dataset_list_widget_->currentRow();
    QSharedPointer<SpecMap> dataset = workspace->DatasetAt(row);

    QString filename =
            QFileDialog::getSaveFileName(this,
                                         tr("Save Spectra Matrix"),
                                         workspace->directory(),
                                         tr("Vespucci Binary (*.arma);;"
                                            "Comma-separated Values (*.csv);;"
                                            "Tab-separated Txt (*.txt);;"));
    QFileInfo file_info(filename);

    rowvec wavelength = dataset->wavelength();
    mat output(wavelength);
    output.insert_rows(1, dataset->AverageSpectrum(true));
    output = output.t();

    if (file_info.suffix() == "arma")
        success = output.save(filename.toStdString(), arma_binary);
    else if (file_info.suffix() == "csv")
        success = output.save(filename.toStdString(), csv_ascii);
    else
        success = output.save(filename.toStdString(), raw_ascii);

    if (success)
        QMessageBox::information(this, "File Saved", "File written successfully!");
    else
        QMessageBox::warning(this, "File Not Saved", "Could not save file.");
}

void MainWindow::on_actionSpatial_Data_triggered()
{
    bool success;
    int row = dataset_list_widget_->currentRow();
    QSharedPointer<SpecMap> dataset = workspace->DatasetAt(row);
    mat output(dataset->x().t());
    output.insert_cols(1, dataset->y().t());
    output = output.t();

    QString filename =
            QFileDialog::getSaveFileName(this,
                                         tr("Save Spectra Matrix"),
                                         workspace->directory(),
                                         tr("Vespucci Binary (*.arma);;"
                                            "Comma-separated Values (*.csv);;"
                                            "Tab-separated Txt (*.txt);;"));
    QFileInfo file_info(filename);

    if (file_info.suffix() == "arma")
        success = output.save(filename.toStdString(), arma_binary);
    else if (file_info.suffix() == "csv")
        success = output.save(filename.toStdString(), csv_ascii);
    else
        success = output.save(filename.toStdString(), raw_ascii);

    if (success)
        QMessageBox::information(this, "File Saved", "File written successfully!");
    else
        QMessageBox::warning(this, "File Not Saved", "Could not save file.");
}

void MainWindow::on_actionSpectral_Abcissa_triggered()
{
    bool success;
    int row = dataset_list_widget_->currentRow();
    QSharedPointer<SpecMap> dataset = workspace->DatasetAt(row);

    QString filename =
            QFileDialog::getSaveFileName(this,
                                         tr("Save Spectra Matrix"),
                                         workspace->directory(),
                                         tr("Vespucci Binary (*.arma);;"
                                            "Comma-separated Values (*.csv);;"
                                            "Tab-separated Txt (*.txt);;"));
    QFileInfo file_info(filename);

    if (file_info.suffix() == "arma")
        success = dataset->wavelength().save(filename.toStdString(), arma_binary);
    else if (file_info.suffix() == "csv")
        success = dataset->wavelength().save(filename.toStdString(), csv_ascii);
    else
        success = dataset->wavelength().save(filename.toStdString(), raw_ascii);

    if (success)
        QMessageBox::information(this, "File Saved", "File written successfully!");
    else
        QMessageBox::warning(this, "File Not Saved", "Could not save file.");
}

void MainWindow::on_actionAll_Data_triggered()
{
    bool success;
    int row = dataset_list_widget_->currentRow();
    QSharedPointer<SpecMap> dataset = workspace->DatasetAt(row);
    mat output(dataset->spectra());
    rowvec temp_wavelength(dataset->wavelength());
    mat addendum(1,2);
    addendum.zeros();
    temp_wavelength.insert_cols(0, addendum); //add two 0 entries to wl
    output.insert_cols(0, dataset->y());  //y at 0
    output.insert_cols(0, dataset->x()); //x at 0, y at 1
    output.insert_rows(0, temp_wavelength); //put wl on first line


    QString filename =
            QFileDialog::getSaveFileName(this,
                                         tr("Save Spectra Matrix"),
                                         workspace->directory(),
                                         tr("Vespucci Dataset (*.vds);;"
                                            "Tab-delimited Text (*.txt);;"
                                            "Comma-separated Variables (*.csv);;"));
    QFileInfo file_info(filename);
    if (file_info.suffix() == "vds")
        success = output.save(filename.toStdString(), arma_binary);
    else if (file_info.suffix() == "txt")
        success = output.save(filename.toStdString(), raw_ascii);
    else
        success = output.save(filename.toStdString(), csv_ascii);

    if (success)
        QMessageBox::information(this, "File Saved", "File written successfully!");
    else
        QMessageBox::warning(this, "File Not Saved", "Could not save file.");
}

void MainWindow::on_actionPrincipal_Component_Statistics_triggered()
{/*
    bool success;
    int row = dataset_list_widget_->currentRow();
    QSharedPointer<SpecMap> dataset = workspace->DatasetAt(row);

    if (dataset->principal_components_calculated()){
        QString filename =
                QFileDialog::getSaveFileName(this,
                                             tr("Save Spectra Matrix"),
                                             workspace->directory(),
                                             tr("Comma-separated Variables (*.csv);;"
                                                "Tab-delimited Text (*.txt);;"));
        QFileInfo file_info(filename);
        if (file_info.suffix() == "txt")
            success = dataset->principal_components_data()->tsquared()->save(filename.toStdString(), raw_ascii);
        else
            success = dataset->principal_components_data()->tsquared()->save(filename.toStdString(), csv_ascii);
    }
    */

}

void MainWindow::on_actionFilter_Derivatize_triggered()
{
    int row = dataset_list_widget_->currentRow();
    FilterDialog *filter_dialog = new FilterDialog(this, workspace, row);
    filter_dialog->show();
}

void MainWindow::on_actionClose_Dataset_triggered()
{
    int dataset_row = dataset_list_widget_->currentRow();
    QString name = dataset_list_widget_->currentItem()->text();
    QString text = "Are you sure you want to close the dataset " + name + "?" +
            " The data and all associated maps will be deleted.";

    int response = QMessageBox::question(this, "Close Dataset?", text,
                                         QMessageBox::Ok, QMessageBox::Cancel);

    if (response == QMessageBox::Ok)
        workspace->RemoveDatasetAt(dataset_row);
}

void MainWindow::on_actionDocumentation_triggered()
{
    QUrl website_url("http://dpfoose.github.io/Vespucci/");
    QDesktopServices::openUrl(website_url);
}

void MainWindow::on_actionCrop_triggered()
{
    int row = dataset_list_widget_->currentRow();
    CropDialog *crop_dialog = new CropDialog(this, workspace, row);
    crop_dialog->show();
}

void MainWindow::on_actionCorrect_Baseline_triggered()
{
    int row = dataset_list_widget_->currentRow();
    BaselineDialog *baseline_dialog = new BaselineDialog(this, workspace, row);
    baseline_dialog->show();
}

void MainWindow::on_actionView_Dataset_Elements_triggered()
{
    int row = dataset_list_widget_->currentRow();
    DataViewer *data_viewer = new DataViewer(0, workspace, row);
    data_viewer->show();
}
