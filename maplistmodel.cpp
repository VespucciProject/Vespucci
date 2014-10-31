/*******************************************************************************
    Copyright (C) 2014 Wright State University - All Rights Reserved
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
#include "maplistmodel.h"

MapListModel::MapListModel(QObject *parent, VespucciDataset *dataset) :
    QAbstractListModel(parent)
{
    dataset_ = dataset;
}

int MapListModel::rowCount(const QModelIndex &parent) const
{
    return maps_.size();
}

///
/// \brief MapListModel::data
/// \param index
/// \param role The role (will always be Qt::DisplayRole (to display map name)
/// \return
/// A re-implementation of QAbstractItemModel::data();
QVariant MapListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= maps_.size())
        return QVariant();
    else if (role == Qt::DisplayRole)
        return QVariant(maps_.at(index.row())->name());
    else
        return QVariant();
}

bool MapListModel::removeRow(int row, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row);
    maps_.removeAt(row);
    endRemoveRows();
    emit dataChanged(parent, parent);
    return true;
}

bool MapListModel::AddMap(QSharedPointer<MapData> map)
{
    int row = maps_.size();
    QModelIndex index = createIndex(row, 0);
    beginInsertRows(index, row, row);
    maps_.append(map);
    endInsertRows();
    emit dataChanged(index, index);
    return true;
}

QSharedPointer<MapData> MapListModel::MapAt(int row)
{
    return maps_.at(row);
}

///
/// \brief MapListModel::ClearMaps
/// Clears the map container. Used when closing the program.
void MapListModel::ClearMaps()
{
    maps_.clear();
}
