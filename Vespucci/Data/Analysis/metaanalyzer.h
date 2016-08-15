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
#ifndef METAANALYZER_H
#define METAANALYZER_H

#include "Global/vespucciworkspace.h"
class MetaAnalyzer
{
public:
    MetaAnalyzer(QSharedPointer<VespucciWorkspace> ws, const QStringList &data_keys, bool transpose);
    void ClassicalLeastSquares(const QString &name,
                               const QString &reference_key);
    void VertexComponents(const QString &name,
                          uword endmembers);
    void KMeans(size_t clusters,
                const QString &metric_text,
                const QString &name);
    void PrincipalComponents(const QString &name);
    void PrincipalComponents(const QString &name,
                             bool scale_data);
    void PartialLeastSquares(const QString &name,
                             uword components);
    void AgglomerativeClustering(const QString &name,
                                 const QString &metric,
                                 const QString &linkage);

private:
    void GetData();
    QString FindUniqueName(QString name);
    void AddResults(QSharedPointer<AnalysisResults> results, QStringList matrices);
    QSharedPointer<VespucciWorkspace> workspace_;
    QStringList data_keys_;
    mat data_;
    vec abscissa_;
    bool transpose_;
};

#endif // METAANALYZER_H
