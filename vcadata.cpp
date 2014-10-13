#include "vcadata.h"

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
    arma_ext::VCA(trans(spectra),
                  endmembers,
                  indices_,
                  endmember_spectra_,
                  projected_data_,
                  fractional_abundances_);
}

///
/// \brief VCAData::Endmember
/// \param i Index of endmember column
/// \return Plottable QVector of an endmember
///
QVector<double> VCAData::Endmember(const uword i)
{
    std::vector<double> spectrum_std;
    if (i >= endmember_spectra_.n_cols)
        spectrum_std = conv_to<std::vector<double> >::from(endmember_spectra_.col(i));
    else
        spectrum_std = conv_to<std::vector<double> >::from(endmember_spectra_.col(endmember_spectra_.n_cols - 1));
    return QVector<double>::fromStdVector(spectrum_std);
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
