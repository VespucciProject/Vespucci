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
#ifndef VCADATA_H
#define VCADATA_H
#include "Data/Analysis/analysisresults.h"
#include <QVector>
using namespace arma;
///
/// \brief The VCAData class
/// A class for performing and storing data from Vertex Components Analysis
class VCAData: public AnalysisResults
{
public:
    VCAData(QString name);
    void Apply(mat spectra, int endmembers);
    colvec Results(const uword component);
    QVector<double> EndmemberQVec(const uword i);
    vec Endmember(const uword i);
    mat EndmembersAsRows(uvec indices);
    int NumberComponents();
    double EndmemberMin(const uword i);
    double EndmemberMax(const uword i);
    mat *value(QString key);
    QStringList KeyList() const override;
    const mat &GetMatrix(const QString &key) override;
    QMap<QString, QString> GetMetadata() override;
    QString GetColumnHeading(const QString &key, int column) override;
private:
    QString name_;

    ///
    /// \brief endmember_spectra_
    /// Spectra of the endmembers (stored as columns)
    mat endmember_spectra_;

    ///
    /// \brief projected_data_
    /// "Projected data". Not particularly useful. Will be same size as spectra_
    mat projected_data_;

    ///
    /// \brief fractional_abundances_
    /// Fractional abundances of each endmember for each line
    mat fractional_abundances_;
    ///
    /// \brief indices_
    /// Indices of endmembers
    uvec indices_;

    ///
    /// \brief indices_mat_
    /// For VespucciTableModel
    mat indices_mat_;
};

#endif // VCADATA_H
