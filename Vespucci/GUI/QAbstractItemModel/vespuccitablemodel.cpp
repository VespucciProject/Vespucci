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
#include "GUI/QAbstractItemModel/vespuccitablemodel.h"
///
/// \brief VespucciTableModel::VespucciTableModel
/// \param parent See QDialog
/// \param iinput_data The matrix that is displayed by the model
///
VespucciTableModel::VespucciTableModel(QObject *parent, const mat & input_data, const QStringList &data_keys) :
    QAbstractTableModel(parent),
    data_(input_data),
    data_keys_(data_keys)
{
    start_column_ = 0;
}


VespucciTableModel::VespucciTableModel(QObject *parent, const mat & input_data, const uword &start_column, const QStringList &data_keys)
    : QAbstractTableModel(parent),
      data_(input_data),
      data_keys_(data_keys)
{
    start_column_ = start_column;
}

///
/// \brief VespucciTableModel::data
/// \param index The model index of the data to be displayed
/// \param role Will return empty for roles other than Qt::DisplayRole
/// \return
/// See QAbstractTableModel::data()
QVariant VespucciTableModel::data(const QModelIndex &index, int role) const
{
    if (role != Qt::DisplayRole){
        return QVariant();
    }
    int row = index.row();
    int column = index.column() + start_column_;
    return QString::number(data_.at(row, column));
}

///
/// \brief VespucciTableModel::columnCount
/// \param parent
/// \return
/// See QAbstractTableModel::columnCount()
int VespucciTableModel::columnCount(const QModelIndex &parent) const
{
    //we don't need to use the model index here (our model only uses one object)

    //for objects with many columns, we only display the first 16 columns
    //otherwise we take up too much memory
    //if user wants to view that many columns, she can save the object
    return (data_.n_cols <= 15 ? data_.n_cols : 15);
}

///
/// \brief VespucciTableModel::rowCount
/// \param parent
/// \return
/// See QAbstractTableModel::rowCount
int VespucciTableModel::rowCount(const QModelIndex &parent) const
{
    //we don't need to use the model index here (our model only uses one object)
    return data_.n_rows;
}

///
/// \brief VespucciTableModel::GetMatrix
/// \return
/// Get a reference to the data
const mat & VespucciTableModel::GetMatrix() const
{
    return data_;
}

bool VespucciTableModel::SaveMatrix(const QString &filename, const QString &extension) const
{
    if (extension == "bin")
        return data_.save(filename.toStdString(), raw_binary);
    else if (extension == "csv")
        return data_.save(filename.toStdString(), csv_ascii);
    else if (extension == "txt")
        return data_.save(filename.toStdString(), raw_ascii);
    else
        return false;
}

///
/// \brief VespucciTableModel::MatrixColumns
/// \return
/// Called by DataWidget to assertain how many views have to be provided
uword VespucciTableModel::MatrixColumns()
{
    return data_.n_cols;
}

QStringList VespucciTableModel::data_keys()
{
    return data_keys_;
}
