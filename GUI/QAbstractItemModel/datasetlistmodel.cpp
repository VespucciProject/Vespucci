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
#include "GUI/QAbstractItemModel/datasetlistmodel.h"

DatasetListModel::DatasetListModel(QObject *parent, VespucciWorkspace *ws) :
    QAbstractListModel(parent)
{
    workspace = ws;
    workspace->SetDatasets(&datasets_);
}

int DatasetListModel::rowCount(const QModelIndex &parent) const
{
    return datasets_.size();
}

///
/// \brief DatasetListModel::data
/// \param index
/// \param role The role (will always be Qt::DisplayRole (to display dataset name)
/// \return
/// A re-implementation of QAbstractItemModel::data();
QVariant DatasetListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= datasets_.size())
        return QVariant();
    else if (role == Qt::DisplayRole)
        return QVariant(datasets_.at(index.row())->name());
    else
        return QVariant();
}

bool DatasetListModel::removeRow(int row, const QModelIndex &parent)
{
    cout << "DatasetListModel::removeRow()" << endl;
    beginRemoveRows(parent, row, row);
    datasets_.at(row)->DestroyLogFile();
    datasets_.removeAt(row);
    endRemoveRows();
    emit dataChanged(parent, parent);
    return true;
}

bool DatasetListModel::AddDataset(QSharedPointer<VespucciDataset> dataset)
{
    int row = datasets_.size();
    QModelIndex index = createIndex(row, 0);
    beginInsertRows(index, row, row);
    datasets_.append(dataset);
    endInsertRows();
    emit dataChanged(index, index);
    emit DatasetAdded(index);
    workspace->UpdateCount();
    return true;
}

QSharedPointer<VespucciDataset> DatasetListModel::DatasetAt(int row)
{
    return datasets_.at(row);
}

///
/// \brief DatasetListModel::ClearMapss
/// Clears the dataset container. Used when closing the program.
void DatasetListModel::ClearDatasets()
{
    //This is done instead of datasets_.clear() to make sure that the log file
    //destructor is called for every dataset.
    while (datasets_.size() > 0)
        removeRow(0, QModelIndex());
}
