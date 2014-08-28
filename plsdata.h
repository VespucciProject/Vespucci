#ifndef PLSDATA_H
#define PLSDATA_H
#include "specmap.h"
using namespace arma;
class PLSData
{
public:
    PLSData(SpecMap *parent,
            QString *directory);

    bool Apply(mat spectra, rowvec wavelength, int components);

    mat *X_loadings();
    mat *Y_loadings();
    mat *X_scores();
    mat *Y_scores();
    mat *coefficients();
    mat *percent_variance();  

    int NumberComponents();
    colvec Results(int i, bool &valid);
private:
    mat Y_;
    mat X_loadings_;
    mat Y_loadings_;
    mat X_scores_;
    mat Y_scores_;
    mat coefficients_;
    mat percent_variance_;
    mat fitted_;
    SpecMap *parent_;
    QString *directory_;
};

#endif // PLSDATA_H
