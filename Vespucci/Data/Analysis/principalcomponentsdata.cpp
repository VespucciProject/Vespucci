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
PrincipalComponentsData::PrincipalComponentsData(QString name)
{
    name_ = name;
}

///
/// \brief PrincipalComponentsData::Apply
/// \param spectra
/// Perform analysis using arma::princomp
void PrincipalComponentsData::Apply(const mat &spectra)
{
    princomp(coeff_, score_, latent_, tsquared_, spectra.t());
    double eigenvalue_sum = sum(latent_);
    percent_variance_ = latent_ / eigenvalue_sum;
    percent_variance_ /= 0.01;
}

///
/// \brief PrincipalComponentsData::Results
/// \param component
/// \return
/// Return the score for a particular column
colvec PrincipalComponentsData::Results(int component)
{
    return score_.col(component);
}

vec* PrincipalComponentsData::tsquared()
{
    return &tsquared_;
}

mat* PrincipalComponentsData::coeff()
{
    return &coeff_;
}

vec* PrincipalComponentsData::latent()
{
    return &latent_;
}

mat* PrincipalComponentsData::score()
{
    return &score_;
}

vec* PrincipalComponentsData::percent_variance()
{
    return &percent_variance_;
}

mat *PrincipalComponentsData::value(QString key)
{
    if (key == "PCA Projected Data (Scores)")
        return &score_;
    else if (key == "PCA Coefficients (Loadings)")
        return &coeff_;
    else if (key == "PCA Hotelling t² Values" || "PCA Hotelling t^2 Values" || "PCA Hotelling t-squared Values")
        return (mat *) &tsquared_;
    else if (key == "PCA Eigenvalues of Covariance Matrix")
        return (mat *) &latent_;
    else if (key == "PCA Percent Variance")
        return (mat *) &percent_variance_;
    else
        return NULL;
}

QSharedPointer<AnalysisResults> PrincipalComponentsData::GetResults()
{
    QSharedPointer<AnalysisResults> results(new AnalysisResults(name_, "PCA Data"));
    QStringList column_headings;
    for (int i = 1; i < 16; ++i)
        column_headings << "Loading " + QString::number(i);
    results->AppendObject("Scores", score_, column_headings);;
    column_headings.clear();
    for (int i = 1; i < 16; ++i)
        column_headings << "Loading " + QString::number(i);
    results->AppendObject("Loadings", coeff_, column_headings);;
    column_headings.clear();
    column_headings << "Eigenvalues";
    results->AppendObject("Eigenvalues", latent_, column_headings);;
    column_headings.clear();
    column_headings << "Percent Variance";
    results->AppendObject("Percent Variance", percent_variance_, column_headings);;
    column_headings.clear();
    column_headings << "Hotelling t²";
    results->AppendObject("Hotelling t² Values", tsquared_, column_headings);;
    return results;
}
