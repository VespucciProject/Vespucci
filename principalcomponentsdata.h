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

    mat *tsquared();

private:
    mat coeff_;
    mat score_;
    vec latent_;
    vec tsquared_;
    SpecMap *parent_;
    QString *directory_;
};

#endif // PRINCIPALCOMPONENTSDATA_H
