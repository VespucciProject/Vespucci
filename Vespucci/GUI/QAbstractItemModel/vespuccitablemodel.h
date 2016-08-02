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
#ifndef VESPUCCITABLEMODEL_H
#define VESPUCCITABLEMODEL_H

#include <QAbstractTableModel>
#include "Global/vespucciworkspace.h"
#include "Data/Dataset/vespuccidataset.h"
class VespucciDataset;

///
/// \brief The VespucciTableModel class
/// The QAbstractTableModel that handles armadillo objects for the DataViewer
/// class.
class VespucciTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    VespucciTableModel(QObject *parent, const mat & input_data, const QStringList &data_keys);
    VespucciTableModel(QObject *parent, const mat & input_data, const uword &start_column, const QStringList &data_keys);
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    const mat& GetMatrix() const;
    bool SaveMatrix(const QString &filename, const QString &extension) const;
    uword MatrixColumns();
    QStringList data_keys();

signals:

public slots:

private:
    ///
    /// \brief start_column_
    /// Only 15 columns are dispalyed at a time. The start column decides which
    /// is the first column to display.
    uword start_column_;
    ///
    /// \brief data_
    /// Pointer to the object being displayed
    const mat & data_;

    QStringList data_keys_;
};

#endif // VESPUCCITABLEMODEL_H
