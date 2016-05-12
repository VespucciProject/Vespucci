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
#include "genericanalysisresults.h"

GenericAnalysisResults::GenericAnalysisResults(const QString &name, const QString &type, QMap<QString, mat> matrices):
    AnalysisResults(name, type)
{
    matrices_ = matrices;
}

const mat &GenericAnalysisResults::GetMatrix(const QString &key)
{
    if (matrices_.contains(key)) return matrices_.value(key);
    else return EmptyMatrix();
}

QStringList GenericAnalysisResults::KeyList()
{
    return matrices_.keys();
}

QMap<QString, QString> GenericAnalysisResults::GetMetadata()
{
    return QMap<QString, QString>();
}

QString GenericAnalysisResults::GetColumnHeading(const QString &key, int column)
{
    return QString();
}
