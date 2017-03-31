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
#ifndef METAANALYZER_H
#define METAANALYZER_H
#include "Data/Analysis/abstractdataanalyzer.h"
#include "Data/Analysis/analysisresults.h"
#include "Global/vespucciworkspace.h"
///
/// \brief The MetaAnalyzer class
/// This class is used to perform analysis on "pseudo-datasets". This allows us
/// perform analysis on a concatentated spectra matrix from multiple datasets
/// (the MultiAnalyzer derived class) or on a single matrix in a single dataset
/// (the MatrixAnalyzer derived class). This class is not concerned with how
/// the data_ and abscissa_ members are initialized (this is handled by GetData
/// in derived classes).
class MetaAnalyzer : public AbstractDataAnalyzer
{
public:
    MetaAnalyzer(QSharedPointer<VespucciWorkspace> ws);
    ~MetaAnalyzer();

    void Univariate(const QString &name,
                    double &left_bound,
                    double &right_bound,
                    arma::uword bound_window);
    void PeakStatistics(const QString &name,
                        double &left_bound,
                        double &right_bound);
    void BandRatio(const QString &name,
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
                const QString &partition_policy,
                bool allow_empty,
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
    ///
    /// \brief AddAnalysisResults
    /// \param results
    /// \param matrices
    /// This is the method used by derived class to add analysis results to the
    /// relevant datasets. Derived classes must implement this on their own (it
    ///  is pure virtual after all)
    virtual void AddAnalysisResults(QSharedPointer<AnalysisResults> results, QStringList matrices) = 0;

    ///
    /// \brief GetData
    /// This is the method used to populate data_ in the derived classes
    virtual void GetData() = 0;

    virtual QString FindUniqueName(QString name) = 0;
protected:
    ///
    /// \brief workspace_
    /// The global workspace, used to obtain access to matrices
    QSharedPointer<VespucciWorkspace> workspace_;
    mat data_;
    vec abscissa_;
};

#endif // METAANALYZER_H
