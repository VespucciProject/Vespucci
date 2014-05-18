#ifndef PRINCIPALCOMPONENTSDATA_H
#define PRINCIPALCOMPONENTSDATA_H

#include "specmap.h"
using namespace arma;

class SpecMap;
class PrincipalComponentsData {
public:
    PrincipalComponentsData(SpecMap *parent,
                            QString *directory,
                            mat coeff,
                            mat score,
                            vec latent,
                            vec tsquared);

    bool SaveCoefficientMatrix();
    bool SaveScoreMatrix();
    bool SaveLatentMatrix();
    bool SaveTSquaredMatrix();
    bool SaveReport(); //saves all as a text report
    bool SaveAll(); //saves all as MS Excel or HDF5

    colvec Results(int component);

private:
    mat coeff_;
    mat score_;
    vec latent_;
    vec tsquared_;
    SpecMap *parent_;
    QString *directory_;
};

#endif // PRINCIPALCOMPONENTSDATA_H
