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
    if (datasets_.contains(key))
        return datasets_[key];
    else
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
    if (datasets_.contains(dataset_key)){
        if (datasets_[dataset_key]->AnalysisResultsKeys().contains(results_key))
            return datasets_[dataset_key]->GetAnalysisResult(results_key);
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
    if (datasets_.contains(dataset_key)){
        if (datasets_[dataset_key]->MapKeys().contains(map_key))
            return datasets_[dataset_key]->GetMapData(map_key);
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

const mat &DataModel::GetResultsMatrix(const QString &dataset_key,
                                       const QString &results_key,
                                       const QString &matrix_key) const
{
    if (datasets_.contains(dataset_key)){
        if (datasets_[dataset_key]->AnalysisResultsKeys().contains(results_key)){
            if (datasets_[dataset_key]->GetAnalysisResult(results_key)->KeyList().contains(matrix_key))
                return datasets_[dataset_key]->GetAnalysisResultMatrix(results_key, matrix_key);
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

const mat &DataModel::GetResultsMatrix(const QStringList &keys) const
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
const mat &DataModel::GetCoreMatrix(const QString &dataset_key, const QString &matrix_key) const
{
    if (datasets_.contains(dataset_key)){
        if (matrix_key == "Spectra" || matrix_key == "Spectral Absicssa"
                || matrix_key == "x" || matrix_key == "y")
            return datasets_[dataset_key]->GetCoreMatrix(matrix_key);
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

const mat &DataModel::GetCoreMatrix(const QStringList &keys) const
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
const mat &DataModel::GetAuxiliaryMatrix(const QString &dataset_key,
                                         const QString &matrix_key) const
{
    if (datasets_.contains(dataset_key)){
        if (datasets_[dataset_key]->AuxiliaryMatrixKeys().contains(matrix_key))
            return datasets_[dataset_key]->GetAuxiliaryMatrix(matrix_key);
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

const mat &DataModel::GetAuxiliaryMatrix(const QStringList &keys) const
{
    if (keys.size() != 2) throw invalid_argument("Wrong number of keys provided");
    return GetAuxiliaryMatrix(keys[0], keys[1]);
}

const mat &DataModel::GetMatrix(const QString &dataset_key, const QString &matrix_key) const
{
    if (datasets_.contains(dataset_key)){
        if (datasets_[dataset_key]->CoreMatrixKeys().contains(matrix_key))
            return datasets_[dataset_key]->GetCoreMatrix(matrix_key);
        else if (datasets_[dataset_key]->AuxiliaryMatrixKeys().contains(matrix_key))
            return datasets_[dataset_key]->GetAuxiliaryMatrix(matrix_key);
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

const mat &DataModel::GetMatrix(const QStringList &keys) const
{
    if (keys.size() == 2) return GetMatrix(keys[0], keys[1]);
    if (keys.size() == 3) return GetResultsMatrix(keys);
    throw invalid_argument("too few keys provided");
}

///
/// \brief DataModel::DatasetNames
/// \return A list of the names of the datasets handled by this model
///
QStringList DataModel::DatasetNames() const
{
    return datasets_.keys();
}

///
/// \brief DataModel::AnalysisResultsNames
/// \param dataset_key
/// \return
/// Throws invalid_argument if dataset doesn't exist
QStringList DataModel::AnalysisResultsNames(const QString &dataset_key) const
{
    if (datasets_.contains(dataset_key))
        return datasets_[dataset_key]->AnalysisResultsKeys();
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
    if (datasets_.contains(dataset_key))
        return datasets_[dataset_key]->AuxiliaryMatrixKeys();
    else
        throw invalid_argument("Dataset "
                               + dataset_key.toStdString()
                               + "does not exist!");
}

QStringList DataModel::CoreMatrixNames(const QString &dataset_key)
{
    if (datasets_.contains(dataset_key))
        return datasets_[dataset_key]->CoreMatrixKeys();
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
    if (!datasets_.contains(dataset->name()))
        datasets_[dataset->name()] = dataset;
}

///
/// \brief DataModel::RemoveDataset
/// \param name
///
void DataModel::RemoveDataset(const QString &name)
{
    if (datasets_.contains(name))
        datasets_.remove(name);
}

const mat &DataModel::EmptyMatrix() const
{
    return empty_matrix_;
}

