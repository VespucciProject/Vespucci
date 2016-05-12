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
#ifndef GENERICANALYSISRESULTS_H
#define GENERICANALYSISRESULTS_H

#include "Data/Dataset/vespuccidataset.h"
#include "Data/Analysis/analysisresults.h"
using namespace std;
using namespace arma;

class GenericAnalysisResults : public AnalysisResults
{
public:
    GenericAnalysisResults(const QString &name, const QString &type, QMap<QString, mat> matrices);
    const mat &GetMatrix(const QString &key) override;
    QStringList KeyList() const override;
    QMap<QString, QString> GetMetadata() override;
    QString GetColumnHeading(const QString &key, int column) override;
private:
    QMap<QString, mat> matrices_;
    mat current_matrix_;
};

#endif // GENERICANALYSISRESULTS_H
