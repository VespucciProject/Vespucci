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
#ifndef ABSTRACTDATAANALYZER_H
#define ABSTRACTDATAANALYZER_H

#include <QString>
#include <QStringList>
#include <mlpack/core.hpp>
///
/// \brief The AbstractDataAnalyzer class
/// Pure abstract class describing an object that can be passed as a pointer to gui widgets.
class AbstractDataAnalyzer
{
public:
    virtual ~AbstractDataAnalyzer();
    virtual void Univariate(const QString &name,
                            double &left_bound,
                            double &right_bound,
                            arma::uword bound_window) = 0;
    virtual void FitPeak(const QString &name,
                         const QString &peak_shape,
                         double &left_bound,
                         double &right_bound) = 0;
    virtual void BandRatio(const QString &name,
                           double &first_left_bound,
                           double &first_right_bound,
                           double &second_left_bound,
                           double &second_right_bound,
                           arma::uword bound_window) = 0;
    virtual void ClassicalLeastSquares(const QString &name,
                                       const QStringList &reference_keys) = 0;
    virtual void VertexComponents(const QString &name,
                                  arma::uword endmembers) = 0;
    virtual void KMeans(const QString &name,
                        const QString &metric_text,
                        const QString &partition_policy,
                        bool allow_empty,
                        size_t clusters) = 0;
    virtual void PrincipalComponents(const QString &name) = 0;
    virtual void PrincipalComponents(const QString &name,
                                     bool scale_data) = 0;
    virtual void PartialLeastSquares(const QString &name,
                                     arma::uword components) = 0;
    virtual void PLSCalibration(const QString &name,
                                const QStringList &control_keys) = 0;
    virtual void TrainPLSDA(const QString &name,
                            const QStringList &label_keys) = 0;
    virtual void AgglomerativeClustering(const QString &name,
                                         const QString &metric,
                                         const QString &linkage) = 0;
    virtual size_t columns() const = 0;
    virtual double AbscissaMax() const = 0;
    virtual double AbscissaMin() const = 0;
    virtual arma::vec abscissa() const = 0;
    virtual arma::vec PointSpectrum(arma::uword index) const = 0;
};


#endif // ABSTRACTDATAANALYZER_H
