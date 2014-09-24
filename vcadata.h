#ifndef VCADATA_H
#define VCADATA_H
#include "vespuccidataset.h"
class VespucciDataset;
using namespace arma;
class VCAData
{
public:
    VCAData(VespucciDataset *parent,
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
    mat endmember_spectra_;
    mat projected_data_;
    mat fractional_abundances_;
    uvec indices_;
    VespucciDataset *parent_;
    QString *directory_;

};

#endif // VCADATA_H
