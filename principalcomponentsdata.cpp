#include "principalcomponentsdata.h"

PrincipalComponentsData::PrincipalComponentsData(SpecMap *parent,
                                                 QString *directory,
                                                 mat coeff,
                                                 mat score,
                                                 vec latent,
                                                 vec tsquared)
{
    coeff_ = coeff;
    score_ = score;
    latent_ = latent;
    tsquared_ = tsquared;
    parent_ = parent;
    directory_ = directory;
}

colvec PrincipalComponentsData::Results(int component)
{
    return score_.col(component);
}
