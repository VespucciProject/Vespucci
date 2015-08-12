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
#include "Global/vespucciworkspace.h"
#include <iostream>
#include <QtSvg>
///
/// \brief VespucciWorkspace::VespucciWorkspace
/// Constructor
VespucciWorkspace::VespucciWorkspace()
{
    dataset_loading_count_ = 0;
    directory_ = QDir::homePath();
    //only datarange and colors need to be set.
    //need to call a function here or in mainwindow to evaluate values.


}

///
/// \brief VespucciWorkspace::~VespucciWorkspace
/// Destructor
VespucciWorkspace::~VespucciWorkspace()
{
    cout << "Workspace destructor" << endl;
}

///
/// \brief VespucciWorkspace::SetPointers
/// \param main_window
/// Set pointers to the main window in the workspace (mainwindow requires
/// workspace).
void VespucciWorkspace::SetPointers(MainWindow *main_window)
{
    main_window_ = main_window;
    QObject::connect(dataset_list_model_, SIGNAL(DatasetAdded(QModelIndex)), main_window_, SLOT(DatasetAdded(QModelIndex)));
}


// FUNCTIONS THAT ADD AND REMOVE WORKSPACE OBJECTS

///
/// \brief VespucciWorkspace::AddDataset
/// \param dataset
/// Add a dataset to the datasets_ QList
void VespucciWorkspace::AddDataset(QSharedPointer<VespucciDataset> dataset)
{
    bool success = dataset_list_model_->AddDataset(dataset);

    if (!success)
        QMessageBox::warning(main_window_, "Fail!", "Adding dataset failed", QMessageBox::Ok);
}



// Useful when the index is known (as when the list is being iterated)
// These functions should be safe because the names list and the object list only update together

///
/// \brief VespucciWorkspace::RemoveDatasetAt
/// \param i
/// Useful when the index is known (as when the list is being iterated)
/// These functions should be safe because the names list and the object list only update together

void VespucciWorkspace::RemoveDatasetAt(int row, const QModelIndex &parent)
{
    dataset_list_model_->removeRow(row, parent);
}




//ACCESS FUNCTIONS
///
/// \brief VespucciWorkspace::dataset_loading_count
/// \return
/// A count of the datasets that have been loaded.
int VespucciWorkspace::dataset_loading_count() const
{
    return dataset_loading_count_;
}

///
/// \brief VespucciWorkspace::directory
/// \return
/// The global working directory
QString VespucciWorkspace::directory() const
{
    return directory_;
}

///
/// \brief VespucciWorkspace::set_directory
/// \param directory New global directory
/// Sets the global directory
void VespucciWorkspace::set_directory(QString directory)
{
    directory_ = directory;
}

///
/// \brief VespucciWorkspace::dataset_names
/// \return List of names of all the workspaces
QStringList VespucciWorkspace::dataset_names() const
{
    return dataset_names_;
}

///
/// \brief VespucciWorkspace::main_window
/// \return Pointer to the main window
///
MainWindow *VespucciWorkspace::main_window()
{
    return main_window_;
}

///
/// \brief VespucciWorkspace::GetWavelengthMax
/// \param row Index of the dataset
/// \return Maximum of the spectral abscissa
///
double VespucciWorkspace::GetWavelengthMax(int row) const
{
    return datasets_->at(row)->wavelength().max();
}

///
/// \brief VespucciWorkspace::GetWavelengthMin
/// \param row Index of the dataset
/// \return  Minimum of the spectral abscissa
///
double VespucciWorkspace::GetWavelengthMin(int row) const
{
    return datasets_->at(row)->wavelength().min();
}


///
/// \brief VespucciWorkspace::DatasetAt
/// \param i Index of desired dataset
/// \return Desired dataset
///
QSharedPointer<VespucciDataset> VespucciWorkspace::DatasetAt(int i) const
{
    //return datasets_[i];
    return dataset_list_model_->DatasetAt(i);
}

///
/// \brief VespucciWorkspace::directory_ptr
/// \return Pointer to global working directory
///
QString* VespucciWorkspace::directory_ptr()
{
    return &directory_;
}

///
/// \brief VespucciWorkspace::RecalculateGlobalDataRange
/// \param new_data_range Pointer to new data range
/// \return Whether or not data range was changed
///
bool VespucciWorkspace::RecalculateGlobalDataRange(QCPRange *new_data_range)
{
    if (new_data_range->upper > global_data_range_.upper){
        global_data_range_.upper = new_data_range->upper;
        return true;
    }
    if (new_data_range->lower < global_data_range_.lower){
        global_data_range_.lower = new_data_range->lower;
        return true;
    }
    return false;
}

///
/// \brief VespucciWorkspace::RefreshGlobalColorGradient
/// \param new_gradient New gradient
/// Changes the global gradient
void VespucciWorkspace::RefreshGlobalColorGradient(QCPColorGradient new_gradient)
{
    global_gradient_ = new_gradient;
}

///
/// \brief VespucciWorkspace::SetGlobalDataRange
/// \param new_data_range
/// Changes the global data range
void VespucciWorkspace::SetGlobalDataRange(QCPRange *new_data_range)
{
    global_data_range_ = *new_data_range;
}

///
/// \brief VespucciWorkspace::global_data_range
/// \return The global data range
///
QCPRange* VespucciWorkspace::global_data_range()
{
    return &global_data_range_;
}

///
/// \brief VespucciWorkspace::global_gradient
/// \return The global color gradient
///
QCPColorGradient* VespucciWorkspace::global_gradient()
{
    return &global_gradient_;
}

///
/// \brief VespucciWorkspace::CreateLogFile
/// \param dataset_name
/// \return
/// Creates a pointer to a heap-allocated QFile which is open.
QFile* VespucciWorkspace::CreateLogFile(QString dataset_name)
{
    //set up temporary filename (UTC date in close to ISO 8601 format)
    QDateTime datetime = QDateTime::currentDateTimeUtc();
    QString log_filename = datetime.toString("yyyy-MM-dd-hhmmss");
    log_filename += "_" + dataset_name + ".temp.txt";
    QFile *log_file = new QFile(log_filename);
    log_file->open(QIODevice::ReadWrite);
    return log_file;
}

///
/// \brief VespucciWorkspace::ClearDatasets
/// Clears the internal container for datasets in the list model
void VespucciWorkspace::ClearDatasets()
{
    dataset_list_model_->ClearDatasets();
}

///
/// \brief VespucciWorkspace::datasets
/// \return a copy of the list of pointers to datasets
///
QList<QSharedPointer<VespucciDataset> > *VespucciWorkspace::datasets()
{
    return datasets_;
}

///
/// \brief VespucciWorkspace::SetListWidgetModel
/// \param model
/// Set the dataset list model
void VespucciWorkspace::SetListWidgetModel(DatasetListModel *model)
{
    dataset_list_model_ = model;
}

///
/// \brief VespucciWorkspace::SetDatasets
/// \param datasets
/// Set the master list of datasets;
void VespucciWorkspace::SetDatasets(QList<QSharedPointer<VespucciDataset> > *datasets)
{
    datasets_ = datasets;
}

///
/// \brief VespucciWorkspace::UpdateCount
/// \return The new count
/// Increments dataset loading count
unsigned int VespucciWorkspace::UpdateCount()
{
    return ++dataset_loading_count_;
}

///
/// \brief dataset_list_model
/// \return The dataset list model
///
DatasetListModel* VespucciWorkspace::dataset_list_model() const
{
    return dataset_list_model_;
}

void VespucciWorkspace::CleanLogFiles()
{
    QDir current_dir(QCoreApplication::applicationDirPath());
    QStringList filters;
    filters << "*.txt";
    foreach (QString filename, current_dir.entryList(filters, QDir::Files))
        QFile::remove(filename);
}


/*
///
/// \brief VespucciWorkspace::SavePlot
/// \param plot
/// \param filename
/// \return
///
bool VespucciWorkspace::SavePlot(QCustomPlot *plot, QString filename) const
{
    QStringList filename_list = filename.split(".");
    QString extension = filename_list.last();

    //this method of determining type may not be valid on non-Win platforms
    //check this on GNU/Linux and Mac OSX later.

    bool success = true;

    if (extension == "bmp")
        success = plot->saveBmp(filename, 0, 0, 1.0);
    else if (extension == "pdf")
        success = plot->savePdf(filename, false, 0, 0, "Vespucci 1.0", "Plot");
    else if (extension == "png"){
        bool ok;
        int quality = QInputDialog::getInt(plot->parentWidget(), "Enter Quality",
                                           "Quality (%)",
                                           80, 0, 100, 1, &ok);
        if (ok)
            plot->savePng(filename, 0, 0, 1.0, quality);
    }

    else if (extension == "jpg"){
        bool ok;
        int quality = QInputDialog::getInt(plot->parentWidget(), "Enter Quality",
                                           "Quality (%)",
                                           80, 0, 100, 1, &ok);
        if (ok)
            plot->saveJpg(filename, 0, 0, 1.0, quality);
    }
    else if (extension == "svg"){
        QPicture picture;
        QCPPainter qcp_painter(&picture);
        plot->toPainter(&qcp_painter);


        QSvgGenerator generator;
        generator.setFileName(filename);

        QPainter painter;

        painter.begin(&generator);
        painter.drawPicture(0, 0, picture);
        painter.end();
    }
    else if (extension == "emf"){
        QStringList filename_trunk_list = filename_list;
        filename_trunk_list.removeLast();
        QString filename_trunk = filename_trunk_list.join(".");
        QString SVG_filename = filename_trunk + ".svg";
        QPicture picture;
        QCPPainter qcp_painter(&picture);
        plot->toPainter(&qcp_painter);

        QSvgGenerator generator;
        generator.setFileName(SVG_filename);

        QPainter painter;

        painter.begin(&generator);
        painter.drawPicture(0, 0, picture);
        painter.end();

        //call java program "EMFGenerator" to convert svg file then
        QProcess *process = new QProcess(0);
        QString command = "java -jar EMFGenerator.jar \"" + SVG_filename + "\"";
        process->start(command);
    }

    else{
        //default to tif, force extension (for Windows compatability)
        if (extension != "tif")
            filename.append(".tif");
        bool ok;
        int quality = QInputDialog::getInt(plot->parentWidget(),
                                           "Compression",
                                           "Enter 0 for no compression,"
                                           "1 for LZW lossless compression",
                                           0, 0, 1, 1, &ok);
        if (ok){
            QPixmap background = plot->background();
            plot->setBackground(Qt::transparent);
            plot->replot();
            success = plot->saveRastered(filename, 0, 0, 1.0, "TIF", quality);
            plot->setBackground(background);
            plot->replot();

        }
    }

    return success;
}
*/
