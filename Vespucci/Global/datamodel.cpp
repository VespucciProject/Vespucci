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
#include "datamodel.h"

DataModel::DataModel()
{

}

///
/// \brief DataModel::GetDataset Get the VespucciDataset named key
/// \param key
/// \return
/// Throws std::invalid_argument if dataset does not exist
QSharedPointer<VespucciDataset> DataModel::GetDataset(const QString &key) const
{
    for (int i = 0; i < datasets_.size(); ++i)
        if (key == datasets_.at(i)->name()) return datasets_.at(i);
    throw invalid_argument("Dataset "
                           + key.toStdString()
                           + " does not exist!");

}

///
/// \brief DataModel::GetResults Get the AnalysisResults object named results_key from the dataset named dataset_key
/// \param dataset_key
/// \param results_key
/// \return
/// Throws std::invalid_argument if dataset does not exist or dataset does exist
/// but analysis result does not exist. text will specify.
QSharedPointer<AnalysisResults> DataModel::GetResults(const QString &dataset_key,
                                                      const QString &results_key) const
{
    if (HasDataset(dataset_key)){
        if (GetDataset(dataset_key)->AnalysisResultsKeys().contains(results_key))
            return GetDataset(dataset_key)->GetAnalysisResult(results_key);
        else
            throw invalid_argument("Dataset "
                                   + dataset_key.toStdString()
                                   + " does not have analysis result "
                                   + results_key.toStdString());
    }
    else{
        throw invalid_argument("Dataset "
                               + dataset_key.toStdString()
                               + " does not exist!");
    }
}

QSharedPointer<AnalysisResults> DataModel::GetResults(const QStringList &keys) const
{
    if (keys.size() != 2)
        throw invalid_argument("Wrong number of keys provided");
    return GetResults(keys[0], keys[1]);
}

///
/// \brief DataModel::GetMap Get the MapData object named map_key from dataset named dataset_key
/// \param dataset_key
/// \param map_key
/// \return
/// Throws std::invalid_argument if dataset or map does not exist
QSharedPointer<MapData> DataModel::GetMap(const QString &dataset_key,
                                          const QString &map_key) const
{
    if (HasDataset(dataset_key)){
        if (GetDataset(dataset_key)->MapKeys().contains(map_key))
            return GetDataset(dataset_key)->GetMapData(map_key);
        else
            throw invalid_argument("Dataset "
                                   + dataset_key.toStdString()
                                   + " does not have map "
                                   + map_key.toStdString());
    }
    else{
        throw invalid_argument("Dataset "
                               + dataset_key.toStdString()
                               + " does not exist!");
    }
}

QSharedPointer<MapData> DataModel::GetMap(const QStringList &keys) const
{
    if (keys.size() != 2) throw invalid_argument("Wrong number of keys provided");
    return GetMap(keys[0], keys[1]);
}

QList<QSharedPointer<MapData> > DataModel::GetMapsUsingColorRange(const QString &range_key)
{
    QList<QSharedPointer<MapData> > maps_using_range;
    for (auto dataset: datasets_){
        for (auto key: dataset->MapKeys()){
            QSharedPointer<MapData> map = dataset->GetMapData(key);
            if (map->global_gradient_key() == range_key)
                maps_using_range << map;
        }
    }
    return maps_using_range;
}

const mat & DataModel::GetResultsMatrix(const QString &dataset_key,
                                       const QString &results_key,
                                       const QString &matrix_key) const
{
    if (HasDataset(dataset_key)){
        if (GetDataset(dataset_key)->AnalysisResultsKeys().contains(results_key)){
            if (GetDataset(dataset_key)->GetAnalysisResult(results_key)->KeyList().contains(matrix_key))
                return GetDataset(dataset_key)->GetAnalysisResultMatrix(results_key, matrix_key);
            else
                throw invalid_argument("Analysis Result"
                                       + results_key.toStdString()
                                       + " does not have matrix"
                                       + matrix_key.toStdString());

        }
        else{
            throw invalid_argument("Dataset "
                                   + dataset_key.toStdString()
                                   + " does not have result "
                                   + results_key.toStdString());
        }
    }
    else{
        throw invalid_argument("Dataset "
                               + dataset_key.toStdString()
                               + " does not exist!");
    }
}

const mat & DataModel::GetResultsMatrix(const QStringList &keys) const
{
    if (keys.size() != 3) throw invalid_argument("Wrong number of keys provided");
    return GetResultsMatrix(keys[0], keys[1], keys[2]);
}

///
/// \brief DataModel::GetCoreMatrix
/// \param dataset_key
/// \param matrix_key
/// \return The core matrix (spectra_, abscissa_, x_, y_ with key matrix_key from dataset with key dataset_key
/// Throws invalid_argument if invalid core matrix name given
const mat & DataModel::GetCoreMatrix(const QString &dataset_key, const QString &matrix_key) const
{
    if (HasDataset(dataset_key)){
        if (matrix_key == "Spectra" || matrix_key == "Spectral Abscissa"
                || matrix_key == "x" || matrix_key == "y")
            return GetDataset(dataset_key)->GetCoreMatrix(matrix_key);
        else
            throw invalid_argument("Dataset "
                                   + dataset_key.toStdString()
                                   + " does not have core matrix "
                                   + matrix_key.toStdString());
    }
    else{
        throw invalid_argument("Dataset "
                               + dataset_key.toStdString()
                               + " does not exist!");
    }
}

const mat & DataModel::GetCoreMatrix(const QStringList &keys) const
{
    if (keys.size() != 2) throw invalid_argument("Wrong number of keys provided");
    return GetCoreMatrix(keys[0], keys[1]);
}

///
/// \brief DataModel::GetAuxiliaryMatrix
/// \param dataset_key
/// \param matrix_key
/// \return reference to the auxiliary matrix named matrix_key in the dataset named dataset_key
/// Throws invalid argument if invalid auxiliary matrix name given
const mat & DataModel::GetAuxiliaryMatrix(const QString &dataset_key,
                                         const QString &matrix_key) const
{
    if (HasDataset(dataset_key)){
        if (GetDataset(dataset_key)->AuxiliaryMatrixKeys().contains(matrix_key))
            return GetDataset(dataset_key)->GetAuxiliaryMatrix(matrix_key);
        else
            throw invalid_argument("Dataset "
                                   + dataset_key.toStdString()
                                   + " does not have auxiliary matrix "
                                   + matrix_key.toStdString());
    }
    else{
        throw invalid_argument("Dataset " + dataset_key.toStdString()
                               + " does not exist!");
    }
}

const mat & DataModel::GetAuxiliaryMatrix(const QStringList &keys) const
{
    if (keys.size() != 2) throw invalid_argument("Wrong number of keys provided");
    return GetAuxiliaryMatrix(keys[0], keys[1]);
}

const mat & DataModel::GetMatrix(const QString &dataset_key, const QString &matrix_key) const
{
    if (HasDataset(dataset_key)){
        if (GetDataset(dataset_key)->CoreMatrixKeys().contains(matrix_key))
            return GetDataset(dataset_key)->GetCoreMatrix(matrix_key);
        else if (GetDataset(dataset_key)->AuxiliaryMatrixKeys().contains(matrix_key))
            return GetDataset(dataset_key)->GetAuxiliaryMatrix(matrix_key);
        else
            throw invalid_argument("Dataset "
                                   + dataset_key.toStdString()
                                   + " does not have matrix "
                                   + matrix_key.toStdString());
    }
    else{
        throw invalid_argument("Dataset "
                               + dataset_key.toStdString()
                               + "does not exist!");
    }
}

const mat & DataModel::GetMatrix(const QStringList &keys) const
{
    if (keys.size() == 2) return GetMatrix(keys[0], keys[1]);
    if (keys.size() == 3) return GetResultsMatrix(keys);
    throw invalid_argument("too few keys provided");
}

bool DataModel::Mappable(const QStringList &keys) const
{
    uword spatial_rows = GetDataset(keys[0])->x_ptr()->n_rows;
    uword data_rows = GetMatrix(keys).n_rows;
    return (data_rows == spatial_rows);
}

bool DataModel::Plottable(const QStringList &keys) const
{
    uword abscissa_rows = GetDataset(keys[0])->abscissa_ptr()->n_rows;
    uword data_rows = GetMatrix(keys).n_rows;
    return (data_rows == abscissa_rows);
}

///
/// \brief DataModel::DatasetNames
/// \return A list of the names of the datasets handled by this model
///
QStringList DataModel::DatasetNames() const
{
    QStringList dataset_names;
    for (auto dataset: datasets_)
        dataset_names << dataset->name();
    return dataset_names;
}

///
/// \brief DataModel::AnalysisResultsNames
/// \param dataset_key
/// \return
/// Throws invalid_argument if dataset doesn't exist
QStringList DataModel::AnalysisResultsNames(const QString &dataset_key) const
{
    if (HasDataset(dataset_key))
        return GetDataset(dataset_key)->AnalysisResultsKeys();
    else
        throw invalid_argument("Dataset "
                               + dataset_key.toStdString()
                               + "does not exist!");
}

///
/// \brief DataModel::AuxiliaryMatrixNames
/// \param dataset_key
/// \return
/// Throws invalid_argument if dataset doesn't exist
QStringList DataModel::AuxiliaryMatrixNames(const QString &dataset_key) const
{
    if (HasDataset(dataset_key))
        return GetDataset(dataset_key)->AuxiliaryMatrixKeys();
    else
        throw invalid_argument("Dataset "
                               + dataset_key.toStdString()
                               + "does not exist!");
}

QStringList DataModel::CoreMatrixNames(const QString &dataset_key)
{
    if (HasDataset(dataset_key))
        return GetDataset(dataset_key)->CoreMatrixKeys();
    else
        throw invalid_argument("Dataset "
                               + dataset_key.toStdString()
                               + "does not exist!");
}

///
/// \brief DataModel::AddDataset
/// \param dataset
/// Refuses to add a dataset if a dataset with that name already exists
/// will throw invalid_argument.
/// VespucciWorkspace should handle it.
void DataModel::AddDataset(QSharedPointer<VespucciDataset> dataset)
{
    int i = 1;
    QString name = dataset->name();
    while (HasDataset(dataset->name()))
        dataset->SetName(name + "(" + QString::number(i++) + ")");
    datasets_.append(dataset);
}

///
/// \brief DataModel::RemoveDataset
/// \param name
///
void DataModel::RemoveDataset(const QString &name)
{
    for (int i = 0; i < datasets_.size(); ++i)
        if (datasets_.at(i)->name() == name) datasets_.removeAt(i);
}

const mat & DataModel::EmptyMatrix() const
{
    return empty_matrix_;
}

bool DataModel::HasDataset(const QString &key) const
{
    for (int i = 0; i < datasets_.size(); ++i)
        if (datasets_.at(i)->name() == key) return true;
    return false;
}

bool DataModel::HasMatrix(const QStringList &keys) const
{
    if (!HasDataset(keys.first())) return false;

    QSharedPointer<VespucciDataset> dataset = GetDataset(keys.first());

    if (keys.size() == 2)
        return dataset->AuxiliaryMatrixKeys().contains(keys.last());

    if (keys.size() == 3 && dataset->AnalysisResultsKeys().contains(keys[1]))
        return dataset->GetAnalysisResult(keys[1])->HasMatrix(keys[2]);

    return false;
}

