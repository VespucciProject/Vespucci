#include "principalcomponentsworker.h"

PrincipalComponentsWorker::PrincipalComponentsWorker(mat* coeff, mat* score, vec* latent, vec* tsquared, mat* X)
{
    //object is only used to execute arma::princomp_econ (which I added)
    coeff_ = coeff;
    score_ = score;
    latent_ = latent;
    tsquared_ = tsquared;
    X_ = X;
}

PrincipalComponentsWorker::~PrincipalComponentsWorker()
{
    //destructor
}

void PrincipalComponentsWorker::princomp()
{
    princomp_econ(*coeff_, *score_, *latent_, *tsquared_, *X_);
    emit finished();
}
