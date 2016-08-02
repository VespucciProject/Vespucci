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
#include "Data/Analysis/vcadata.h"

VCAData::VCAData(QString name):
    AnalysisResults(name, "VCA Results")
{

}

///
/// \brief VCAData::Apply
/// Performs vertex component analysis on this object
/// \param X
/// \param endmembers
///
void VCAData::Apply(const mat &spectra, int endmembers)
{
    uvec indices;
    mat projected_data, fractional_abundances, endmember_spectra, indices_mat;
    Vespucci::Math::DimensionReduction::VCA(spectra,
                  endmembers,
                  indices,
                  endmember_spectra,
                  projected_data,
                  fractional_abundances);
    indices_mat.set_size(indices.n_elem, 1);

    for (uword i = 0; i < indices.n_elem; ++i)
        indices_mat(i, 0) = indices(i);

    QStringList em_col_headings;
    for (uword i = 1; i <= uword(endmembers); ++i)
        em_col_headings << "Endmember " + QString::number(i);

    AddMetadata("Endmembers calculated", QString::number(endmembers));
    AddMatrix("Endmembers", endmember_spectra, em_col_headings);
    AddMatrix("Fractional Abundances", fractional_abundances, em_col_headings);
    AddMatrix("Pure Pixel Indices", indices_mat, QStringList());
    AddMatrix("Projected Data", projected_data, QStringList());
}


