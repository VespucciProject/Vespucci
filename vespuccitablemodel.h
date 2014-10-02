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
#ifndef VESPUCCITABLEMODEL_H
#define VESPUCCITABLEMODEL_H

#include <QAbstractTableModel>
#include "vespucciworkspace.h"
#include "vespuccidataset.h"
class VespucciDataset;
class VespucciTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit VespucciTableModel(QObject *parent, mat *input_data);
    VespucciTableModel(QObject *parent, QSharedPointer<VespucciDataset> dataset, QString type);
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    mat* GetData();

signals:

public slots:

private:
    int rows_;
    int cols_;
    mat *data_;
    QSharedPointer<VespucciDataset> dataset_;
    bool composite_;
    bool spatial_only_;

};

#endif // VESPUCCITABLEMODEL_H
