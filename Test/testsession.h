/*******************************************************************************
    Copyright (C) 2014-2016 Wright State University - All Rights Reserved
    Daniel P. Foose - Maintainer/Lead Developer

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
*******************************************************************************/
#ifndef TESTSESSION_H
#define TESTSESSION_H
#include "../Vespucci/Data/Dataset/vespuccidataset.h"
#include "test.h"
using namespace std;
using namespace arma;
///
/// \brief The TestSession class
/// Loads mock datasets for testing classes.
class TestSession
{
public:
    TestSession();
    void LoadMockData(QString manifest_path);
    QSharedPointer<VespucciDataset> DatasetAt(int row);
    void GetData(mat &spectra, vec &abscissa, vec &x, vec &y);

    void GenerateMockData(mat &spectra, vec &abscissa, vec &x, vec &y, mat &peak_data, mat &baselines);
    vec GenerateLinearBaseline(double min, double max, uword size);
    vec GenerateRamanBaseline(double a0, double a1, double a2, const vec &abscissa);
    vec GenerateGaussian(vec abscissa, vec centers, vec fwhm, vec intensities);
    vec GenerateLorentzian(vec abscissa, vec centers, vec fwhm, vec intensities);
private:
    QVector<QSharedPointer<VespucciDataset> > datasets_;
    QVector<mat> spectra_;
    QVector<mat> abscissa_;
    QVector<mat> x_;
    QVector<mat> y_;

};

#endif // TESTSESSION_H
