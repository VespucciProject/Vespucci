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
#include "GUI/mainwindow.h"
#include "ui_mainwindow.h"
#include "GUI/Processing/loaddataset.h"
#include "GUI/Display/aboutdialog.h"
#include "GUI/Display/citationdialog.h"
#include "GUI/Analysis/univariatedialog.h"
#include "GUI/Analysis/bandratiodialog.h"
#include "GUI/Analysis/principalcomponentsdialog.h"
#include "GUI/Processing/filterdialog.h"
#include "GUI/Processing/cropdialog.h"
#include "GUI/Processing/baselinedialog.h"
#include "GUI/Display/dataviewer.h"
#include "GUI/Analysis/plsdialog.h"
#include "GUI/Analysis/kmeansdialog.h"
#include "GUI/Analysis/vcadialog.h"
#include "GUI/Processing/metadatasetdialog.h"
#include "GUI/Processing/rangedialog.h"
#include "GUI/Display/spectrumselectiondialog.h"
#include "GUI/Processing/booleanizedialog.h"
#include "GUI/Processing/thresholddialog.h"
#include "GUI/scriptdialog.h"
#include "GUI/Analysis/peakfindingdialog.h"
#include "GUI/Analysis/haspeaksdialog.h"
#include "GUI/Processing/multiimportdialog.h"
#include "GUI/Processing/bulkconversiondialog.h"
#include "GUI/Analysis/classicalleastsquaresdialog.h"
#include "GUI/settingsdialog.h"
#include "GUI/Processing/abscissatransformdialog.h"
#include "GUI/Processing/fouriertransformdialog.h"
#include "GUI/Processing/abscissainterpolationdialog.h"
#include "GUI/Processing/stitchimportdialog.h"
#include "GUI/Analysis/plotmakerdialog.h"
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
    dataset_tree_view_ = findChild<QTreeView *>("datasetTreeView");
    dataset_tree_model_ = new DatasetTreeModel(0, workspace);
    workspace->SetListWidgetModel(dataset_tree_model_);
    dataset_tree_view_->setModel(dataset_tree_model_);
    data_viewer_ = new DataViewer(this);
    global_map_count_ = 0;
    QObject::connect(dataset_tree_view_, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(TreeItemDoubleClicked(QModelIndex)));
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
        dataset_tree_model_->ClearDatasets();
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

    if (response == QMessageBox::Yes) {
        dataset_tree_model_->ClearDatasets();
        qApp->exit();
    }
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
    if (!workspace->datasets()->size()){
        QMessageBox::information(this,
                                 "No datasets loaded",
                                 "No dataset exists on which to perform this operation");
        return;
    }
    QModelIndex index = dataset_tree_view_->currentIndex();
    if (dataset_tree_model_->IsDataset(index)){
        UnivariateDialog *univariate_dialog =
                new UnivariateDialog(this, workspace, index);
        univariate_dialog->show();
    }
}

///
/// \brief MainWindow::on_actionNew_Band_Ratio_Map_triggered
///Triggers band ratio dialog.
void MainWindow::on_actionNew_Band_Ratio_Map_triggered()
{
    if (!workspace->datasets()->size()){
        QMessageBox::information(this,
                                 "No datasets loaded",
                                 "No dataset exists on which to perform this operation");
        return;
    }
    QModelIndex index = dataset_tree_view_->currentIndex();
    if (dataset_tree_model_->IsDataset(index)){
        BandRatioDialog *band_ratio_dialog = new BandRatioDialog(this, workspace, index);
        band_ratio_dialog->show();
    }

}

///
/// \brief MainWindow::on_actionPrincipal_Components_Analysis_triggered
///Triggers principal components dialog
void MainWindow::on_actionPrincipal_Components_Analysis_triggered()
{
    if (!workspace->datasets()->size()){
        QMessageBox::information(this,
                                 "No datasets loaded",
                                 "No dataset exists on which to perform this operation");
        return;
    }
    QModelIndex index = dataset_tree_view_->currentIndex();
    if (dataset_tree_model_->IsDataset(index)){
        PrincipalComponentsDialog *principal_components_dialog =
                new PrincipalComponentsDialog(this, workspace, index);
        principal_components_dialog->show();
    }

}


///
/// \brief MainWindow::on_actionVertex_Components_triggered
///Triggers vertex components dialog
void MainWindow::on_actionVertex_Components_triggered()
{
    if (!workspace->datasets()->size()){
        QMessageBox::information(this,
                                 "No datasets loaded",
                                 "No dataset exists on which to perform this operation");
        return;
    }
    QModelIndex index = dataset_tree_view_->currentIndex();
    if (dataset_tree_model_->IsDataset(index)){
        VCADialog *vca_dialog = new VCADialog(this, workspace, index);
        vca_dialog->show();
    }

}


///
/// \brief MainWindow::on_actionPartial_Least_Squares_triggered
/// Triggers partial least squares dialog
///
void MainWindow::on_actionPartial_Least_Squares_triggered()
{
    if (!workspace->datasets()->size()){
        QMessageBox::information(this,
                                 "No datasets loaded",
                                 "No dataset exists on which to perform this operation");
        return;
    }
    QModelIndex index = dataset_tree_view_->currentIndex();
    if (dataset_tree_model_->IsDataset(index)){
        PLSDialog *pls_dialog =
                new PLSDialog(this, workspace, index);
        pls_dialog->show();
    }

}

///
/// \brief MainWindow::on_actionK_Means_Clustering_triggered
/// Triggers K-Means dialog
///
void MainWindow::on_actionK_Means_Clustering_triggered()
{
    if (!workspace->datasets()->size()){
        QMessageBox::information(this,
                                 "No datasets loaded",
                                 "No dataset exists on which to perform this operation");
        return;
    }
    QModelIndex index = dataset_tree_view_->currentIndex();
    if (dataset_tree_model_->IsDataset(index)){
        KMeansDialog *k_means_dialog =
                new KMeansDialog(this, workspace, index);
        k_means_dialog->show();
    }


}


///
/// \brief MainWindow::on_actionNormalize_Standardize_triggered
///
void MainWindow::on_actionNormalize_Standardize_triggered()
{
    if (!workspace->datasets()->size()){
        QMessageBox::information(this,
                                 "No datasets loaded",
                                 "No dataset exists on which to perform this operation");
        return;
    }
    QModelIndex index = dataset_tree_view_->currentIndex();
    QSharedPointer<VespucciDataset> data = workspace->DatasetAt(index);
    QStringList methods;
    methods << "Min/Max" << "Vector Norm" << "Unit Area" << "Z-score"
            << "Standard Normal Variate" << "Peak Intensity" << "Scale Spectra"
            << "Absolute Value" << "Mean Center";
    bool ok;
    QString item = QInputDialog::getItem(this,
                                         tr("Normalization/Standardization"),
                                         tr("Method:"), methods, 0, false, &ok);
    double scaling_factor, offset;
    try{
        if (ok && item == "Min/Max"){data->MinMaxNormalize();}
        else if (ok && item == "Vector Norm"){data->VectorNormalize();}
        else if (ok && item == "Mean Center"){data->MeanCenter();}
        else if (ok && item == "Unit Area"){data->UnitAreaNormalize();}
        else if (ok && item == "Z-score"){
            offset = QInputDialog::getDouble(this, "Enter Offset", "Offset",
                                             0, -2147483647, 2147483647, 4, &ok);
            data->SNVNormalize(offset, true);
        }
        else if (ok && item == "Peak Intensity"){
            double min = data->wavelength_ptr()->min();
            double max = data->wavelength_ptr()->max();
            RangeDialog *range_dialog = new RangeDialog(this, min, max);
            range_dialog->setWindowTitle("Peak Intensity Normalization");
            QObject::connect(range_dialog, SIGNAL(DialogAccepted(double,double)), this, SLOT(RangeDialogAccepted(double,double)));
            range_dialog->show();
        }
        else if (ok && item == "Scale Spectra"){
            scaling_factor = QInputDialog::getDouble(this, "Enter Scaling Factor",
                                                     "Factor", 1, -100, 100, 2, &ok);
            data->Scale(scaling_factor);
        }
        else if (ok && item == "Standard Normal Variate"){
            offset = QInputDialog::getDouble(this, "Enter Offset", "Offset",
                                             0, -2147483647, 2147483647, 4, &ok);
            data->SNVNormalize(offset, false);
        }
        else if (ok && item == "Absolute Value"){data->AbsoluteValue();}
        else
            return;
    }
    catch(exception e){
        DisplayExceptionWarning(e);
    }
}

///
/// \brief MainWindow::on_actionSubtract_Background_triggered
///Subtracts a background matrix (a saved armadillo binary matrix) from spectra
void MainWindow::on_actionSubtract_Background_triggered()
{
    if (!workspace->datasets()->size()){
        QMessageBox::information(this,
                                 "No datasets loaded",
                                 "No dataset exists on which to perform this operation");
        return;
    }
    QModelIndex index = dataset_tree_view_->currentIndex();
    QSharedPointer<VespucciDataset> data = workspace->DatasetAt(index);
    QString filename =
            QFileDialog::getOpenFileName(this,
                                         tr("Select Background File"),
                                         workspace->directory(),
                                         "Vespucci Spectrum Files (*.arma *.txt *.csv)");
    mat input;
    QFileInfo file_info(filename);
    workspace->set_directory(file_info.dir().path());
    bool success = input.load(filename.toStdString());
    //We take spectra inputs as row-major
    if (input.n_rows < input.n_cols){
        try{
            input = input.t();
        }catch(exception e){
            DisplayExceptionWarning(e);
        }
    }
    //The first column is probably an abscissa.
    if (input.n_cols > 2){
        success = false;
    }
    if (input.n_cols == 2){
        try{
            input.shed_col(0);
        }catch(exception e){
            DisplayExceptionWarning(e);
        }
    }
    if (!success){
        QMessageBox::warning(this, "File Open Error", "File cannot be opened or contains improper data.");
        return;
    }
    else{
        try{
            data->SubtractBackground(input, filename);
        }
        catch(exception e){
            DisplayExceptionWarning(e);
        }
    }
}

///
/// \brief MainWindow::on_actionSpectra_triggered
///Saves the spectra matrix of selected dataset as binary, csv, or raw ascii.
void MainWindow::on_actionSpectra_triggered()
{
    if (!workspace->datasets()->size()){
        QMessageBox::information(this,
                                 "No datasets loaded",
                                 "No dataset exists on which to perform this operation");
        return;
    }

    bool success;
    QModelIndex index = dataset_tree_view_->currentIndex();
    QSharedPointer<VespucciDataset> dataset = workspace->DatasetAt(index);
    QString filename =
            QFileDialog::getSaveFileName(this,
                                         tr("Save Spectra Matrix"),
                                         workspace->directory(),
                                         tr("Vespucci Binary (*.arma);;"
                                            "Comma-separated Values (*.csv);;"
                                            "Tab-separated Txt (*.txt);;"));
    QFileInfo file_info(filename);
    workspace->set_directory(file_info.dir().path());

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

///
/// \brief MainWindow::on_actionSpectra_as_Columns_triggered
/// Saves a transpose of spectra_
void MainWindow::on_actionSpectra_as_Columns_triggered()
{
    if (!workspace->datasets()->size()){
        QMessageBox::information(this,
                                 "No datasets loaded",
                                 "No dataset exists on which to perform this operation");
        return;
    }
    bool success;
    QModelIndex index = dataset_tree_view_->currentIndex();
    QSharedPointer<VespucciDataset> dataset = workspace->DatasetAt(index);

    QString filename =
            QFileDialog::getSaveFileName(this,
                                         tr("Save Spectra Matrix"),
                                         workspace->directory(),
                                         tr("Vespucci Binary (*.arma);;"
                                            "Comma-separated Values (*.csv);;"
                                            "Tab-separated Txt (*.txt);;"));
    QFileInfo file_info(filename);
    workspace->set_directory(file_info.dir().path());
    mat output = dataset->spectra().t();

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

///
/// \brief MainWindow::on_actionAverage_Spectra_triggered
/// Saves an average spectrum of the selected dataset
void MainWindow::on_actionAverage_Spectra_triggered()
{
    if (!workspace->datasets()->size()){
        QMessageBox::information(this,
                                 "No datasets loaded",
                                 "No dataset exists on which to perform this operation");
        return;
    }
    bool success;
    QModelIndex index = dataset_tree_view_->currentIndex();
    QSharedPointer<VespucciDataset> dataset = workspace->DatasetAt(index);

    QString filename =
            QFileDialog::getSaveFileName(this,
                                         tr("Save Average Spectrum"),
                                         workspace->directory(),
                                         tr("Vespucci Binary (*.arma);;"
                                            "Comma-separated Values (*.csv);;"
                                            "Tab-separated Txt (*.txt);;"));
    QFileInfo file_info(filename);
    workspace->set_directory(file_info.dir().path());

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

///
/// \brief MainWindow::on_actionAverage_Spectra_with_Abscissa_triggered
/// Saves average spectrum (with abscissa above).
void MainWindow::on_actionAverage_Spectra_with_Abscissa_triggered()
{
    if (!workspace->datasets()->size()){
        QMessageBox::information(this,
                                 "No datasets loaded",
                                 "No dataset exists on which to perform this operation");
        return;
    }
    bool success;
    QModelIndex index = dataset_tree_view_->currentIndex();
    QSharedPointer<VespucciDataset> dataset = workspace->DatasetAt(index);

    QString filename =
            QFileDialog::getSaveFileName(this,
                                         tr("Save Spectra Matrix"),
                                         workspace->directory(),
                                         tr("Vespucci Binary (*.arma);;"
                                            "Comma-separated Values (*.csv);;"
                                            "Tab-separated Txt (*.txt);;"));
    QFileInfo file_info(filename);
    workspace->set_directory(file_info.dir().path());


    vec wavelength = dataset->wavelength();
    mat output(wavelength);
    try{
        output.insert_cols(1, dataset->AverageSpectrum(true));
    }
    catch(exception e){
        DisplayExceptionWarning(e);
    }

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

///
/// \brief MainWindow::on_actionSpectral_Abscissa_triggered
/// Saves the spectral abscissa
void MainWindow::on_actionSpectral_Abscissa_triggered()
{
    if (!workspace->datasets()->size()){
        QMessageBox::information(this,
                                 "No datasets loaded",
                                 "No dataset exists on which to perform this operation");
        return;
    }
    bool success;
    QModelIndex index = dataset_tree_view_->currentIndex();
    QSharedPointer<VespucciDataset> dataset = workspace->DatasetAt(index);

    QString filename =
            QFileDialog::getSaveFileName(this,
                                         tr("Save Spectra Matrix"),
                                         workspace->directory(),
                                         tr("Vespucci Binary (*.arma);;"
                                            "Comma-separated Values (*.csv);;"
                                            "Tab-separated Txt (*.txt);;"));
    QFileInfo file_info(filename);
    workspace->set_directory(file_info.dir().path());


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

///
/// \brief MainWindow::on_actionAll_Data_triggered
/// Saves a matrix containing all spatial and spectral data
void MainWindow::on_actionAll_Data_triggered()
{
    if (!workspace->datasets()->size()){
        QMessageBox::information(this,
                                 "No datasets loaded",
                                 "No dataset exists on which to perform this operation");
        return;
    }
    if (dataset_tree_view_->model()->rowCount() < 1)
        return;
    bool success;
    QModelIndex index = dataset_tree_view_->currentIndex();
    QSharedPointer<VespucciDataset> dataset = workspace->DatasetAt(index);

    QString filename =
            QFileDialog::getSaveFileName(this,
                                         tr("Save Spectra Matrix"),
                                         workspace->directory(),
                                         tr("Vespucci Dataset (*.vds)"));

    QFileInfo file_info(filename);
    workspace->set_directory(file_info.dir().path());

    //exception can be thrown when intializing field
    try{
        success = dataset->Save(filename);
    }
    catch(exception e){
        DisplayExceptionWarning(e);
        success = false;
    }

    if (success)
        QMessageBox::information(this, "File Saved", "File written successfully!");
    else
        QMessageBox::warning(this, "File Not Saved", "Could not save file.");
}

///
/// \brief MainWindow::on_actionFilter_Derivatize_triggered
/// Triggers dialog to filter data
void MainWindow::on_actionFilter_Derivatize_triggered()
{
    if (!workspace->datasets()->size()){
        QMessageBox::information(this,
                                 "No datasets loaded",
                                 "No dataset exists on which to perform this operation");
        return;
    }
    QModelIndex index = dataset_tree_view_->currentIndex();
    FilterDialog *filter_dialog = new FilterDialog(this, workspace, index);
    filter_dialog->show();
}

///
/// \brief MainWindow::on_actionClose_Dataset_triggered
/// Closes the dataset. Should force it to go out of scope
void MainWindow::on_actionClose_Dataset_triggered()
{
    if (!workspace->datasets()->size()){
        QMessageBox::information(this,
                                 "No datasets loaded",
                                 "No dataset exists on which to perform this operation");
        return;
    }
    QModelIndex index = dataset_tree_view_->currentIndex();
    QString name = dataset_tree_view_->currentIndex().data().value<QString>();
    QString text = "Are you sure you want to close the dataset " + name + "?" +
            " The data and all associated maps will be deleted.";

    int response = QMessageBox::question(this, "Close Dataset?", text,
                                         QMessageBox::Ok, QMessageBox::Cancel);

    if (response == QMessageBox::Ok)
        dataset_tree_model_->removeRow(index);
}

///
/// \brief MainWindow::on_actionDocumentation_triggered
/// Triggers the window that takes you to the website
void MainWindow::on_actionDocumentation_triggered()
{
    QUrl website_url("http://dpfoose.github.io/Vespucci/");
    QDesktopServices::openUrl(website_url);
}

///
/// \brief MainWindow::on_actionCrop_triggered
/// Triggers the CropDialog
void MainWindow::on_actionCrop_triggered()
{    if (!workspace->datasets()->size()){
        QMessageBox::information(this,
                                 "No datasets loaded",
                                 "No dataset exists on which to perform this operation");
        return;
    }
    QModelIndex index = dataset_tree_view_->currentIndex();
    CropDialog *crop_dialog = new CropDialog(this, workspace, index);
    crop_dialog->show();
}

///
/// \brief MainWindow::on_actionCorrect_Baseline_triggered
/// Triggers Baseline correction dialog
void MainWindow::on_actionCorrect_Baseline_triggered()
{
    if (!workspace->datasets()->size()){
        QMessageBox::information(this,
                                 "No datasets loaded",
                                 "No dataset exists on which to perform this operation");
        return;
    }
    QModelIndex index = dataset_tree_view_->currentIndex();
    BaselineDialog *baseline_dialog = new BaselineDialog(this, workspace, index);
    baseline_dialog->show();
}

///
/// \brief MainWindow::on_actionView_Dataset_Elements_triggered
/// Triggers DataViewer
void MainWindow::on_actionView_Dataset_Elements_triggered()
{
    if (!workspace->datasets()->size()){
        QMessageBox::information(this,
                                 "No datasets loaded",
                                 "No dataset exists on which to perform this operation");
        return;
    }
    QModelIndex index = dataset_tree_view_->currentIndex();
    //DataViewer *data_viewer = new DataViewer(0, workspace, index);
    //data_viewer->show();
}

///
/// \brief MainWindow::on_actionSet_Global_Color_Scale_triggered
/// Set the global color scale.
void MainWindow::on_actionSet_Global_Color_Scale_triggered()
{
    //populate a list
    QStringList color_list;
    color_list  << "ColorBrewerBlueGreen"
                << "ColorBrewerBluePurple"
                << "ColorBrewerGreenBlue"
                << "ColorBrewerOrangeRed"
                << "ColorBrewerPurpleBlue"
                << "ColorBrewerPurpleBlueGreen"
                << "ColorBrewerPurpleRed"
                << "ColorBrewerRedPurple"
                << "ColorBrewerYellowGreen"
                << "ColorBrewerYellowGreenBlue"
                << "ColorBrewerYellowOrangeBrown"
                << "ColorBrewerYellowOrangeRed"
                << "ColorBrewerBlues"
                << "ColorBrewerGreens"
                << "ColorBrewerOranges"
                << "ColorBrewerPurples"
                << "ColorBrewerReds"
                << "ColorBrewerGrayscale"
                << "QCustomPlotGrayscale"
                << "QCustomPlotNight"
                << "QCustomPlotCandy"
                << "QCustomPlotIon"
                << "QCustomPlotThermal"
                << "↔QCustomPlotPolar"
                << "↔QCustomPlotSpectrum"
                << "QCustomPlotJet"
                << "QCustomPlotHues"
                << "QCustomPlotHot"
                << "QCustomPlotCold"
                << "↔ColorBrewerBrownBlueGreen"
                << "↔ColorBrewerPinkYellowGreen"
                << "↔ColorBrewerPurpleGreen"
                << "↔ColorBrewerPurpleOrange"
                << "↔ColorBrewerRedBlue"
                << "↔ColorBrewerRedGray"
                << "↔ColorBrewerRedYellowBlue"
                << "↔ColorBrewerRedYellowGreen"
                << "↔ColorBrewerSpectral"
                << "↔VespucciSpectral";
    QString color_name = QInputDialog::getItem(this, "Select Color Scheme", "Choose Scheme", color_list);
    int color_index = color_list.indexOf(color_name);
    QCPColorGradient gradient;
    switch (color_index)
        {
        case 0: gradient =  QCPColorGradient::cbBuGn; break;
        case 1: gradient =  QCPColorGradient::cbBuPu; break;
        case 2: gradient =  QCPColorGradient::cbGnBu; break;
        case 3: gradient =  QCPColorGradient::cbOrRd; break;
        case 4: gradient =  QCPColorGradient::cbPuBu; break;
        case 5: gradient =  QCPColorGradient::cbPuBuGn; break;
        case 6: gradient =  QCPColorGradient::cbPuRd; break;
        case 7: gradient =  QCPColorGradient::cbRdPu; break;
        case 8: gradient =  QCPColorGradient::cbYlGn; break;
        case 9: gradient =  QCPColorGradient::cbYlGnBu; break;
        case 10: gradient =  QCPColorGradient::cbYlOrBr; break;
        case 11: gradient =  QCPColorGradient::cbYlOrRd; break;
        case 12: gradient =  QCPColorGradient::cbBlues; break;
        case 13: gradient =  QCPColorGradient::cbGreens; break;
        case 14: gradient =  QCPColorGradient::cbOranges; break;
        case 15: gradient =  QCPColorGradient::cbPurples; break;
        case 16: gradient =  QCPColorGradient::cbReds; break;
        case 17: gradient =  QCPColorGradient::cbGreys; break;
        case 18: gradient =  QCPColorGradient::gpGrayscale; break;
        case 19: gradient =  QCPColorGradient::gpNight; break;
        case 20: gradient =  QCPColorGradient::gpCandy; break;
        case 21: gradient =  QCPColorGradient::gpIon; break;
        case 22: gradient =  QCPColorGradient::gpThermal; break;
        case 23: gradient =  QCPColorGradient::gpPolar; break;
        case 24: gradient =  QCPColorGradient::gpSpectrum; break;
        case 25: gradient =  QCPColorGradient::gpJet; break;
        case 26: gradient =  QCPColorGradient::gpHues; break;
        case 27: gradient =  QCPColorGradient::gpHot; break;
        case 28: gradient =  QCPColorGradient::gpCold; break;
        case 29: gradient =  QCPColorGradient::cbBrBG; break;
        case 30: gradient =  QCPColorGradient::cbPiYG; break;
        case 31: gradient =  QCPColorGradient::cbPRGn; break;
        case 32: gradient =  QCPColorGradient::cbPuOr; break;
        case 33: gradient =  QCPColorGradient::cbRdBu; break;
        case 34: gradient =  QCPColorGradient::cbRdGy; break;
        case 35: gradient =  QCPColorGradient::cbRdYlBu; break;
        case 36: gradient =  QCPColorGradient::cbRdYlGn; break;
        case 37: gradient =  QCPColorGradient::cbSpectral; break;
        case 38: gradient =  QCPColorGradient::vSpectral; break;
        default: gradient =  QCPColorGradient::gpCold;
        }
    workspace->RefreshGlobalColorGradient(gradient);
}

///
/// \brief MainWindow::global_data_range
/// \return
/// Return the global data range (to MapViewer widgets)
QCPRange* MainWindow::global_data_range()
{
    return workspace->global_data_range();
}

///
/// \brief MainWindow::global_gradient
/// \return
/// Return the global color gradient (to MapViewer widgets)
QCPColorGradient* MainWindow::global_gradient()
{
    return workspace->global_gradient();
}

///
/// \brief MainWindow::RecalculateGlobalDataRange
/// \param new_data_range
/// Recalculate the global data range based on previous range and the new data
void MainWindow::RecalculateGlobalDataRange(QCPRange* new_data_range)
{
    bool changed = false;
    if (global_map_count_ <= 1)
        workspace->SetGlobalDataRange(new_data_range);
    else
        changed = workspace->RecalculateGlobalDataRange(new_data_range);

    if (changed){
        emit GlobalDataRangeChanged(*new_data_range);
    }
}

///
/// \brief MainWindow::RefreshGlobalColorGradient
/// \param new_gradient
/// Change the global color gradient and update it for all objects using it
void MainWindow::RefreshGlobalColorGradient(QCPColorGradient new_gradient)
{
    workspace->RefreshGlobalColorGradient(new_gradient);
    emit GlobalGradientChanged(new_gradient);
}

///
/// \brief MainWindow::SetGlobalDataRange
/// \param new_data_range
/// Set the global data range and update it for all objects using it
void MainWindow::SetGlobalDataRange(QCPRange* new_data_range)
{
    workspace->SetGlobalDataRange(new_data_range);
    emit GlobalDataRangeChanged(*new_data_range);
}

///
/// \brief MainWindow::workspace_ptr
/// \return
/// Very kludgy way of getting the workspace variable to window variables.
VespucciWorkspace* MainWindow::workspace_ptr()
{
    return workspace;
}

void MainWindow::on_actionUndo_triggered()
{
    if (!workspace->datasets()->size()){
        QMessageBox::information(this,
                                 "No datasets loaded",
                                 "No dataset exists on which to perform this operation");
        return;
    }
    QModelIndex index = dataset_tree_view_->currentIndex();
    QSharedPointer<VespucciDataset> dataset = workspace->DatasetAt(index);

    if (!dataset->Undoable()){
        dataset.clear();
        QMessageBox::information(this, "Nothing to Undo", "There are no actions to undo");
        return;
    }

    QString text = tr("Are you sure you want to undo ") + dataset->last_operation()
            + " on " + dataset->name() + "?";
    int response =
            QMessageBox::question(this,
                          tr("Undo Operation"),
                          text,
                          QMessageBox::Ok | QMessageBox::Cancel);

    if (response == QMessageBox::Ok){
        dataset->Undo();
        return;
    }
    else{
        return;
    }
    dataset.clear(); //should fall out of scope anyway, but let's be safe

}

///
/// \brief MainWindow::DisplayExceptionWarning
/// \param e
/// Used to handle exceptions arising from dialogs. Displays a pop-up box describing
/// the nature of the error. Most exceptions are not critical, but will result in the
/// action that caused the exception being canceled.
void MainWindow::DisplayExceptionWarning(std::exception e)
{
    string str = "The following exception was thrown: " + string(e.what());
    QMessageBox::warning(this, "Exception Occurred", QString::fromStdString(str));
}

void MainWindow::DisplayExceptionWarning(string where, exception e)
{
    string str = "The following exception was thrown: "
            + string(e.what())
            + ". In the internal function: "
            + where
            + ".";
    QMessageBox::warning(this, "Exception Occurred", QString::fromStdString(str));
}

void MainWindow::on_datasetsListView_clicked(const QModelIndex &index)
{
    if (!workspace->datasets()->size()){
        QMessageBox::information(this,
                                 "No datasets loaded",
                                 "No dataset exists on which to perform this operation");
        return;
    }

    QSharedPointer<VespucciDataset> dataset =
            dataset_tree_model_->DatasetAt(index);

}

///
/// \brief MainWindow::SetActiveDatasetListRow
/// \param row
/// Change the row that is active in the dataset list view
void MainWindow::SetActiveDatasetListRow(const QModelIndex &index)
{
    if (!workspace->datasets()->size()){
        QMessageBox::information(this,
                                 "No datasets loaded",
                                 "No dataset exists on which to perform this operation");
        return;
    }
    QSharedPointer<VespucciDataset> dataset =
            dataset_tree_model_->DatasetAt(index);

}


void MainWindow::DisplayWarning(const QString &title, const QString &text)
{
    QMessageBox::warning(this, title, text);
}

void MainWindow::DisplayInformation(const QString &title, const QString &text)
{
    QMessageBox::information(this, title, text);
}

QStringList MainWindow::DatasetNames() const
{
    return workspace->dataset_names();
}


///
/// \brief MainWindow::DatasetAdded
/// \param index
/// Slot corresponding to the signal from the list model that a dataset has been added.
void MainWindow::DatasetAdded(const QModelIndex &index)
{
    if (dataset_tree_view_->model()->rowCount() < 1)
        return;
    QSharedPointer<VespucciDataset> dataset =
            dataset_tree_model_->DatasetAt(index);

    map_list_view_->setCurrentIndex(index);
    ++global_map_count_;
}

///
/// \brief MainWindow::on_mapsListView_doubleClicked
/// \param index
/// Opens or closes a map when it is double clicked.
void MainWindow::on_mapsListView_doubleClicked(const QModelIndex &index)
{/*
    if (!workspace->datasets()->size()){
        QMessageBox::information(this,
                                 "No datasets loaded",
                                 "No dataset exists on which to perform this operation");
        return;
    }
    MapListModel *map_list_model = qobject_cast<MapListModel*>(map_list_view_->model());
    QSharedPointer<MapData> map_data = map_list_model->MapAt(index);
    if (map_data->MapWindowVisible())
        map_data->HideMapWindow();
    else
        map_data->ShowMapWindow();
*/
}

void MainWindow::on_actionDelete_Map_triggered()
{
    /*
    if (!workspace->datasets()->size()){
        QMessageBox::information(this,
                                 "No datasets loaded",
                                 "No dataset exists on which to perform this operation");
        return;
    }
    MapListModel *map_list_model = qobject_cast<MapListModel*>(map_list_view_->model());
    QModelIndex index = map_list_view_->currentIndex();
    map_list_model->removeRow(index(), index);
    */
}

void MainWindow::on_actionNew_Composite_Dataset_triggered()
{
    if (!workspace->datasets()->size()){
        QMessageBox::information(this,
                                 "No datasets loaded",
                                 "No dataset exists on which to perform this operation");
        return;
    }
    MetaDatasetDialog *meta_dialog = new MetaDatasetDialog(this, workspace);
    meta_dialog->show();
}



void MainWindow::on_actionReject_Clipped_Spectra_triggered()
{
    if (!workspace->datasets()->size()){
        QMessageBox::information(this,
                                 "No datasets loaded",
                                 "No dataset exists on which to perform this operation");
        return;
    }
    ThresholdDialog *dialog = new ThresholdDialog(this, workspace, dataset_tree_view_->currentIndex());
    dialog->show();
}


void MainWindow::RangeDialogAccepted(double min, double max)
{
    if (dataset_tree_view_->model()->rowCount() < 1)
        return;
    QModelIndex index = dataset_tree_view_->currentIndex();
    QSharedPointer<VespucciDataset> data = workspace->DatasetAt(index);
    try{
        data->PeakIntensityNormalize(min, max);
    }
    catch(exception e){
        DisplayExceptionWarning(e);
    }
}

void MainWindow::TreeItemDoubleClicked(const QModelIndex &index)
{
    //if item represents a displayable (color map or matrix), then trigger its display
    //nothing stops the user from having a matrix open in multiple tabs in the DataViewer
    //If they want to not display the data, they can close the tab
    if (dataset_tree_model_->IsMatrix(index)){
        MatrixTreeItem *item = dynamic_cast<MatrixTreeItem *>(dataset_tree_model_->getItem(index));
        data_viewer_->AddTab(item->value(), item->data(0).toString());
    }
    if (dataset_tree_model_->IsMap(index)){
        ImageTreeItem *item = dynamic_cast<ImageTreeItem *>(dataset_tree_model_->getItem(index));
        item->ToggleMapViewerVisible();
    }
}

void MainWindow::on_actionView_Edit_Spectra_triggered()
{
    if (!workspace->datasets()->size()){
        QMessageBox::information(this,
                                 "No datasets loaded",
                                 "No dataset exists on which to perform this operation");
        return;
    }
    QSharedPointer<VespucciDataset> data =
            workspace->DatasetAt(dataset_tree_view_->currentIndex());
    SpectrumSelectionDialog *spectrum_selector =
            new SpectrumSelectionDialog(0, this, data);
    spectrum_selector->show();
}

void MainWindow::on_actionBooleanize_Clamp_triggered()
{
    if (!workspace->datasets()->size()){
        QMessageBox::information(this,
                                 "No datasets loaded",
                                 "No dataset exists on which to perform this operation");
        return;
    }
    BooleanizeDialog *booleanize_dialog = new BooleanizeDialog(this, workspace, dataset_tree_view_->currentIndex());
    booleanize_dialog->show();
}

void MainWindow::on_actionRemove_Vectors_of_Zeros_triggered()
{
    if (!workspace->datasets()->size()){
        QMessageBox::information(this,
                                 "No datasets loaded",
                                 "No dataset exists on which to perform this operation");
        return;
    }
    QStringList items;
    bool ok;
    items << "Remove columns of zeros (removes spectra)" << "Remove rows of zeros (removes wavelengths)";
    QString input = QInputDialog::getItem(this, "Select dimension", "Behavior", items, 0, false, &ok);
    if(!ok){
        return;
    }
    else{
        QSharedPointer<VespucciDataset> data = dataset_tree_model_->DatasetAt(dataset_tree_view_->currentIndex());
        if (input == "Remove columns of zeros (removes spectra)"){
            try{
                data->ShedZeroSpectra();
            }
            catch(std::exception e){
                DisplayExceptionWarning(e);
            }
        }
        else if (input == "Remove rows of zeros (removes wavelengths)"){
            try{
                data->ShedZeroWavelengths();
            }
            catch(std::exception e){
                DisplayExceptionWarning(e);
            }
        }
        else{
            QMessageBox::warning(this, "Invalid Entry", "Invalid response to QInputDialog::getItem()");
        }
        data.clear();
    }
}

void MainWindow::on_actionRun_script_triggered()
{
    if (!workspace->datasets()->size()){
        QMessageBox::information(this,
                                 "No datasets loaded",
                                 "No dataset exists on which to perform this operation");
        return;
    }
    QModelIndex index = dataset_tree_view_->currentIndex();
    ScriptDialog *script_dialog = new ScriptDialog(this, workspace, index);
    script_dialog->show();
}

void MainWindow::on_actionDetect_Peaks_triggered()
{
    if (!workspace->datasets()->size()){
        QMessageBox::information(this,
                                 "No datasets loaded",
                                 "No dataset exists on which to perform this operation");
        return;
    }
    QModelIndex index = dataset_tree_view_->currentIndex();
    PeakFindingDialog *peak_dialog = new PeakFindingDialog(this, workspace, index);
    peak_dialog->show();
}

void MainWindow::on_actionCalculate_Peak_Populations_triggered()
{
    if (!workspace->datasets()->size()){
        QMessageBox::information(this,
                                 "No datasets loaded",
                                 "No dataset exists on which to perform this operation");
        return;
    }
    QModelIndex index = dataset_tree_view_->currentIndex();
    HasPeaksDialog *peaks_dialog = new HasPeaksDialog(this, workspace, index);
    peaks_dialog->show();
}

void MainWindow::on_actionImport_From_Multiple_Point_Spectra_triggered()
{
    MultiImportDialog *import_dialog = new MultiImportDialog(this, workspace);
    import_dialog->show();
}


void MainWindow::on_actionBatch_File_Conversion_triggered()
{
    BulkConversionDialog *conversion_dialog = new BulkConversionDialog(this, workspace);
    conversion_dialog->show();
}

void MainWindow::on_actionClassical_Least_Squares_triggered()
{
    if (!workspace->datasets()->size()){
        QMessageBox::information(this,
                                 "No datasets loaded",
                                 "No dataset exists on which to perform this operation");
        return;
    }
    QModelIndex index = dataset_tree_view_->currentIndex();
    ClassicalLeastSquaresDialog *cls_dialog = new ClassicalLeastSquaresDialog(this, workspace, index);
    cls_dialog->show();
}

void MainWindow::on_actionSettings_triggered()
{
    SettingsDialog *settings_dialog = new SettingsDialog(this, workspace);
    settings_dialog->show();
}

void MainWindow::on_actionTransform_Abscissa_triggered()
{
    if (!workspace->datasets()->size()){
        QMessageBox::information(this,
                                 "No datasets loaded",
                                 "No dataset exists on which to perform this operation");
        return;
    }
    QModelIndex index = dataset_tree_view_->currentIndex();
    AbscissaTransformDialog *abscissa_transform_dialog = new AbscissaTransformDialog(this, workspace, index);
    abscissa_transform_dialog->show();
}

void MainWindow::on_actionFourierTransform_triggered()
{
    if (!workspace->datasets()->size()){
        QMessageBox::information(this,
                                 "No datasets loaded",
                                 "No dataset exists on which to perform this operation");
        return;
    }
    QModelIndex index = dataset_tree_view_->currentIndex();
    FourierTransformDialog *fourier_transform_dialog = new FourierTransformDialog(this, workspace, index);
    fourier_transform_dialog->show();

}

void MainWindow::on_actionInterpolate_to_New_Abscissa_triggered()
{
    if (!workspace->datasets()->size()){
        QMessageBox::information(this,
                                 "No datasets loaded",
                                 "No dataset exists on which to perform this operation");
    }
    QModelIndex index = dataset_tree_view_->currentIndex();
    AbscissaInterpolationDialog *abs_interp_dialog =
            new AbscissaInterpolationDialog(this, workspace, index);
    abs_interp_dialog->show();
}

void MainWindow::on_actionSave_Log_File_triggered()
{
    if (!workspace->datasets()->size()){
        QMessageBox::information(this,
                                 "No datasets loaded",
                                 "No dataset exists on which to perform this operation");
        return;
    }
    QModelIndex index = dataset_tree_view_->currentIndex();
    QSharedPointer<VespucciDataset> dataset = workspace->DatasetAt(index);
    QString filename = QFileDialog::getSaveFileName(this,
                                                    "Save Log File",
                                                    workspace->directory(),
                                                    "Text Files (*.txt)");
    bool ok = dataset->SaveLogFile(filename);
    QString message = (ok? "File Saved Successfully" : "Saving Log File Failed");
    QString title = (ok? "Saved Log File" : "Saving Log File Failed");
    QMessageBox::information(this, title, message);
}

void MainWindow::on_actionImport_Dataset_from_Multiple_Files_triggered()
{
    StitchImportDialog *stitch_dialog = new StitchImportDialog(this, workspace);
    stitch_dialog->show();
}

void MainWindow::on_actionCreate_Plot_triggered()
{

}
