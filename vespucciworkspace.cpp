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
VespucciWorkspace::VespucciWorkspace()
{
    dataset_loading_count_ = 0;
    directory_ = QDir::homePath();
    //only datarange and colors need to be set.
    //need to call a function here or in mainwindow to evaluate values.


}

void VespucciWorkspace::SetPointers(MainWindow *main_window)
{
    main_window_ = main_window;
    dataset_list_widget_ = main_window->findChild<QListWidget *>("datasetsListWidget");
}


// FUNCTIONS THAT ADD AND REMOVE WORKSPACE OBJECTS

void VespucciWorkspace::AddDataset(QSharedPointer<VespucciDataset> dataset)
{
    QString name = dataset->name();
    datasets_.append(dataset);
    dataset_list_widget_->addItem(name);
    ++dataset_loading_count_;
}



// Useful when the index is known (as when the list is being iterated)
// These functions should be safe because the names list and the object list only update together
void VespucciWorkspace::RemoveDatasetAt(int i)
{
    QListWidgetItem *item = dataset_list_widget_->takeItem(i);
    dataset_list_widget_->removeItemWidget(item);
    datasets_.removeAt(i);

}




//ACCESS FUNCTIONS
int VespucciWorkspace::dataset_loading_count()
{
    return dataset_loading_count_;
}


QString VespucciWorkspace::directory()
{
    return directory_;
}

void VespucciWorkspace::set_directory(QString directory)
{
    directory_ = directory;
}

QStringList VespucciWorkspace::dataset_names()
{
    return dataset_names_;
}

MainWindow *VespucciWorkspace::main_window()
{
    return main_window_;
}

double VespucciWorkspace::GetWavelengthMax(int row)
{
    return datasets_[row]->wavelength().max();
}

double VespucciWorkspace::GetWavelengthMin(int row)
{
    return datasets_[row]->wavelength().min();
}

//Will later include a sanity check before closing workspace.
//Will later call the workspace destructor
void VespucciWorkspace::close()
{
    cout<< "Closing workspace" << endl;
}

QSharedPointer<VespucciDataset> VespucciWorkspace::DatasetAt(int i)
{
    return datasets_[i];
}

QString* VespucciWorkspace::directory_ptr()
{
    return &directory_;
}

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

void VespucciWorkspace::RefreshGlobalColorGradient(QCPColorGradient new_gradient)
{
    global_gradient_ = new_gradient;
}

void VespucciWorkspace::SetGlobalDataRange(QCPRange *new_data_range)
{
    global_data_range_ = *new_data_range;
}

QCPRange* VespucciWorkspace::global_data_range()
{
    return &global_data_range_;
}

QCPColorGradient* VespucciWorkspace::global_gradient()
{
    return &global_gradient_;
}
