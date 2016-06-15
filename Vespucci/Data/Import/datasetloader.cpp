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
#include "datasetloader.h"

///
/// \brief DatasetLoader::DatasetLoader
/// \param filename
///
DatasetLoader::DatasetLoader(QString filename)
    : archive_(filename), current_file_(&archive_)
{
    valid_ = false;
    LoadManifest();
    bool manifest_parsed = ParseManifest();
    bool objects_loaded = LoadObjects();
    valid_ = manifest_parsed && objects_loaded;

}

///
/// \brief DatasetLoader::LoadManifest
///
void DatasetLoader::LoadManifest()
{
    archive_.setCurrentFile("manifest.yml");
    current_file_.open(QIODevice::ReadOnly);
    QTextStream input_stream(&current_file_);
    manifest_ = input_stream.readAll();
    current_file_.close();
}

///
/// \brief DatasetLoader::ParseManifest
///
bool DatasetLoader::ParseManifest()
{
    return false;
}

///
/// \brief DatasetLoader::LoadObjects
/// Fills the appropraite members with data loaded from bin files
bool DatasetLoader::LoadObjects()
{
    return false;
}

///
/// \brief DatasetLoader::GetData
/// \param core_objects
/// \param calcualted_objects
/// \param metadata
/// Returns the loaded data
/// The VespucciDataset object that spawned this object uses this function to initialize its members
bool DatasetLoader::GetData(QMap<QString, mat> &core_objects, QMap<QString, QMap<QString, mat> > &calcualted_objects, QMap<QString, QString> &metadata)
{
    if (!valid_){
        core_objects_.clear();
        calculated_objects_.clear();
        metadata_.clear();
    }
    core_objects = core_objects_;
    calcualted_objects = calculated_objects_;
    metadata = metadata_;
    return valid_;
}
