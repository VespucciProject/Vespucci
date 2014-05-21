#ifndef VESPUCCIWORKSPACE_H
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

#define VESPUCCIWORKSPACE_H

#include "specmap.h"
#include "univariatemap.h"
#include "mapdata.h"

class SpecMap;
class MapData;

class VespucciWorkspace
{
public:
    VespucciWorkspace();

    QStringList dataset_names();
    //QStringList map_names();

    //adds or removes dataset or map to relevant lists
    void AddDataset(SpecMap dataset);
    void RemoveDataset(QString name);

    void AddMap(MapData* map);
    void RemoveMap(MapData* map);

    void RemoveMapAt(int row);

    MapData* MapAt(int i);

    void RemoveDatasetAt(int i);

    void RefreshLists();

    int dataset_loading_count();
    //int map_loading_count();

    void set_directory(QString directory);
    QString directory();
    QString *directory_ptr();

    void close();
    void SetPointers(QMainWindow *main_window);

    QMainWindow* main_window();
    double GetWavelengthMin(int row);
    double GetWavelengthMax(int row);

    SpecMap* DatasetAt(int i);

private:
    //pointers to main window and necessary widgets
    QMainWindow *main_window_;
    //QListWidget *map_list_widget_;
    QListWidget *dataset_list_widget_;

    QList<SpecMap> datasets_;
    QString directory_; //= QDir::homePath();

    QStringList dataset_names_;
    //QStringList map_names_;

    int dataset_loading_count_;
    //int map_loading_count_;

    QList<MapData *> master_map_list_;
};

#endif // VESPUCCIWORKSPACE_H
