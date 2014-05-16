#include "principalcomponentsdata.h"

PrincipalComponentsData::PrincipalComponentsData(SpecMap *parent,
                                                 QString *directory,
                                                 mat coeff,
                                                 mat score,
                                                 vec latent,
                                                 vec tsquared,
                                                 int map_list_index)
{
    coeff_ = coeff;
    score_ = score;
    latent_ = latent;
    tsquared_ = tsquared;
    parent_ = parent;
    directory_ = directory;
    map_list_index_ = map_list_index;
}
