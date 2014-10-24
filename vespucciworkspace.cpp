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

#include "vespucciworkspace.h"
#include <iostream>

//VespucciWorkspace and MainWindow objects hold information about each other
//This might not be the best idea...

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
int VespucciWorkspace::dataset_loading_count()
{
    return dataset_loading_count_;
}

///
/// \brief VespucciWorkspace::directory
/// \return
/// The global working directory
QString VespucciWorkspace::directory()
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
QStringList VespucciWorkspace::dataset_names()
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
/// \return Maximum of the spectral abcissa
///
double VespucciWorkspace::GetWavelengthMax(int row)
{
    return datasets_->at(row)->wavelength().max();
}

///
/// \brief VespucciWorkspace::GetWavelengthMin
/// \param row Index of the dataset
/// \return  Minimum of the spectral abcissa
///
double VespucciWorkspace::GetWavelengthMin(int row)
{
    return datasets_->at(row)->wavelength().min();
}


///
/// \brief VespucciWorkspace::DatasetAt
/// \param i Index of desired dataset
/// \return Desired dataset
///
QSharedPointer<VespucciDataset> VespucciWorkspace::DatasetAt(int i)
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
/// Releases all the dataset pointers, which will close all datasets
void VespucciWorkspace::ClearDatasets()
{

    for (int i=0; i < datasets_->size(); ++i)
        datasets_[i].clear();
}

///
/// \brief VespucciWorkspace::datasets
/// \return a copy of the list of pointers to datasets
///
QList<QSharedPointer<VespucciDataset> > *VespucciWorkspace::datasets()
{
    return datasets_;
}

void VespucciWorkspace::SetListWidgetModel(DatasetListModel *model)
{
    dataset_list_model_ = model;
}

void VespucciWorkspace::SetDatasets(QList<QSharedPointer<VespucciDataset> > *datasets)
{
    datasets_ = datasets;
}
