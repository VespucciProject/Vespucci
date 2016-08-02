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
#ifndef TESTAHCA_H
#define TESTAHCA_H
#include "Math/Clustering/agglomerativeclustering.h"
#include <QObject>
///
/// \brief The TestAHCA class
/// Tests for complete clustering assignments (every point assigned to a cluster)
/// and exclusive clustering assignments (every point is assigned to only one cluster)
class TestAHCA : public QObject
{
    Q_OBJECT
public:
    explicit TestAHCA(QObject *parent);

signals:

public slots:

private slots:
    void testCompleteClusters();
    void testExclusiveClusters();
private:
    std::vector<Vespucci::Math::Clustering::AHCA> ahca_obj;
};

#endif // TESTAHCA_H
