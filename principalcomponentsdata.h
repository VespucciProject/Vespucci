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
#ifndef PRINCIPALCOMPONENTSDATA_H
#define PRINCIPALCOMPONENTSDATA_H

#include "specmap.h"
using namespace arma;

class SpecMap;
class PrincipalComponentsData {
public:
    PrincipalComponentsData(SpecMap *parent, QString *directory);

    void Apply(mat spectra);

    bool SaveCoefficientMatrix();

    colvec Results(int component);

    vec *tsquared();
    mat *score();
    vec *latent();
    mat *coeff();

private:
    mat coeff_;
    mat score_;
    vec latent_;
    vec tsquared_;
    SpecMap *parent_;
    QString *directory_;

};

#endif // PRINCIPALCOMPONENTSDATA_H
