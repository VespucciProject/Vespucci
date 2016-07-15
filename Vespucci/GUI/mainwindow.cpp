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
#include "GUI/Processing/multiimportdialog.h"
#include "GUI/Processing/bulkconversiondialog.h"
#include "GUI/Analysis/classicalleastsquaresdialog.h"
#include "GUI/settingsdialog.h"
#include "GUI/Processing/abscissatransformdialog.h"
#include "GUI/Processing/fouriertransformdialog.h"
#include "GUI/Processing/abscissainterpolationdialog.h"
#include "GUI/Processing/stitchimportdialog.h"
#include "GUI/Analysis/plotmakerdialog.h"
#include "GUI/Display/mapdialog.h"
#include "GUI/Display/globalgradientdialog.h"

///
/// \brief MainWindow::MainWindow
/// \param parent usually 0
/// \param ws the workspace of this instance of Vespucci
///Default constructor
MainWindow::MainWindow(QWidget *parent, QSharedPointer<VespucciWorkspace> ws) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    workspace_ = ws;
    ui->setupUi(this);
    dataset_tree_model_ = new DatasetTreeModel(ui->datasetTreeView);
    ui->datasetTreeView->setModel(dataset_tree_model_);
    data_viewer_ = new DataViewer(this, workspace_);
    plot_viewer_ = new PlotViewer(this);
    spectrum_editor_ = new SpectrumSelectionDialog(this);
    stats_viewer_ = new StatsDialog(this, workspace_);
    macro_editor_ = new MacroDialog(this, workspace_);
    python_shell_ = new PythonShellDialog(this, workspace_);

    global_map_count_ = 0;
    workspace_->SetPointers(this, dataset_tree_model_);

    //Connects the closing of persistent dialogs to unchecking their menu entries
    connect(data_viewer_, &DataViewer::SetActionChecked,
            this, &MainWindow::SetDataViewerActionChecked);
    connect(plot_viewer_, &PlotViewer::SetActionChecked,
            this, &MainWindow::SetPlotViewerActionChecked);
    connect(spectrum_editor_, &SpectrumSelectionDialog::SetActionChecked,
            this, &MainWindow::SetSpectrumEditorActionChecked);
    connect(stats_viewer_, &StatsDialog::SetActionChecked,
            this, &MainWindow::SetStatsViewerActionChecked);
    connect(macro_editor_, &MacroDialog::SetActionChecked,
            this, &MainWindow::SetMacroEditorActionChecked);
    connect(python_shell_, &PythonShellDialog::SetActionChecked,
            this, &MainWindow::SetPythonShellActionChecked);

    //connects the change in active dataset connected to change in display
    connect(this, &MainWindow::MatrixSelectionChanged,
            stats_viewer_, &StatsDialog::MatrixSelectionChanged);
    connect(this, &MainWindow::DatasetSelectionChanged,
            spectrum_editor_, &SpectrumSelectionDialog::DatasetSelectionChanged);

    //Triggers the removal of references that are about to become bad
    connect(this, &MainWindow::DatasetToBeRemoved,
            data_viewer_, &DataViewer::DatasetToBeRemoved);
    connect(this, &MainWindow::DatasetToBeRemoved,
            spectrum_editor_, &SpectrumSelectionDialog::DatasetToBeRemoved);
    connect(this, &MainWindow::DatasetToBeRemoved,
            stats_viewer_, &StatsDialog::DatasetToBeRemoved);
    connect(this, &MainWindow::MatrixToBeRemoved,
            data_viewer_, &DataViewer::MatrixToBeRemoved);
    connect(this, &MainWindow::MatrixToBeRemoved,
            stats_viewer_, &StatsDialog::MatrixToBeRemoved);

}

MainWindow::~MainWindow()
{
    delete ui;
}

///
/// \brief MainWindow::RefreshTreeModel
/// \param data_model
/// Creates a new DatasetTreeModel and changes the tree view to the new model
void MainWindow::RefreshTreeModel(const DataModel *data_model)
{
    dataset_tree_model_ = new DatasetTreeModel(ui->datasetTreeView, data_model);
    ui->datasetTreeView->setModel(dataset_tree_model_);
    ui->datasetTreeView->resizeColumnToContents(0);
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

void MainWindow::TreeItemSelected(const QModelIndex &index)
{
    if (!index.isValid()) return;
    TreeItem *selected_item = dataset_tree_model_->getItem(index);
    emit DatasetSelectionChanged(selected_item->keys()[0]);
    if (selected_item->type() == TreeItem::ItemType::Matrix)
        emit MatrixSelectionChanged(selected_item->keys());
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
    LoadDataset *load_dataset_window = new LoadDataset(this,workspace_);
    load_dataset_window->setAttribute(Qt::WA_DeleteOnClose);
    load_dataset_window->show();
}

///
/// \brief MainWindow::on_actionAbout_Vespucci_triggered
///Triggers the "About Vespucci" window.
void MainWindow::on_actionAbout_Vespucci_triggered()
{
    AboutDialog *about_window = new AboutDialog(this);
    about_window->setAttribute(Qt::WA_DeleteOnClose);
    about_window->show();
}

///
/// \brief MainWindow::on_actionCiting_Vespucci_triggered
///Triggers a window containing citation information.
void MainWindow::on_actionCiting_Vespucci_triggered()
{
    CitationDialog *citation_window = new CitationDialog(this);
    citation_window->setAttribute(Qt::WA_DeleteOnClose);
    citation_window->show();
}

///
/// \brief MainWindow::on_actionNew_Univariate_Map_triggered
///Triggers univariate dialog.
void MainWindow::on_actionNew_Univariate_Map_triggered()
{
    TreeItem *item = dataset_tree_model_->getItem(ui->datasetTreeView->currentIndex());
    if (item->type() == TreeItem::ItemType::Base){
        QMessageBox::information(this,
                                 "No datasets loaded",
                                 "No dataset exists on which to perform this operation");
        return;
    }
    QString dataset_key = item->DatasetKey();
    UnivariateDialog *univariate_dialog =
        new UnivariateDialog(this, workspace_, dataset_key);
    univariate_dialog->setAttribute(Qt::WA_DeleteOnClose);
    univariate_dialog->show();

}

///
/// \brief MainWindow::on_actionNew_Band_Ratio_Map_triggered
///Triggers band ratio dialog.
void MainWindow::on_actionNew_Band_Ratio_Map_triggered()
{
    TreeItem *item = dataset_tree_model_->getItem(ui->datasetTreeView->currentIndex());
    if (item->type() == TreeItem::ItemType::Base){
        QMessageBox::information(this,
                                 "No datasets loaded",
                                 "No dataset exists on which to perform this operation");
        return;
    }


    QString dataset_key = item->DatasetKey();

    BandRatioDialog *band_ratio_dialog = new BandRatioDialog(this, workspace_, dataset_key);
    band_ratio_dialog->setAttribute(Qt::WA_DeleteOnClose);
    band_ratio_dialog->show();
}

///
/// \brief MainWindow::on_actionPrincipal_Components_Analysis_triggered
///Triggers principal components dialog
void MainWindow::on_actionPrincipal_Components_Analysis_triggered()
{
    TreeItem *item = dataset_tree_model_->getItem(ui->datasetTreeView->currentIndex());
    if (item->type() == TreeItem::ItemType::Base){
        QMessageBox::information(this,
                                 "No datasets loaded",
                                 "No dataset exists on which to perform this operation");
        return;
    }


    QString dataset_key = item->DatasetKey();


    PrincipalComponentsDialog *principal_components_dialog =
            new PrincipalComponentsDialog(this, workspace_, dataset_key);
    principal_components_dialog->setAttribute(Qt::WA_DeleteOnClose);
    principal_components_dialog->show();


}


///
/// \brief MainWindow::on_actionVertex_Components_triggered
///Triggers vertex components dialog
void MainWindow::on_actionVertex_Components_triggered()
{
    TreeItem *item = dataset_tree_model_->getItem(ui->datasetTreeView->currentIndex());
    if (item->type() == TreeItem::ItemType::Base){
        QMessageBox::information(this,
                                 "No datasets loaded",
                                 "No dataset exists on which to perform this operation");
        return;
    }


    QString dataset_key = item->DatasetKey();


    VCADialog *vca_dialog = new VCADialog(this, workspace_, dataset_key);
    vca_dialog->setAttribute(Qt::WA_DeleteOnClose);
    vca_dialog->show();


}


///
/// \brief MainWindow::on_actionPartial_Least_Squares_triggered
/// Triggers partial least squares dialog
///
void MainWindow::on_actionPartial_Least_Squares_triggered()
{
    TreeItem *item = dataset_tree_model_->getItem(ui->datasetTreeView->currentIndex());
    if (item->type() == TreeItem::ItemType::Base){
        QMessageBox::information(this,
                                 "No datasets loaded",
                                 "No dataset exists on which to perform this operation");
        return;
    }


    QString dataset_key = item->DatasetKey();


    PLSDialog *pls_dialog =
            new PLSDialog(this, workspace_, dataset_key);
    pls_dialog->setAttribute(Qt::WA_DeleteOnClose);
    pls_dialog->show();


}

///
/// \brief MainWindow::on_actionK_Means_Clustering_triggered
/// Triggers K-Means dialog
///
void MainWindow::on_actionK_Means_Clustering_triggered()
{
    TreeItem *item = dataset_tree_model_->getItem(ui->datasetTreeView->currentIndex());
    if (item->type() == TreeItem::ItemType::Base){
        QMessageBox::information(this,
                                 "No datasets loaded",
                                 "No dataset exists on which to perform this operation");
        return;
    }


    QString dataset_key = item->DatasetKey();


    KMeansDialog *k_means_dialog =
            new KMeansDialog(this, workspace_, dataset_key);
    k_means_dialog->setAttribute(Qt::WA_DeleteOnClose);
    k_means_dialog->show();
}


///
/// \brief MainWindow::on_actionNormalize_Standardize_triggered
///
void MainWindow::on_actionNormalize_Standardize_triggered()
{
    TreeItem *item = dataset_tree_model_->getItem(ui->datasetTreeView->currentIndex());
    if (item->type() == TreeItem::ItemType::Base){
        QMessageBox::information(this,
                                 "No datasets loaded",
                                 "No dataset exists on which to perform this operation");
        return;
    }
    QString dataset_key = item->DatasetKey();
    QSharedPointer<VespucciDataset> dataset = workspace_->GetDataset(dataset_key);
    QStringList methods;
    methods << "Min/Max" << "Vector Norm" << "Unit Area" << "Z-score"
            << "Standard Normal Variate" << "Peak Intensity" << "Scale Spectra"
            << "Absolute Value" << "Mean Center";
    bool ok;
    QString method = QInputDialog::getItem(this,
                                           "Normalization/Standardization",
                                           "Method:", methods, 0, false, &ok);
    double scaling_factor, offset;
    try{
        if (ok && method == "Min/Max"){dataset->MinMaxNormalize();}
        else if (ok && method == "Vector Norm"){dataset->VectorNormalize();}
        else if (ok && method == "Mean Center"){dataset->MeanCenter();}
        else if (ok && method == "Unit Area"){dataset->UnitAreaNormalize();}
        else if (ok && method == "Z-score"){
            offset = QInputDialog::getDouble(this, "Enter Offset", "Offset",
                                             0, -2147483647, 2147483647, 4, &ok);
            dataset->SNVNormalize(offset, true);
        }

        else if (ok && method == "Peak Intensity"){
            double min = dataset->wavelength_ptr()->min();
            double max = dataset->wavelength_ptr()->max();
            RangeDialog *range_dialog = new RangeDialog(this, min, max);
            range_dialog->setWindowTitle("Peak Intensity Normalization");
            connect(range_dialog, &RangeDialog::DialogAccepted,
                    this, &MainWindow::RangeDialogAccepted);
            range_dialog->show();
        }
        else if (ok && method == "Scale Spectra"){
            scaling_factor = QInputDialog::getDouble(this, "Enter Scaling Factor",
                                                     "Factor", 1, -100, 100, 2, &ok);
            dataset->Scale(scaling_factor);
        }
        else if (ok && method == "Standard Normal Variate"){
            offset = QInputDialog::getDouble(this, "Enter Offset", "Offset",
                                             0, -2147483647, 2147483647, 4, &ok);
            dataset->SNVNormalize(offset, false);
        }
        else if (ok && method == "Absolute Value"){dataset->AbsoluteValue();}
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
    TreeItem *item = dataset_tree_model_->getItem(ui->datasetTreeView->currentIndex());
    if (item->type() == TreeItem::ItemType::Base){
        QMessageBox::information(this,
                                 "No datasets loaded",
                                 "No dataset exists on which to perform this operation");
        return;
    }

    QString dataset_key = item->DatasetKey();

    QSharedPointer<VespucciDataset> dataset = workspace_->GetDataset(dataset_key);
    QString filename =
            QFileDialog::getOpenFileName(this,
                                         tr("Select Background File"),
                                         workspace_->directory(),
                                         "Vespucci Spectrum Files (*.arma *.txt *.csv)");
    mat input;
    QFileInfo file_info(filename);
    workspace_->set_directory(file_info.dir().path());
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
            dataset->SubtractBackground(input, filename);
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
    TreeItem *item = dataset_tree_model_->getItem(ui->datasetTreeView->currentIndex());
    if (item->type() == TreeItem::ItemType::Base){
        QMessageBox::information(this,
                                 "No datasets loaded",
                                 "No dataset exists on which to perform this operation");
        return;
    }

    bool success;
    QString dataset_key = item->DatasetKey();
    QSharedPointer<VespucciDataset> dataset = workspace_->GetDataset(dataset_key);
    QString filename =
            QFileDialog::getSaveFileName(this,
                                         tr("Save Spectra Matrix"),
                                         workspace_->directory(),
                                         tr("Vespucci Binary (*.arma);;"
                                            "Comma-separated Values (*.csv);;"
                                            "Tab-separated Txt (*.txt);;"));
    QFileInfo file_info(filename);
    workspace_->set_directory(file_info.dir().path());

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
    TreeItem *item = dataset_tree_model_->getItem(ui->datasetTreeView->currentIndex());
    if (item->type() == TreeItem::ItemType::Base){
        QMessageBox::information(this,
                                 "No datasets loaded",
                                 "No dataset exists on which to perform this operation");
        return;
    }
    bool success;

    QString dataset_key = item->DatasetKey();

    QSharedPointer<VespucciDataset> dataset = workspace_->GetDataset(dataset_key);

    QString filename =
            QFileDialog::getSaveFileName(this,
                                         tr("Save Spectra Matrix"),
                                         workspace_->directory(),
                                         tr("Vespucci Binary (*.arma);;"
                                            "Comma-separated Values (*.csv);;"
                                            "Tab-separated Txt (*.txt);;"));
    QFileInfo file_info(filename);
    workspace_->set_directory(file_info.dir().path());
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
    TreeItem *item = dataset_tree_model_->getItem(ui->datasetTreeView->currentIndex());
    if (item->type() == TreeItem::ItemType::Base){
        QMessageBox::information(this,
                                 "No datasets loaded",
                                 "No dataset exists on which to perform this operation");
        return;
    }
    bool success;

    QString dataset_key = item->DatasetKey();

    QSharedPointer<VespucciDataset> dataset = workspace_->GetDataset(dataset_key);

    QString filename =
            QFileDialog::getSaveFileName(this,
                                         tr("Save Average Spectrum"),
                                         workspace_->directory(),
                                         tr("Vespucci Binary (*.arma);;"
                                            "Comma-separated Values (*.csv);;"
                                            "Tab-separated Txt (*.txt);;"));
    QFileInfo file_info(filename);
    workspace_->set_directory(file_info.dir().path());

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
    TreeItem *item = dataset_tree_model_->getItem(ui->datasetTreeView->currentIndex());
    if (item->type() == TreeItem::ItemType::Base){
        QMessageBox::information(this,
                                 "No datasets loaded",
                                 "No dataset exists on which to perform this operation");
        return;
    }
    bool success;

    QString dataset_key = item->DatasetKey();

    QSharedPointer<VespucciDataset> dataset = workspace_->GetDataset(dataset_key);

    QString filename =
            QFileDialog::getSaveFileName(this,
                                         tr("Save Spectra Matrix"),
                                         workspace_->directory(),
                                         tr("Vespucci Binary (*.arma);;"
                                            "Comma-separated Values (*.csv);;"
                                            "Tab-separated Txt (*.txt);;"));
    QFileInfo file_info(filename);
    workspace_->set_directory(file_info.dir().path());


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
    TreeItem *item = dataset_tree_model_->getItem(ui->datasetTreeView->currentIndex());
    if (item->type() == TreeItem::ItemType::Base){
        QMessageBox::information(this,
                                 "No datasets loaded",
                                 "No dataset exists on which to perform this operation");
        return;
    }
    bool success;

    QString dataset_key = item->DatasetKey();

    QSharedPointer<VespucciDataset> dataset = workspace_->GetDataset(dataset_key);

    QString filename =
            QFileDialog::getSaveFileName(this,
                                         tr("Save Spectra Matrix"),
                                         workspace_->directory(),
                                         tr("Vespucci Binary (*.arma);;"
                                            "Comma-separated Values (*.csv);;"
                                            "Tab-separated Txt (*.txt);;"));
    QFileInfo file_info(filename);
    workspace_->set_directory(file_info.dir().path());


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
    TreeItem *item = dataset_tree_model_->getItem(ui->datasetTreeView->currentIndex());
    if (item->type() == TreeItem::ItemType::Base){
        QMessageBox::information(this,
                                 "No datasets loaded",
                                 "No dataset exists on which to perform this operation");
        return;
    }
    if (ui->datasetTreeView->model()->rowCount() < 1)
        return;
    bool success;

    QString dataset_key = item->DatasetKey();

    QSharedPointer<VespucciDataset> dataset = workspace_->GetDataset(dataset_key);

    QString filename =
            QFileDialog::getSaveFileName(this,
                                         tr("Save Spectra Matrix"),
                                         workspace_->directory(),
                                         tr("Vespucci Dataset (*.vds)"));

    QFileInfo file_info(filename);
    workspace_->set_directory(file_info.dir().path());

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
    TreeItem *item = dataset_tree_model_->getItem(ui->datasetTreeView->currentIndex());
    if (item->type() == TreeItem::ItemType::Base){
        QMessageBox::information(this,
                                 "No datasets loaded",
                                 "No dataset exists on which to perform this operation");
        return;
    }

    QString dataset_key = item->DatasetKey();

    FilterDialog *filter_dialog = new FilterDialog(this, workspace_, dataset_key);
    filter_dialog->setAttribute(Qt::WA_DeleteOnClose);
    filter_dialog->show();
}

///
/// \brief MainWindow::on_actionClose_Dataset_triggered
/// Closes the dataset. Should force it to go out of scope
void MainWindow::on_actionClose_Dataset_triggered()
{
    TreeItem *item = dataset_tree_model_->getItem(ui->datasetTreeView->currentIndex());
    if (item->type() == TreeItem::ItemType::Base){
        QMessageBox::information(this,
                                 "No datasets loaded",
                                 "No dataset exists on which to perform this operation");
        return;
    }

    QString dataset_key = item->DatasetKey();

    QString name = ui->datasetTreeView->currentIndex().data().value<QString>();
    QString text = "Are you sure you want to close the dataset " + name + "?" +
            " The data and all associated maps will be deleted.";

    int response = QMessageBox::question(this, "Close Dataset?", text,
                                         QMessageBox::Ok, QMessageBox::Cancel);

    if (response == QMessageBox::Ok){
        emit DatasetToBeRemoved(dataset_key);
        workspace_->RemoveDataset(dataset_key);
    }
}

///
/// \brief MainWindow::on_actionDocumentation_triggered
/// Triggers the window that takes you to the website
void MainWindow::on_actionDocumentation_triggered()
{
    QUrl website_url("http://vespucciproject.org/");
    QDesktopServices::openUrl(website_url);
}

///
/// \brief MainWindow::on_actionCrop_triggered
/// Triggers the CropDialog
void MainWindow::on_actionCrop_triggered()
{
    TreeItem *item = dataset_tree_model_->getItem(ui->datasetTreeView->currentIndex());
    if (item->type() == TreeItem::ItemType::Base){
        QMessageBox::information(this,
                                 "No datasets loaded",
                                 "No dataset exists on which to perform this operation");
        return;
    }

    QString dataset_key = item->DatasetKey();
    CropDialog *crop_dialog = new CropDialog(this, workspace_, dataset_key);
    crop_dialog->setAttribute(Qt::WA_DeleteOnClose);
    crop_dialog->show();
}

///
/// \brief MainWindow::on_actionCorrect_Baseline_triggered
/// Triggers Baseline correction dialog
void MainWindow::on_actionCorrect_Baseline_triggered()
{
    TreeItem *item = dataset_tree_model_->getItem(ui->datasetTreeView->currentIndex());
    if (item->type() == TreeItem::ItemType::Base){
        QMessageBox::information(this,
                                 "No datasets loaded",
                                 "No dataset exists on which to perform this operation");
        return;
    }


    QString dataset_key = item->DatasetKey();

    BaselineDialog *baseline_dialog = new BaselineDialog(this, workspace_, dataset_key);
    baseline_dialog->setAttribute(Qt::WA_DeleteOnClose);
    baseline_dialog->show();
}

///
/// \brief MainWindow::workspace_ptr
/// \return
/// Very kludgy way of getting the workspace variable to window variables.
QSharedPointer<VespucciWorkspace> MainWindow::workspace_ptr()
{
    return workspace_;
}

void MainWindow::on_actionUndo_triggered()
{
    TreeItem *item = dataset_tree_model_->getItem(ui->datasetTreeView->currentIndex());
    if (item->type() == TreeItem::ItemType::Base){
        QMessageBox::information(this,
                                 "No datasets loaded",
                                 "No dataset exists on which to perform this operation");
        return;
    }


    QString dataset_key = item->DatasetKey();

    QSharedPointer<VespucciDataset> dataset = workspace_->GetDataset(dataset_key);

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

void MainWindow::SetActiveDatasetTreeIndex(const QModelIndex &index)
{
    ui->datasetTreeView->setCurrentIndex(index);
}


void MainWindow::DisplayWarning(const QString &title, const QString &text)
{
    QMessageBox::warning(this, title, text);
}

void MainWindow::DisplayInformation(const QString &title, const QString &text)
{
    QMessageBox::information(this, title, text);
}

void MainWindow::SetDatasetTreeModel(DatasetTreeModel *new_model)
{
    ui->datasetTreeView->setModel(new_model);
}

DatasetTreeModel *MainWindow::dataset_tree_model()
{
    return dataset_tree_model_;
}

PlotViewer *MainWindow::plot_viewer()
{
    return plot_viewer_;
}

DataViewer *MainWindow::data_viewer()
{
    return data_viewer_;
}


void MainWindow::on_actionDelete_Map_triggered()
{
    TreeItem *item = dataset_tree_model_->getItem(ui->datasetTreeView->currentIndex());
    if (item->type() == TreeItem::ItemType::Map){
        workspace_->GetDataset(item->keys().first())->RemoveMap(item->keys().last());
    }
}

void MainWindow::on_actionNew_Composite_Dataset_triggered()
{
    TreeItem *item = dataset_tree_model_->getItem(ui->datasetTreeView->currentIndex());
    if (item->type() == TreeItem::ItemType::Base){
        QMessageBox::information(this,
                                 "No datasets loaded",
                                 "No dataset exists on which to perform this operation");
        return;
    }
    MetaDatasetDialog *meta_dialog = new MetaDatasetDialog(this, workspace_);
    meta_dialog->setAttribute(Qt::WA_DeleteOnClose);
    meta_dialog->show();
}



void MainWindow::on_actionReject_Clipped_Spectra_triggered()
{
    TreeItem *item = dataset_tree_model_->getItem(ui->datasetTreeView->currentIndex());
    if (item->type() == TreeItem::ItemType::Base){
        QMessageBox::information(this,
                                 "No datasets loaded",
                                 "No dataset exists on which to perform this operation");
        return;
    }
    QString dataset_key = item->DatasetKey();
    ThresholdDialog *dialog = new ThresholdDialog(this, workspace_, dataset_key);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->show();
}


void MainWindow::RangeDialogAccepted(double min, double max)
{
    if (ui->datasetTreeView->model()->rowCount() < 1)
        return;
    TreeItem *item = dataset_tree_model_->getItem(ui->datasetTreeView->currentIndex());

    QString dataset_key = item->DatasetKey();

    QSharedPointer<VespucciDataset> dataset = workspace_->GetDataset(dataset_key);
    try{
        dataset->PeakIntensityNormalize(min, max);
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
    TreeItem *item = dataset_tree_model_->getItem(index);
    TreeItem::ItemType type = item->type();


    if (type == TreeItem::ItemType::Map){
        try{
            QSharedPointer<MapData> map_data = workspace_->GetMap(item->keys()[0], item->keys()[1]);
            map_data->ShowMapWindow(!map_data->MapWindowVisible());
        }catch (exception e){
            DisplayExceptionWarning("TreeItemDoubleClicked", e);
        }
    }
    else if (type == TreeItem::ItemType::Matrix){
        QStringList keys = item->keys();
        if (keys.size() == 2){
            try{
                data_viewer_->AddTab(item->keys());
            }catch(exception e){
                DisplayExceptionWarning("TreeItemDoubleClicked", e);
            }
        }
        if (keys.size() == 3){
            try{
                data_viewer_->AddTab(item->keys());
            }catch (exception e){
                DisplayExceptionWarning("TreeItemDoubleClicked", e);
            }
        }
    }
    else{
        //there is nothing to dispaly
    }
}

void MainWindow::SetPlotViewerActionChecked(bool checked)
{
    ui->actionPlot_Viewer->setChecked(checked);
}

void MainWindow::SetDataViewerActionChecked(bool checked)
{
    ui->actionData_Viewer->setChecked(checked);
}

void MainWindow::SetStatsViewerActionChecked(bool checked)
{
    ui->actionStatistics_Viewer->setChecked(checked);
}

void MainWindow::SetSpectrumEditorActionChecked(bool checked)
{
    ui->actionSpectrum_Editor->setChecked(checked);
}

void MainWindow::SetMacroEditorActionChecked(bool checked)
{
    ui->actionMacro_Editor->setChecked(checked);
}

void MainWindow::SetPythonShellActionChecked(bool checked)
{
    ui->actionPython_Shell->setChecked(checked);
}

void MainWindow::on_actionBooleanize_Clamp_triggered()
{
    TreeItem *item = dataset_tree_model_->getItem(ui->datasetTreeView->currentIndex());
    if (item->type() == TreeItem::ItemType::Base){
        QMessageBox::information(this,
                                 "No datasets loaded",
                                 "No dataset exists on which to perform this operation");
        return;
    }
    QString dataset_key = item->DatasetKey();
    BooleanizeDialog *booleanize_dialog = new BooleanizeDialog(this, workspace_, dataset_key);
    booleanize_dialog->setAttribute(Qt::WA_DeleteOnClose);
    booleanize_dialog->show();
}

void MainWindow::on_actionRemove_Vectors_of_Zeros_triggered()
{
    TreeItem *item = dataset_tree_model_->getItem(ui->datasetTreeView->currentIndex());
    if (item->type() == TreeItem::ItemType::Base){
        QMessageBox::information(this,
                                 "No datasets loaded",
                                 "No dataset exists on which to perform this operation");
        return;
    }
    QSharedPointer<VespucciDataset> dataset = workspace_->GetDataset(item->DatasetKey());
    QStringList items;
    bool ok;
    items << "Remove columns of zeros (removes spectra)" << "Remove rows of zeros (removes wavelengths)";
    QString input = QInputDialog::getItem(this, "Select dimension", "Behavior", items, 0, false, &ok);
    if(!ok){
        return;
    }
    else{
        QSharedPointer<VespucciDataset> data = workspace_->DatasetAt(ui->datasetTreeView->currentIndex());
        if (input == "Remove columns of zeros (removes spectra)"){
            try{
                dataset->ShedZeroSpectra();
            }
            catch(std::exception e){
                DisplayExceptionWarning(e);
            }
        }
        else if (input == "Remove rows of zeros (removes wavelengths)"){
            try{
                dataset->ShedZeroWavelengths();
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
    TreeItem *item = dataset_tree_model_->getItem(ui->datasetTreeView->currentIndex());
    if (item->type() == TreeItem::ItemType::Base){
        QMessageBox::information(this,
                                 "No datasets loaded",
                                 "No dataset exists on which to perform this operation");
        return;
    }

    QString dataset_key = item->DatasetKey();

    ScriptDialog *script_dialog = new ScriptDialog(this, workspace_, dataset_key);
    script_dialog->setAttribute(Qt::WA_DeleteOnClose);
    script_dialog->show();
}

void MainWindow::on_actionDetect_Peaks_triggered()
{
    TreeItem *item = dataset_tree_model_->getItem(ui->datasetTreeView->currentIndex());
    if (item->type() == TreeItem::ItemType::Base){
        QMessageBox::information(this,
                                 "No datasets loaded",
                                 "No dataset exists on which to perform this operation");
        return;
    }


    QString dataset_key = item->DatasetKey();

}

void MainWindow::on_actionCalculate_Peak_Populations_triggered()
{
    TreeItem *item = dataset_tree_model_->getItem(ui->datasetTreeView->currentIndex());
    if (item->type() == TreeItem::ItemType::Base){
        QMessageBox::information(this,
                                 "No datasets loaded",
                                 "No dataset exists on which to perform this operation");
        return;
    }


    QString dataset_key = item->DatasetKey();


}

void MainWindow::on_actionImport_From_Multiple_Point_Spectra_triggered()
{
    MultiImportDialog *import_dialog = new MultiImportDialog(this, workspace_);
    import_dialog->setAttribute(Qt::WA_DeleteOnClose);
    import_dialog->show();
}


void MainWindow::on_actionBatch_File_Conversion_triggered()
{
    BulkConversionDialog *conversion_dialog = new BulkConversionDialog(this, workspace_);
    conversion_dialog->setAttribute(Qt::WA_DeleteOnClose);
    conversion_dialog->show();
}

void MainWindow::on_actionClassical_Least_Squares_triggered()
{
    TreeItem *item = dataset_tree_model_->getItem(ui->datasetTreeView->currentIndex());
    if (item->type() == TreeItem::ItemType::Base){
        QMessageBox::information(this,
                                 "No datasets loaded",
                                 "No dataset exists on which to perform this operation");
        return;
    }


    QString dataset_key = item->DatasetKey();

    ClassicalLeastSquaresDialog *cls_dialog = new ClassicalLeastSquaresDialog(this, workspace_, dataset_key);
    cls_dialog->setAttribute(Qt::WA_DeleteOnClose);
    cls_dialog->show();
}

void MainWindow::on_actionSettings_triggered()
{
    SettingsDialog *settings_dialog = new SettingsDialog(this, workspace_);
    settings_dialog->setAttribute(Qt::WA_DeleteOnClose);
    settings_dialog->show();
}

void MainWindow::on_actionTransform_Abscissa_triggered()
{
    TreeItem *item = dataset_tree_model_->getItem(ui->datasetTreeView->currentIndex());
    if (item->type() == TreeItem::ItemType::Base){
        QMessageBox::information(this,
                                 "No datasets loaded",
                                 "No dataset exists on which to perform this operation");
        return;
    }


    QString dataset_key = item->DatasetKey();

    AbscissaTransformDialog *abscissa_transform_dialog = new AbscissaTransformDialog(this, workspace_, dataset_key);
    abscissa_transform_dialog->setAttribute(Qt::WA_DeleteOnClose);
    abscissa_transform_dialog->show();
}

void MainWindow::on_actionFourierTransform_triggered()
{
    TreeItem *item = dataset_tree_model_->getItem(ui->datasetTreeView->currentIndex());
    if (item->type() == TreeItem::ItemType::Base){
        QMessageBox::information(this,
                                 "No datasets loaded",
                                 "No dataset exists on which to perform this operation");
        return;
    }


    QString dataset_key = item->DatasetKey();

    FourierTransformDialog *fourier_transform_dialog = new FourierTransformDialog(this, workspace_, dataset_key);
    fourier_transform_dialog->setAttribute(Qt::WA_DeleteOnClose);
    fourier_transform_dialog->show();

}

void MainWindow::on_actionInterpolate_to_New_Abscissa_triggered()
{
    TreeItem *item = dataset_tree_model_->getItem(ui->datasetTreeView->currentIndex());
    if (item->type() == TreeItem::ItemType::Base){
        QMessageBox::information(this,
                                 "No datasets loaded",
                                 "No dataset exists on which to perform this operation");
    }


    QString dataset_key = item->DatasetKey();

    AbscissaInterpolationDialog *abs_interp_dialog =
            new AbscissaInterpolationDialog(this, workspace_, dataset_key);
    abs_interp_dialog->setAttribute(Qt::WA_DeleteOnClose);
    abs_interp_dialog->show();
}

void MainWindow::on_actionSave_Log_File_triggered()
{
    TreeItem *item = dataset_tree_model_->getItem(ui->datasetTreeView->currentIndex());
    if (item->type() == TreeItem::ItemType::Base){
        QMessageBox::information(this,
                                 "No datasets loaded",
                                 "No dataset exists on which to perform this operation");
        return;
    }


    QString dataset_key = item->DatasetKey();
    QSharedPointer<VespucciDataset> dataset = workspace_->GetDataset(dataset_key);
    QString filename = QFileDialog::getSaveFileName(this,
                                                    "Save Log File",
                                                    workspace_->directory(),
                                                    "Text Files (*.txt)");
    bool ok = dataset->SaveLogFile(filename);
    QString message = (ok? "File Saved Successfully" : "Saving Log File Failed");
    QString title = (ok? "Saved Log File" : "Saving Log File Failed");
    QMessageBox::information(this, title, message);


}

void MainWindow::on_actionImport_Dataset_from_Multiple_Files_triggered()
{
    StitchImportDialog *stitch_dialog = new StitchImportDialog(this, workspace_);
    stitch_dialog->setAttribute(Qt::WA_DeleteOnClose);
    stitch_dialog->show();
}

void MainWindow::on_actionCreate_Plot_triggered()
{

}

void MainWindow::on_actionPlot_Viewer_toggled(bool arg1)
{
    plot_viewer_->setVisible(arg1);
}

void MainWindow::ChildDialogVisibleToggled(const QString &key, bool arg1)
{
    if (child_dialogs_.contains(key)){
        child_dialogs_[key]->setVisible(arg1);
    }
}

void MainWindow::on_actionData_Viewer_toggled(bool arg1)
{
    if (arg1) data_viewer_->show();
    else data_viewer_->close();
}

void MainWindow::on_actionStatistics_Viewer_toggled(bool arg1)
{
    if (arg1) stats_viewer_->show();
    else stats_viewer_->close();
}

void MainWindow::on_actionSpectrum_Editor_toggled(bool arg1)
{
    if (arg1) spectrum_editor_->show();
    else spectrum_editor_->close();
}

void MainWindow::on_actionPython_Shell_toggled(bool arg1)
{
    if (arg1) python_shell_->show();
    else python_shell_->close();
}

void MainWindow::on_actionMacro_Editor_toggled(bool arg1)
{
    if (arg1) macro_editor_->show();
    else macro_editor_->close();
}

void MainWindow::on_actionMapResult_triggered()
{
    TreeItem *item = dataset_tree_model_->getItem(ui->datasetTreeView->currentIndex());
    QStringList item_keys = item->keys();
    MapDialog *map_dialog = new MapDialog(this, item_keys);
    map_dialog->setAttribute(Qt::WA_DeleteOnClose);
    map_dialog->show();
}

void MainWindow::on_actionSave_Selected_Matrix_triggered()
{
    TreeItem *item = dataset_tree_model_->getItem(ui->datasetTreeView->currentIndex());
    if (item->type() == TreeItem::ItemType::Matrix){
        QString filename = QFileDialog::getSaveFileName(this, "Save " + item->keys().last(),
                                                        workspace_->directory(),
                                                        "Comma-separated text (*.csv);;"
                                                        "Space-delimited text (*.txt);;"
                                                        "Armadillo binary (*.arma);;"
                                                        "Raw binary (*.bin)");
        QString extension = QFileInfo(filename).suffix();
        try{
            if (extension == "bin")
                workspace_->GetMatrix(item->keys()).save(filename.toStdString(), raw_binary);
            else if (extension == "arma")
                workspace_->GetMatrix(item->keys()).save(filename.toStdString(), arma_binary);
            else if (extension == "csv")
                workspace_->GetMatrix(item->keys()).save(filename.toStdString(), csv_ascii);
            else
                workspace_->GetMatrix(item->keys()).save(filename.toStdString(), raw_ascii);
        }catch(exception e){
            DisplayExceptionWarning(e);
        }
    }
}

void MainWindow::on_actionOnline_Documentation_triggered()
{
    QUrl website_url("http://vespucciproject.org/Vespucci-docs");
    QDesktopServices::openUrl(website_url);
}

void MainWindow::on_actionGlobal_Color_Scales_triggered()
{
   GlobalGradientDialog *gradient_dialog = new GlobalGradientDialog(this, workspace_);
   gradient_dialog->setAttribute(Qt::WA_DeleteOnClose);
   gradient_dialog->show();

}
