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
#include "Data/Analysis/plsdata.h"

PLSData::PLSData(QString name):
    AnalysisResults(name, "Partial Least Squares Results")
{

}

///
/// \brief PLSData::Apply
/// \param spectra Input matrix
/// \param wavelength Spectral abscissa
/// \param components Number of components to calculate
/// \return
/// Performs PLS analysis on the spectra matrix.
bool PLSData::Classify(const mat &spectra, const vec &wavelength, int components)
{
    mat Y = repmat(wavelength, 1, components);
    mat X_loadings, Y_loadings, X_scores, Y_scores, coefficients, percent_variance, fitted;
    bool success = Vespucci::Math::DimensionReduction::plsregress(spectra, Y, components,
                                        X_loadings, Y_loadings,
                                        X_scores, Y_scores,
                                        coefficients, percent_variance,
                                        fitted);

    //mat residuals = fitted - spectra;
    if (success){
        AddMetadata("Type", "Classification (PCA)");
        AddMetadata("Components calculated", QString::number(components));
        AddMatrix("Percent Variance", percent_variance);
        AddMatrix("Predictor Loadings", X_loadings);
        AddMatrix("Response Loadings", Y_loadings);
        AddMatrix("Predictor Scores", X_scores);
        AddMatrix("Response Scores", Y_scores);
        AddMatrix("Coefficients", coefficients);
        AddMatrix("Fitted Data", fitted);
        //AddMatrix("Residuals", residuals);
    }

    return success;

}

bool PLSData::Calibrate(const mat &spectra, const mat &controls)
{
    //spectra is y
    //controls are X
    mat X_loadings, Y_loadings, X_scores, Y_scores, coefficients, percent_variance, fitted;
    bool success = Vespucci::Math::DimensionReduction::plsregress(controls, spectra, controls.n_cols,
                                                                  X_loadings, Y_loadings,
                                                                  X_scores, Y_scores,
                                                                  coefficients, percent_variance,
                                                                  fitted);

    inplace_trans(coefficients);
    //mat residuals = fitted - spectra;
    if (success){
        AddMetadata("Type", "Calibration");
        AddMetadata("Components calculated", QString::number(controls.n_cols));
        AddMatrix("Percent Variance", percent_variance);
        AddMatrix("Predictor Loadings", X_loadings);
        AddMatrix("Response Loadings", Y_loadings);
        AddMatrix("Predictor Scores", X_scores);
        AddMatrix("Response Scores", Y_scores);
        AddMatrix("Coefficients", coefficients);
        AddMatrix("Fitted Data", fitted);
        //AddMatrix("Residuals", residuals);
    }

    return success;
}

///
/// \brief PLSData::Discriminate
/// \param data
/// \param labels
/// \return
/// Perform PLS-DA
bool PLSData::Discriminate(const mat &data, const mat &labels)
{
    //data (usually, but not necessarly spectra) is X
    //labels are Y;
    if (labels.n_rows != data.n_cols) return false;
    mat X_loadings, Y_loadings, X_scores, Y_scores, coefficients, percent_variance, fitted;
    bool success = Vespucci::Math::DimensionReduction::plsregress(data.t(), labels, labels.n_cols,
                                                                  X_loadings, Y_loadings,
                                                                  X_scores, Y_scores,
                                                                  coefficients, percent_variance,
                                                                  fitted);
    mat residuals = fitted - labels;

    if (success){
        AddMetadata("Type", "Calibration");
        AddMetadata("Components calculated", QString::number(labels.n_cols));
        AddMatrix("Percent Variance", percent_variance);
        AddMatrix("Predictor Loadings", X_loadings);
        AddMatrix("Response Loadings", Y_loadings);
        AddMatrix("Predictor Scores", X_scores);
        AddMatrix("Response Scores", Y_scores);
        AddMatrix("Coefficients", coefficients);
        AddMatrix("Fitted Data", fitted);
        AddMatrix("Residuals", residuals);
    }

    return success;
}
