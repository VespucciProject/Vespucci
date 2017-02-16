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
#ifndef TESTDATAMODEL_H
#define TESTDATAMODEL_H

#include <QObject>
#include <QtTest>
#include "Global/datamodel.h"


class TestDataModel : public QObject
{
    Q_OBJECT
public:
    explicit TestDataModel(QObject *parent = 0);

signals:

public slots:
private slots:
    void DatasetInsertion();
    void DatasetDeletion();
    void MatrixInsertion();
    void MatrixDeletion();
    void MapInsertion();
    void MapDeletion();
private:
    QSharedPointer<VespucciWorkspace> workspace_;
    QSharedPointer<DataModel> data_model_;
};

#endif // TESTDATAMODEL_H
