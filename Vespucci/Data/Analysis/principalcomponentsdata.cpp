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
#include "Data/Analysis/principalcomponentsdata.h"

///
/// \brief PrincipalComponentsData::PrincipalComponentsData
/// \param parent Dataset
/// \param directory global directory
///
PrincipalComponentsData::PrincipalComponentsData(QString name):
    AnalysisResults(name, "PCA Results")
{

}

///
/// \brief PrincipalComponentsData::Apply
/// \param spectra
/// Perform analysis using arma::princomp
void PrincipalComponentsData::Apply(const mat &spectra)
{
    mat coeff, score;
    vec latent, tsquared, percent_variance;
    princomp(coeff, score, latent, tsquared, spectra.t());
    double eigenvalue_sum = sum(latent);
    percent_variance = latent / eigenvalue_sum;
    percent_variance /= 0.01;
    vec cumulative = cumsum(percent_variance);
    percent_variance = join_horiz(percent_variance, cumulative);

    AddMatrix("Scores", score);
    AddMatrix("Loadings", coeff);
    AddMatrix("Eigenvalues", latent);
    AddMatrix("Percent Variance", percent_variance);
    AddMatrix("Hotelling t²", tsquared);
}
