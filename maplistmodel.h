/*******************************************************************************
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
*******************************************************************************/

#ifndef MAPLISTMODEL_H
#define MAPLISTMODEL_H

#include <QAbstractListModel>
#include "vespucciworkspace.h"

class VespucciWorkspace;
class MapData;
class VespucciDataset;

///
/// \brief The MapListModel class Exposes the UI to the contents of the master map list
///
class MapListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    MapListModel(QObject *parent, VespucciDataset *dataset);
    MapListModel();
    int rowCount(const QModelIndex &parent) const;
    bool removeRow(int row, const QModelIndex &parent);
    bool AddMap(QSharedPointer<MapData> map);
    QSharedPointer<MapData> MapAt(int row);
    QVariant data(const QModelIndex &index, int role) const;
    void ClearMaps();

signals:

public slots:

private:
    //This view displays the names of maps stored
    ///
    /// \brief maps_ The master map list in the VespucciDataset object
    QList<QSharedPointer<MapData> > maps_;
    VespucciDataset *dataset_;
};

#endif // MAPLISTMODEL_H
