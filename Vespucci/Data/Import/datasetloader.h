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
#ifndef DATASETLOADER_H
#define DATASETLOADER_H

#include <quazip.h>
#include <quazipfile.h>
#include <yaml-cpp/yaml.h>
#include <mlpack/core.hpp>
#include <QMap>
#include <QList>
#include <QTextStream>
using namespace std;
using namespace arma;
///
/// \brief The DatasetLoader class
/// This class loads the Vespucci Dataset archive (a ZIP file saved as .vds)
/// The constructor initializes the QuaZip object corresponding the the archive.
/// The manifest is loaded using LoadManifest();
/// The YAML data in the manifest is parsed by yaml-cpp into and the structures
/// are populated by their keys and empty matrices.
/// The empty matrices are then loaded from the .bin files using LoadObjects
class DatasetLoader
{
public:
    DatasetLoader(QString filename);

    bool GetData(QMap<QString, mat> &core_objects,
                 QMap<QString, QMap<QString, mat> >  &calcualted_objects,
                 QMap<QString, QString> &metadata);
private:
    void LoadManifest();
    bool ParseManifest();
    bool LoadObjects();
    ///
    /// \brief archive_
    /// The object for reading the archive
    QuaZip archive_;
    ///
    /// \brief current_file_
    /// The QIODevice of the current file
    QuaZipFile current_file_;

    ///
    /// \brief core_objects_
    /// key-value store of the spectra, abscissa, and spatial data
    QMap<QString, mat> core_objects_;

    ///
    /// \brief calculated_objects_
    /// key-value store of each meta-object, with key-value store of each subobject
    QMap<QString, QMap<QString, mat> >  calculated_objects_;

    ///
    /// \brief metadata_
    /// key-value store of dataset metadata (abscissa units, dataset name)
    QMap<QString, QString> metadata_;

    ///
    /// \brief object_filenames_
    /// key-value store linking each object to its associated bin file
    QMap<QString, QString> core_object_filenames_;

    ///
    /// \brief calculated_object_filenames_
    /// key-value store linking each subobject to its associated bin file and parent object
    QMap<QString, QMap<QString, QString> > calculated_object_filenames_;

    ///
    /// \brief core_object_dimensions_
    /// The number of rows and columns in each core object
    QMap<QString, QPair<int, int> > core_object_dimensions_;

    ///
    /// \brief calculated_object_dimensions_
    /// The number of rows and columns in each calculated object
    QMap<QString, QMap<QString, QPair<int, int> > > calculated_object_dimensions_;

    ///
    /// \brief valid_
    /// Various errors will cause this to be false
    bool valid_;

    ///
    /// \brief manifest_
    /// YAML data describing the dataset structure
    QString manifest_;
};

#endif // DATASETLOADER_H
