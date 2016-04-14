/*******************************************************************************
    Copyright (C) 2015 Wright State University - All Rights Reserved
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
#ifndef TREEITEM_H
#define TREEITEM_H

#include <QList>
#include <QVariant>
#include "Data/Dataset/vespuccidataset.h"
class MapData;
class VespucciDataset;
class TreeItem
{
public:
    enum ItemType{
        Dataset, AnalysisResult, Matrix, Map, Base
    };
    TreeItem(TreeItem::ItemType type, QStringList &keys, QList<QVariant> &data, TreeItem *parent = 0);
    ~TreeItem();
    void appendChild(TreeItem *child);
    void removeChild(TreeItem *child);
    TreeItem *child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    int row() const;
    TreeItem *parentItem();
    QList<TreeItem*> child_items();
    void ClearChildren();

    TreeItem::ItemType type() const;
    QStringList keys() const;
    const QString DatasetKey() const;
    QStringList ChildNames() const;

    bool HasChild(const QString &name);

    void UpdateType(QString new_type);



private:
    QList<TreeItem*> child_items_;
    QList<QVariant> item_data_;
    TreeItem *parent_item_;

    ///
    /// \brief keys_ Stores relevant keys to get the object in the datamodel
    /// keys_[0] = dataset name
    /// keys_[1] = analysis results name, matrix name or map name
    /// keys_[2] = analysis results matrix name
    QStringList keys_;

    ///
    /// \brief type_
    /// Type from the ItemType Enum
    TreeItem::ItemType type_;

};
#endif //TREEITEM_H
