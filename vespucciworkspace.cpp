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
}

void VespucciWorkspace::SetPointers(QMainWindow *main_window)
{
    main_window_ = main_window;
    dataset_list_widget_ = main_window->findChild<QListWidget *>("datasetsListWidget");
}


// FUNCTIONS THAT ADD AND REMOVE WORKSPACE OBJECTS

void VespucciWorkspace::AddDataset(SpecMap dataset)
{
    QString name = dataset.name();

    datasets_.append(dataset);
    dataset_names_.append(name);
    dataset_list_widget_->addItem(name);
    ++dataset_loading_count_;
}



//Useful when only the name is known, and looking up index separately inefficient
//looks up the index and deletes at the index.
void VespucciWorkspace::RemoveDataset(QString name)
{
    int i;
    for(i=0; i<dataset_names_.size(); ++i){
        if(dataset_names_.at(i) == name){
            this->RemoveDatasetAt(i);
        }
    }
}



// Useful when the index is known (as when the list is being iterated)
// These functions should be safe because the names list and the object list only update together
void VespucciWorkspace::RemoveDatasetAt(int i)
{
    QListWidgetItem *item = dataset_list_widget_->takeItem(i);
    datasets_.removeAt(i);
    dataset_names_.removeAt(i);
    dataset_list_widget_->removeItemWidget(item);
    --dataset_loading_count_;
}

void VespucciWorkspace::RemoveMap(MapData *map)
{
    int dataset_index = map->parent_index();
    int source_index = map->source_index();
    datasets_[dataset_index].RemoveMapAt(source_index);
    master_map_list_.removeOne(map);
    this->RefreshLists();
}

void VespucciWorkspace::RemoveMapAt(int i)
{
    MapData *map = master_map_list_[i];
    int dataset_index = map->parent_index();
    int source_index = map->source_index();
    SpecMap dataset = datasets_[dataset_index];
    dataset.RemoveMapAt(source_index);
    master_map_list_.removeAt(i);
    this->RefreshLists();
}

void VespucciWorkspace::AddMap(MapData *map)
{
    master_map_list_.append(map);
}

MapData* VespucciWorkspace::MapAt(int i)
{
    return master_map_list_[i];
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

QMainWindow* VespucciWorkspace::main_window()
{
    return main_window_;
}

double VespucciWorkspace::GetWavelengthMax(int row)
{

    return datasets_[row].wavelength().max();
}

double VespucciWorkspace::GetWavelengthMin(int row)
{
    return datasets_[row].wavelength().min();
}

//Will later include a sanity check before closing workspace.
//Will later call the workspace destructor
void VespucciWorkspace::close()
{
    cout<<"Closing workspace" << endl;
}

SpecMap* VespucciWorkspace::DatasetAt(int i)
{
    return &datasets_[i];
}

QString* VespucciWorkspace::directory_ptr()
{
    return &directory_;

}

void VespucciWorkspace::RefreshLists()
{
    int i;
    for (i = 0; i < datasets_.count(); ++i){
        datasets_[i].SetGlobalIndex(i);
    }
    for (i = 0; i < master_map_list_.count(); ++i){
        master_map_list_[i]->SetGlobalIndex(i);
    }
}
