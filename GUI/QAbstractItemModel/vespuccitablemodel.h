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
*******************************************************************************/#ifndef VESPUCCITABLEMODEL_H
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
    explicit VespucciTableModel(QObject *parent, const mat *input_data);
    VespucciTableModel(QObject *parent, QSharedPointer<VespucciDataset> dataset, QString type);
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    const mat* GetData() const;

signals:

public slots:

private:
    ///
    /// \brief rows_
    /// Number of rows of the current object displayed
    int rows_;

    ///
    /// \brief cols_
    /// Number of columns of the current object being displayed
    int cols_;

    ///
    /// \brief data_
    /// Pointer to the object being displayed
    const mat *data_;

    ///
    /// \brief dataset_
    /// Dataset containing the object being displayed
    QSharedPointer<VespucciDataset> dataset_;

    ///
    /// \brief composite_
    /// Whether or not current display is a composite of multiple objects from
    /// the dataset
    bool composite_;

    ///
    /// \brief spatial_only_
    /// Whether or not the current data object only contains spatial data
    bool spatial_only_;

};

#endif // VESPUCCITABLEMODEL_H
