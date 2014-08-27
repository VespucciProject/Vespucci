#include "vcadata.h"

VCAData::VCAData(SpecMap *parent,
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
    arma_ext::VCA(spectra,
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
QVector<double> VCAData::Endmember(int i)
{
    if (i >= endmember_spectra_.n_cols)
        i = endmember_spectra_.n_cols - 1;
    std::vector<double> spectrum_std = conv_to<std::vector<double> >::from(endmember_spectra_.col(i));
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

colvec VCAData::Results(int component)
{
    return fractional_abundances_.col(component);
}

double VCAData::EndmemberMax(int i)
{
    cout << "EndmemberMax" << endl;
    if (i >= endmember_spectra_.n_cols)
        i = endmember_spectra_.n_cols - 1;
    colvec endmember = endmember_spectra_.col(i);
    double max = endmember.max();
    return max;
}

double VCAData::EndmemberMin(int i)
{
    cout << "EndmemberMin" << endl;
    if (i >= endmember_spectra_.n_cols)
        i = endmember_spectra_.n_cols - 1;
    colvec endmember = endmember_spectra_.col(i);
    double min = endmember.min();
    return min;
}
