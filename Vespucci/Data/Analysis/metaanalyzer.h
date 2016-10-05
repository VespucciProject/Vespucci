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
#include "Data/Analysis/abstractdataanalyzer.h"
class MetaAnalyzer : public AbstractDataAnalyzer
{
public:
    MetaAnalyzer(QSharedPointer<VespucciWorkspace> ws, const QStringList &data_keys, bool transpose);
    ~MetaAnalyzer();
    virtual void Univariate(const QString &name,
                            double &left_bound,
                            double &right_bound,
                            arma::uword bound_window);
    virtual void BandRatio(const QString &name,
                           double &first_left_bound,
                           double &first_right_bound,
                           double &second_left_bound,
                           double &second_right_bound,
                           arma::uword bound_window);
    void ClassicalLeastSquares(const QString &name,
                               const QStringList &reference_keys);
    void VertexComponents(const QString &name,
                          uword endmembers);
    void KMeans(const QString &name,
                const QString &metric_text,
                size_t clusters);
    void PrincipalComponents(const QString &name);
    void PrincipalComponents(const QString &name,
                             bool scale_data);
    void PartialLeastSquares(const QString &name,
                             uword components);
    void PLSCalibration(const QString &name,
                        const QStringList &control_keys);
    void TrainPLSDA(const QString &name,
                    const QStringList &label_keys);
    void AgglomerativeClustering(const QString &name,
                                 const QString &metric,
                                 const QString &linkage);

    size_t columns() const;
    double AbscissaMin() const;
    double AbscissaMax() const;
    arma::vec abscissa() const;
    arma::vec PointSpectrum(arma::uword index) const;
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
