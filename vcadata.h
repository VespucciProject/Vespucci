#ifndef VCADATA_H
#define VCADATA_H
#include "specmap.h"
class SpecMap;
using namespace arma;
class VCAData
{
public:
    VCAData(SpecMap *parent,
            QString *directory);
    void Apply(mat spectra, int endmembers);
    colvec Results(int component);
    QVector<double> Endmember(int i);
    mat *endmember_spectra();
    mat *projected_data();
    mat *fractional_abundances();
    int NumberComponents();
    double EndmemberMin(int i);
    double EndmemberMax(int i);
private:
    mat endmember_spectra_;
    mat projected_data_;
    mat fractional_abundances_;
    uvec indices_;
    SpecMap *parent_;
    QString *directory_;

};

#endif // VCADATA_H
