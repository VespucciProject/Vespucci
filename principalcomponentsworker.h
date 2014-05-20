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
#ifndef PRINCIPALCOMPONENTSWORKER_H
#define PRINCIPALCOMPONENTSWORKER_H

#include <QObject>
#include "armadillo"
using namespace arma;
//a class that is a worker object for a QThread to perform principal component analysis
class PrincipalComponentsWorker : public QObject
{
    Q_OBJECT

public:
    PrincipalComponentsWorker(mat *coeff, mat *score, vec *latent, vec *tsquared, mat *X);
    ~PrincipalComponentsWorker();

public slots:
    void princomp();

signals:
    void finished();
    void error(QString err);

private:
    mat *coeff_;
    mat *score_;
    vec *latent_;
    vec *tsquared_;
    mat *X_;
};

#endif // PRINCIPALCOMPONENTSWORKER_H
