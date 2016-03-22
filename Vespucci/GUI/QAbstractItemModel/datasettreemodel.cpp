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
#include "datasettreemodel.h"

DatasetTreeModel::DatasetTreeModel(QObject *parent, VespucciWorkspace *ws)
    : QAbstractItemModel(parent)
{
    QList<QVariant> root_data;
    root_data << "Title" << "Summary";
    root_item_ = new TreeItem(root_data);
}

DatasetTreeModel::~DatasetTreeModel()
{
    delete root_item_;
}

QVariant DatasetTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    if(role != Qt::DisplayRole)
        return QVariant();
    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
    return item->data(index.column());
}

Qt::ItemFlags DatasetTreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;
    return QAbstractItemModel::flags(index);
}

QVariant DatasetTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return root_item_->data(section);
    return QVariant();
}

QModelIndex DatasetTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if(!hasIndex(row, column, parent))
        return QModelIndex();
    TreeItem *parent_item;
    if (!parent.isValid())
        parent_item = root_item_;
    else
        parent_item = static_cast<TreeItem*>(parent.internalPointer());

    TreeItem *child_item = parent_item->child(row);
    if (child_item)
        return createIndex(row, column, child_item);
    else
        return QModelIndex();
}

QModelIndex DatasetTreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    TreeItem *child_item = static_cast<TreeItem*>(index.internalPointer());
    TreeItem *parent_item = child_item->parentItem();

    if (parent_item == root_item_)
        return QModelIndex();
    return createIndex(parent_item->row(), 0, parent_item);
}

int DatasetTreeModel::rowCount(const QModelIndex &parent) const
{
    TreeItem *parent_item = getItem(parent);
    return parent_item->childCount();
}

int DatasetTreeModel::columnCount(const QModelIndex &parent) const
{
    return root_item_->columnCount();
}

bool DatasetTreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{

}


bool DatasetTreeModel::removeRows(int row, int count, const QModelIndex &parent)
{
    TreeItem *parent_item = getItem(parent);
    bool success = true;
    beginRemoveRows(parent, row, count + row - 1);
    endRemoveRows();
    return success;
}

void DatasetTreeModel::AddDataset(QSharedPointer<VespucciDataset> dataset)
{
    QModelIndex last_index = index(rowCount(QModelIndex()) - 1, 0);
    beginInsertRows(last_index, root_item_->childCount(), root_item_->childCount());
    root_item_->appendChild(new DatasetTreeItem(dataset, root_item_));
    endInsertRows();
}

QSharedPointer<VespucciDataset> DatasetTreeModel::DatasetAt(const QModelIndex &index)
{
    //this function should only be called if current index is a dataset
    DatasetTreeItem *item = dynamic_cast<DatasetTreeItem*>(getItem(index));
    return item->dataset();
}

bool DatasetTreeModel::IsMatrix(const QModelIndex &index)
{
    return getItem(index)->is_matrix();
}

bool DatasetTreeModel::IsDataset(const QModelIndex &index)
{
    return getItem(index)->is_dataset();
}

bool DatasetTreeModel::IsMap(const QModelIndex &index)
{
    return getItem(index)->is_plot();
}

void DatasetTreeModel::ClearDatasets()
{
    int row_count = rowCount(index(0,0));
    beginRemoveRows(index(0,0), 0, row_count - 1);
    root_item_->ClearChildren();
    endRemoveRows();
}

void DatasetTreeModel::removeRow(const QModelIndex &index)
{
    beginRemoveRows(index, index.row(), index.row());
    removeRows(index.row(), 1, index);
    endRemoveRows();
}

TreeItem *DatasetTreeModel::getItem(const QModelIndex &index) const
{
    if (index.isValid()){
        TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
        if (item){return item;}
    }
    return root_item_;
}
