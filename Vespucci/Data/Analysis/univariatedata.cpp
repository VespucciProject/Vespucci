#include "Data/Analysis/univariatedata.h"

UnivariateData::UnivariateData(QString name):
    AnalysisResults(name, "Univariate Analysis Results")
{

}

void UnivariateData::Apply(double left_bound, double right_bound, uword bound_window, const mat &spectra, const vec &abscissa)
{
    cout << "UnivariateData::Apply" << endl;

    mat baselines;
    field<mat> inflection_baselines;

    mat results = Vespucci::Math::Quantification::QuantifyPeakMat(spectra,
                                                               abscissa,
                                                               left_bound, right_bound,
                                                               bound_window, baselines,
                                                               inflection_baselines);


    AddMetadata("Type", "One-region");
    AddMetadata("Range", QString::number(left_bound) + "–" + QString::number(right_bound));
    AddMetadata("Search window size", QString::number(bound_window));
    mat inflection_points(spectra.n_cols, 2);
    for (arma::uword i = 0; i < spectra.n_cols; ++i){
        inflection_points(i, 0) = inflection_baselines(i).col(0).min();
        inflection_points(i, 1) = inflection_baselines(i).col(0).max();
    }


    QStringList headings({ "Peak Centers",
                           "Peak Intensities",
                           "Adjusted Peak Intensities",
                           "Total Area",
                           "Adjusted Area",
                           "Area Between Inflection Points",
                           "Adjusted Area Between Inflection Points",
                           "Empirical Full-Width at Half-Maximum"});

    AddMatrix("Results", results, headings);
    AddMatrix("Inflection Points", inflection_points);
    AddMatrix("Baslines", baselines);

}

void UnivariateData::Apply(double first_left_bound, double first_right_bound, double second_left_bound, double second_right_bound, uword bound_window, const mat &spectra, const vec &abscissa)
{
    mat first_baselines, second_baselines;
    field<mat> inflection_first_baselines, inflection_second_baselines;
    mat first_results = Vespucci::Math::Quantification::QuantifyPeakMat(spectra,
                                                               abscissa,
                                                               first_left_bound, first_right_bound,
                                                               bound_window, first_baselines,
                                                               inflection_first_baselines);
    mat second_results = Vespucci::Math::Quantification::QuantifyPeakMat(spectra,
                                                                      abscissa,
                                                                      second_left_bound, second_right_bound,
                                                                      bound_window, second_baselines,
                                                                      inflection_second_baselines);
    mat first_inflection_points(spectra.n_cols, 2);
    mat second_inflection_points(spectra.n_cols, 2);

    for (arma::uword i = 0; i < spectra.n_cols; ++i){
        first_inflection_points(i, 0) = inflection_first_baselines(i).col(0).min();
        first_inflection_points(i, 1) = inflection_first_baselines(i).col(0).max();
        second_inflection_points(i,0) = inflection_second_baselines(i).col(0).min();
        second_inflection_points(i,1) = inflection_second_baselines(i).col(0).max();
    }

    mat ratios = first_results / second_results;
    QStringList headings({ "Peak Centers",
                           "Peak Intensities",
                           "Adjusted Peak Intensities",
                           "Total Area",
                           "Adjusted Area",
                           "Area Between Inflection Points",
                           "Adjusted Area Between Inflection Points",
                           "Empirical Full-Width at Half-Maximum"});

    AddMetadata("Type", "Band ratio analysis");
    AddMetadata("First region range", QString::number(first_left_bound) + "–" + QString::number(first_right_bound));
    AddMetadata("Second region range", QString::number(second_left_bound) + "–" + QString::number(second_right_bound));
    AddMatrix("First Region Results", first_results, headings);
    AddMatrix("Second Region Results", second_results, headings);
    AddMatrix("Band Ratios", ratios, headings);
    AddMatrix("First Region Baselines", first_baselines);
    AddMatrix("Second Region Baselines", second_baselines);
    AddMatrix("First Region Inflection Points", first_inflection_points);
    AddMatrix("Second Region Inflection Points", second_inflection_points);

}

void UnivariateData::ApplyCorrelation(const mat &spectra, const vec &control)
{
    AddMetadata("Type", "Correlation analysis");
    mat results = Vespucci::Math::Quantification::CorrelationMat(spectra, control);
    AddMatrix("Correlation Coefficients", results);
}

///
/// \brief UnivariateData::Calibrate
/// \param x Concentration (or other factor)
/// \param y Intensity (or other such value as contained in the original dataset)
/// \param column
/// Perform linear calibration. X is the concentration, y the intensity.
/// Will ignore column if this is a correlation
void UnivariateData::Calibrate(const vec &x, const vec &y, uword column)
{
    if (!HasMatrix("Band Ratios") && !HasMatrix("Results") && !HasMatrix("Correlation Coefficients"))
        return;
    if (column >= GetMatrix("Band Ratios").n_cols || column >= GetMatrix("Results").n_cols || column >= GetMatrix("Correlation Coefficients").n_cols)
        return;

    vec data;
    if (HasMatrix("Band Ratios")) data = GetMatrix("Band Ratios").col(column);
    else if (HasMatrix("Correlation Coefficients")) data = GetMatrix("Correlation Coefficients").col(0);

}
