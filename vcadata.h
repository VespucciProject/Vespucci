#ifndef VCADATA_H
#define VCADATA_H
#include "vespuccidataset.h"
class VespucciDataset;
using namespace arma;
///
/// \brief The VCAData class
/// A class for performing and storing data from Vertex Components Analysis
class VCAData
{
public:
    VCAData(QSharedPointer<VespucciDataset> parent,
            QString *directory);
    void Apply(mat spectra, int endmembers);
    colvec Results(const uword component);
    QVector<double> Endmember(const uword i);
    mat *endmember_spectra();
    mat *projected_data();
    mat *fractional_abundances();
    int NumberComponents();
    double EndmemberMin(const uword i);
    double EndmemberMax(const uword i);
private:
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
    /// \brief parent_
    /// The current dataset
    QSharedPointer<VespucciDataset> parent_;

    ///
    /// \brief directory_
    /// Global working directory
    QString *directory_;

};

#endif // VCADATA_H
