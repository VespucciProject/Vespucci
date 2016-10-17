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
#ifndef MULTIANALYZER_H
#define MULTIANALYZER_H
#include "Global/vespucciworkspace.h"
#include "Data/Analysis/metaanalyzer.h"

using namespace std;
using namespace arma;
///
/// \brief The MultiAnalyzer class
/// This class handles the execution of multivariate analysis across multiple
/// datasets. Each analysis method will create an AnalysisResults object smart
/// pointer that is shared by all datasets used, plus an auxiliary matrix in each
/// dataset for data that corresponds to spectra.
class MultiAnalyzer : public MetaAnalyzer
{
public:
    MultiAnalyzer(QSharedPointer<VespucciWorkspace> workspace, QStringList dataset_keys);
    ~MultiAnalyzer();
    void SNVNormalize(double offset);
    void QUIC_SVD(double epsilon);
    void SVDDenoise(uword k);
private:
    bool CheckMergability(const QStringList dataset_keys);
    void GetDatasets(QStringList keys);
    bool ConcatenateSpectra(QStringList dataset_keys);
    void GetData();
    void AddAnalysisResults(QSharedPointer<AnalysisResults> results, QStringList matrices);
    QString FindUniqueName(QString name);

    ///
    /// \brief start_indices_
    /// Where each dataset begins
    uvec start_indices_;
    ///
    /// \brief end_indices_
    /// Where each dataset ends
    uvec end_indices_;

    QVector<QSharedPointer<VespucciDataset> > datasets_;
    QStringList dataset_keys_;
};

#endif // MULTIANALYZER_H
