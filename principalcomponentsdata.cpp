/************************************************************************************
    Copyright (C) 2014 Daniel P. Foose - All Rights Reserved

    This file is part of Vespucci.

    Vespucci is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Vespucci is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Vespucci.  If not, see <http://www.gnu.org/licenses/>.
***************************************************************************************/
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

mat* PrincipalComponentsData::tsquared()
{
    return &tsquared_;
}
