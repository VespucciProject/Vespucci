/*******************************************************************************
    Copyright (C) 2014 Wright State University - All Rights Reserved
    Daniel P. Foose - Author

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

VCAData::VCAData(QSharedPointer<VespucciDataset> parent,
                 QString *directory)
{
    parent_ = parent;
    directory_ = directory;
}

///
/// \brief VCAData::Apply
/// Performs vertex component analysis on this object
/// \param X
/// \param endmembers
///
void VCAData::Apply(mat spectra, int endmembers)
{
    Vespucci::Math::DimensionReduction::VCA(spectra,
                  endmembers,
                  indices_,
                  endmember_spectra_,
                  projected_data_,
                  fractional_abundances_);
    indices_mat_.set_size(indices_.n_elem, 1);
    for (uword i = 0; i < indices_.n_elem; ++i)
        indices_mat_(i, 0) = indices_(i);
}

///
/// \brief VCAData::Endmember
/// \param i Index of endmember column
/// \return Plottable QVector of an endmember
///
QVector<double> VCAData::EndmemberQVec(const uword i)
{
    vec spectrum = Endmember(i);
    std::vector<double> spectrum_std = conv_to<std::vector<double> >::from(spectrum);
    return QVector<double>::fromStdVector(spectrum_std);
}

///
/// \brief VCAData::Endmember
/// \param i
/// \return
/// Get a vector corresponding to a single endmember
vec VCAData::Endmember(const uword i)
{
    if (i >= endmember_spectra_.n_cols)
        return endmember_spectra_.col(endmember_spectra_.n_cols - 1);
    else
        return endmember_spectra_.col(i);
}

///
/// \brief VCAData::endmember_spectra
/// \return Pointer to the endmember spectra matrix
///
mat *VCAData::endmember_spectra()
{
    return &endmember_spectra_;
}

///
/// \brief VCAData::fractional_abundances
/// \return Pointer to fractional abundances matrix
///
mat *VCAData::fractional_abundances()
{
    return &fractional_abundances_;
}

///
/// \brief VCAData::projected_data
/// \return Pointer to projected data matrix
///
mat *VCAData::projected_data()
{
    return &projected_data_;
}

int VCAData::NumberComponents()
{
    return endmember_spectra_.n_cols;
}

colvec VCAData::Results(const uword component)
{
    return fractional_abundances_.col(component);
}

double VCAData::EndmemberMax(const uword i)
{
    colvec endmember;
    double max;
    try{
        if (i >= endmember_spectra_.n_cols)
            endmember = endmember_spectra_.col(endmember_spectra_.n_cols - 1);
        else
            endmember = endmember_spectra_.col(i);
        max = endmember.max();
    }
    catch(std::exception e){
        throw std::runtime_error("VCAData::EndmemberMax");
    }

    return max;
}

mat *VCAData::value(QString key)
{
    if (key == "VCA Endmembers")
        return &endmember_spectra_;
    else if (key == "VCA Fractional Abundances")
        return &fractional_abundances_;
    else if (key == "VCA Pure Pixel Indices")
        return (mat *) &indices_;
    else if (key == "VCA Projected Data")
        return &projected_data_;
    else
        return NULL;
}

///
/// \brief VCAData::EndmemberMin
/// \param i Index of a column of the endmember_spectra_ matrix.
/// \return The minimum value of the endmember in column i
///
double VCAData::EndmemberMin(const uword i)
{
    colvec endmember;
    double min;
    try{
        if (i >= endmember_spectra_.n_cols)
            endmember = endmember_spectra_.col(endmember_spectra_.n_cols - 1);
        else
            endmember = endmember_spectra_.col(i);
        min = endmember.min();
    }
    catch(std::exception e){
        throw std::runtime_error("Improper VCAData member function call");
    }

    return min;
}

///
/// \brief VCAData::EndmembersAsRows
/// \param indices A list of indices of desired endmemebrs
/// \return Endmember spectra in the same format used in the spectra_ matrix
/// This is used in the MetaDataset constructor to grab specific endmembers.
mat VCAData::EndmembersAsRows(uvec indices)
{
    mat endmembers;
    try{
        endmembers = endmember_spectra_.cols(indices);
    }
    catch(std::exception e){
        throw std::runtime_error("VCAData::EndmembersAsRows");
    }

    return trans(endmembers);
}

mat *VCAData::indices()
{
    return &indices_mat_;
}
