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
