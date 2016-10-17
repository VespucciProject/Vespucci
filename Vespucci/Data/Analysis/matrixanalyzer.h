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
#ifndef MATRIXANALYZER_H
#define MATRIXANALYZER_H

#include "Global/vespucciworkspace.h"
#include "Data/Analysis/metaanalyzer.h"
class MatrixAnalyzer : public MetaAnalyzer
{
public:
    MatrixAnalyzer(QSharedPointer<VespucciWorkspace> ws, const QStringList &data_keys, bool transpose);
    ~MatrixAnalyzer();
private:
    void GetData();

    ///
    /// \brief FindUniqueName
    /// \param name
    /// \return
    /// Makes sure that there is a unique name for any new matrices/results
    QString FindUniqueName(QString name);
    void AddAnalysisResults(QSharedPointer<AnalysisResults> results, QStringList matrices);

    ///
    /// \brief transpose_
    /// Whether or not matrix should be transposed to perform analysis
    bool transpose_;

    ///
    /// \brief data_keys_
    /// Keys in the workspace for the matrix analyzed by this instance
    QStringList data_keys_;
};

#endif // MATRIXANALYZER_H
