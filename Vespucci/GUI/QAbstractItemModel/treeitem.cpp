/*******************************************************************************
    Copyright (C) 2014-2017 Wright State University - All Rights Reserved
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
#include "treeitem.h"

TreeItem::TreeItem(TreeItem::ItemType type, QStringList &keys, QList<QVariant> &data, TreeItem *parent)
{
    type_ = type;
    keys_ = keys;
    item_data_ = data;
    parent_item_ = parent;
}

TreeItem::~TreeItem()
{
    qDeleteAll(child_items_);
}

void TreeItem::appendChild(TreeItem *child)
{
    child_items_.append(child);
}

void TreeItem::removeChild(TreeItem *child)
{
    child_items_.removeOne(child);
}

TreeItem *TreeItem::child(int row)
{
    return child_items_.value(row);
}

int TreeItem::childCount() const
{
    return child_items_.count();
}

int TreeItem::columnCount() const
{
    return item_data_.count();
}

QVariant TreeItem::data(int column) const
{
    return item_data_.value(column);
}

int TreeItem::row() const
{
    if (parent_item_)
        return parent_item_->child_items().indexOf(const_cast<TreeItem*>(this));
    return 0;
}

TreeItem *TreeItem::parentItem()
{
    return parent_item_;
}

QList<TreeItem *> TreeItem::child_items()
{
    return child_items_;
}

void TreeItem::UpdateType(QString new_type)
{
    if (item_data_.size() > 1)
        item_data_[1] = QVariant(new_type);
}

void TreeItem::ClearChildren()
{
    child_items_.clear();
}

TreeItem::ItemType TreeItem::type() const
{
    return type_;
}

QStringList TreeItem::keys() const
{
    return keys_;
}

const QString TreeItem::DatasetKey() const
{
    return keys_[0];
}

QStringList TreeItem::ChildNames() const
{
    QStringList child_names;
    for (auto item: child_items_)
        child_names << item->data(0).toString();
    return child_names;
}

bool TreeItem::HasChild(const QString &name)
{
    for (auto child_item: child_items_)
        if (child_item->data(0) == name)
            return true;

    return false;
}

