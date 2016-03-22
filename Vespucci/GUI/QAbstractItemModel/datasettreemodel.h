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
#ifndef DATASETTREEMODEL_H
#define DATASETTREEMODEL_H

#include <QAbstractItemModel>
#include <GUI/QAbstractItemModel/treeitem.h>
class TreeItem;
class DatasetTreeItem;
class MatrixTreeItem;
class ImageTreeItem;
class AnalysisResultTreeItem;
class DatasetTreeModel : public QAbstractItemModel
{
public:
    DatasetTreeModel(QObject *parent, VespucciWorkspace *ws);
    ~DatasetTreeModel();
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QModelIndex parent(const QModelIndex &index) const Q_DECL_OVERRIDE;
    int rowCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;

    bool setData(const QModelIndex &index, const QVariant &value, int role) Q_DECL_OVERRIDE;
    bool insertRows(int row, int count,
                    const QModelIndex &parent = QModelIndex()) Q_DECL_OVERRIDE;
    bool removeRows(int row, int count,
                    const QModelIndex &parent = QModelIndex()) Q_DECL_OVERRIDE;
    void AddDataset(QSharedPointer<VespucciDataset> dataset_);
    QSharedPointer<VespucciDataset> DatasetAt(const QModelIndex &index);
    bool IsMatrix(const QModelIndex &index);
    bool IsDataset(const QModelIndex &index);
    bool IsMap(const QModelIndex &index);
    void ClearDatasets();
    void removeRow(const QModelIndex &index);
    TreeItem *getItem(const QModelIndex &index) const;


private:
    TreeItem *root_item_;

};

#endif // DATASETTREEMODEL_H
