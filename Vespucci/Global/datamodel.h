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
#ifndef DATAMODEL_H
#define DATAMODEL_H
#include "Data/Dataset/vespuccidataset.h"
class MapData;
class DataModel
{
public:
    DataModel();
    QSharedPointer<VespucciDataset> GetDataset(const QString &key) const;
    QSharedPointer<AnalysisResults> GetResults(const QString &dataset_key,
                                               const QString &results_key)
                                               const;
    QSharedPointer<MapData> GetMap(const QString &dataset_key,
                                   const QString &map_key) const;

    const mat& GetResultsMatrix(const QString &dataset_key,
                          const QString &results_key,
                          const QString &matrix_key) const;
    const mat& GetCoreMatrix(const QString &dataset_key,
                             const QString &matrix_key) const;
    const mat& GetAuxiliaryMatrix(const QString &dataset_key,
                                  const QString &matrix_key) const;
    QStringList DatasetNames() const;
    QStringList AnalysisResultsNames(const QString &dataset_key) const;
    QStringList AuxiliaryMatrixNames(const QString &dataset_key) const;
    QStringList CoreMatrixNames(const QString &dataset_key);
    void AddDataset(QSharedPointer<VespucciDataset> dataset);
    void RemoveDataset(const QString &name);


private:
    QMap<QString, QSharedPointer<VespucciDataset> > datasets_;
};

#endif // DATAMODEL_H
