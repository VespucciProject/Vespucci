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
#ifndef DATASETLISTMODEL_H
#define DATASETLISTMODEL_H

#include <QAbstractListModel>
#include "vespucciworkspace.h"

class VespucciWorkspace;

///
/// \brief The DatasetListModel class Exposes the UI to the contents of the master dataset list
///
class DatasetListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    DatasetListModel(QObject *parent, VespucciWorkspace *ws);
    int rowCount(const QModelIndex &parent) const;
    bool removeRow(int row, const QModelIndex &parent);
    bool AddDataset(QSharedPointer<VespucciDataset> dataset);
    QSharedPointer<VespucciDataset> DatasetAt(int row);
    QVariant data(const QModelIndex &index, int role) const;
    void ClearDatasets();

signals:
    void DatasetAdded(const QModelIndex &index);

public slots:

private:
    //This view displays the names of datasets stored
    ///
    /// \brief datasets_ The master dataset list in the workspace
    ///
    QList<QSharedPointer<VespucciDataset> > datasets_;
    VespucciWorkspace *workspace;
};

#endif // DATASETLISTMODEL_H
