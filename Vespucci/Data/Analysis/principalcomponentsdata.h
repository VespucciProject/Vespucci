/*******************************************************************************
    Copyright (C) 2015 Wright State University - All Rights Reserved
    Daniel P. Foose - Author

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
#ifndef PRINCIPALCOMPONENTSDATA_H
#define PRINCIPALCOMPONENTSDATA_H

#include "Data/Analysis/analysisresults.h"
#include "Data/Dataset/vespuccidataset.h"

using namespace arma;
class VespucciDataset;

///
/// \brief The PrincipalComponentsData class
/// A class for performing and storing data from principal components analysis
class PrincipalComponentsData: public AnalysisResults
{
public:
    PrincipalComponentsData(QString name);
    void Apply(const mat &spectra);
};

#endif // PRINCIPALCOMPONENTSDATA_H
