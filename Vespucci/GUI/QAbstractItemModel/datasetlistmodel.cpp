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
#include "datasetlistmodel.h"

DatasetListModel::DatasetListModel(QObject *parent, QStringList dataset_names)
{
    dataset_names_ = dataset_names;
}

int DatasetListModel::rowCount(const QModelIndex &parent) const
{
    return dataset_names_.size();
}

QVariant DatasetListModel::data(const QModelIndex &index, int role) const
{
    if (role != Qt::DisplayRole){
        return QVariant();
    }
    else{
        int row = index.row();
        return QVariant(dataset_names_[row]);
    }
}

QString DatasetListModel::DatasetName(int row)
{
    return dataset_names_[row];
}
