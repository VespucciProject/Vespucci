#include "Data/Analysis/univariatedata.h"

UnivariateData::UnivariateData(QString name)
{
    name_ = name;
    calibration_stats_["Calibrated"] = 0;
    calibrated_ = false;
}

UnivariateData::UnivariateData(QString name, vec control)
{
    control_ = control;
    name_ = name;
    calibration_stats_["Calibrated"] = 0;
    calibrated_ = false;
    band_ratio_ = false;
    correlation_ = true;
}

void UnivariateData::Apply(double left_bound, double right_bound, uword bound_window, const mat &spectra, const vec &abscissa)
{
    correlation_ = false;
    band_ratio_ = false;
    bound_window_ = bound_window;
    first_left_bound_ = left_bound;
    first_right_bound_ = right_bound;

    first_results_ = Vespucci::Math::Quantification::QuantifyPeakMat(spectra,
                                                               abscissa,
                                                               left_bound, right_bound,
                                                               bound_window, first_baselines_,
                                                               inflection_first_baselines_);

    band_ratio_ = false;
    uword spectrum_count = spectra.n_cols;
    first_inflection_points_.set_size(spectrum_count, 2);
    for (arma::uword i = 0; i < spectrum_count; ++i){
        first_inflection_points_(i, 0) = inflection_first_baselines_(i).col(0).min();
        first_inflection_points_(i, 1) = inflection_first_baselines_(i).col(0).max();
    }

}

void UnivariateData::Apply(double first_left_bound, double first_right_bound, double second_left_bound, double second_right_bound, uword bound_window, const mat &spectra, const vec &abscissa)
{
    correlation_ = false;
    band_ratio_ = true;
    bound_window_ = bound_window;
    first_left_bound_ = first_left_bound;
    first_right_bound_ = first_right_bound;
    second_right_bound_ = second_right_bound;
    second_left_bound_ = second_left_bound;

    first_results_ = Vespucci::Math::Quantification::QuantifyPeakMat(spectra,
                                                               abscissa,
                                                               first_left_bound_, first_right_bound_,
                                                               bound_window_, first_baselines_,
                                                               inflection_first_baselines_);
    second_results_ = Vespucci::Math::Quantification::QuantifyPeakMat(spectra,
                                                                      abscissa,
                                                                      second_left_bound_, second_right_bound_,
                                                                      bound_window_, second_baselines_,
                                                                      inflection_second_baselines_);
    uword spectrum_count = spectra.n_cols;
    first_inflection_points_.set_size(spectrum_count, 2);
    second_inflection_points_.set_size(spectrum_count, 2);
    for (arma::uword i = 0; i < spectrum_count; ++i){
        first_inflection_points_(i, 0) = inflection_first_baselines_(i).col(0).min();
        first_inflection_points_(i, 1) = inflection_first_baselines_(i).col(0).max();
        second_inflection_points_(i,0) = inflection_second_baselines_(i).col(0).min();
        second_inflection_points_(i,1) = inflection_second_baselines_(i).col(0).max();
    }
    band_ratio_ = true;
}

void UnivariateData::ApplyCorrelation(const mat &spectra)
{
    correlation_ = true;
    first_results_ = Vespucci::Math::Quantification::CorrelationMat(spectra, control_);
}


mat UnivariateData::results() const
{
    return first_results_;
}

const mat *UnivariateData::results_ptr() const
{
    return &first_results_;
}

bool UnivariateData::band_ratio() const
{
    return band_ratio_;
}

double UnivariateData::left_bound() const
{
    return first_left_bound_;
}

double UnivariateData::right_bound() const
{
    return first_right_bound_;
}

double UnivariateData::first_left_bound() const
{
    return first_left_bound_;
}

double UnivariateData::first_right_bound() const
{
    return first_right_bound_;
}

double UnivariateData::second_left_bound() const
{
    return second_left_bound_;
}

double UnivariateData::second_right_bound() const
{
    return second_right_bound_;
}


mat UnivariateData::calibration_curve() const
{
    return calibration_curve_;
}

const mat *UnivariateData::calibration_curve_ptr() const
{
    return &calibration_curve_;
}

const map<string, double> UnivariateData::calibration_stats() const
{
    return calibration_stats_;
}

mat UnivariateData::first_baselines() const
{
    return first_baselines_;
}

mat UnivariateData::second_baselines() const
{
    return second_baselines_;
}

uvec UnivariateData::Boundaries() const
{
    return boundaries_;
}

void UnivariateData::SetName(QString name)
{
    name_ = name;
}

QString UnivariateData::name() const
{
    return name_;
}

bool UnivariateData::calibrated() const
{
    return calibrated_;
}

field<mat> UnivariateData::CalculateRatios()
{
    field<mat> objects(8);
    mat matrix;
    vec ratio;
    objects(0) = join_horiz(first_results_.col(0), second_results_.col(0));
    //Peak center ratios don't make sense, so we don't calculate them
    for (uword i = 1; i < first_results_.n_cols; ++i){
        vec ratio = first_results_.col(i) / second_results_.col(i);
        matrix = join_horiz(first_results_.col(i), second_results_.col(i));
        matrix = join_horiz(matrix, ratio);
        objects(i) = matrix;
    }
    return objects;
}

QSharedPointer<AnalysisResults> UnivariateData::GetResults()
{
    QString type;
    if (band_ratio_)
        type = "Two-peak Univariate Analysis Data";
    else if (correlation_)
        type = "Correlation Analysis Data";
    else
        type = "Univariate Analysis Data";

    QSharedPointer<AnalysisResults> results(new AnalysisResults(name_, type));
    QStringList column_headings;
    if (band_ratio_){
        mat matrix = {{first_left_bound_, first_right_bound_},
                      {second_left_bound_, second_right_bound_}};
        results->AppendObject("Boundaries", matrix);
        field<mat> objects = CalculateRatios();
        results->AppendObject("Peak Centers", objects(0), column_headings);
        results->AppendObject("Peak Intensities", objects(1), column_headings);
        results->AppendObject("Adjusted Peak Intensities", objects(2), column_headings);
        results->AppendObject("Total Area", objects(3), column_headings);
        results->AppendObject("Adjusted Area", objects(4), column_headings);
        results->AppendObject("Area Between Inflection Points", objects(5), column_headings);
        results->AppendObject("Adjusted Area Between Inflection Points", objects(6), column_headings);
        results->AppendObject("Empirical Full-Width at Half-Maximum", objects(7), column_headings);
    }
    else if (correlation_){
        results->AppendObject("Correlation Coefficients", first_results_);
    }
    else{
        results->AppendObject("Peak Centers", first_results_.col(0), column_headings);
        results->AppendObject("Peak Intensities", first_results_.col(1), column_headings);
        results->AppendObject("Adjusted Peak Intensities", first_results_.col(2), column_headings);
        results->AppendObject("Total Area", first_results_.col(3), column_headings);
        results->AppendObject("Adjusted Area", first_results_.col(4), column_headings);
        results->AppendObject("Area Between Inflection Points", first_results_.col(5), column_headings);
        results->AppendObject("Adjusted Area Between Inflection Points", first_results_.col(6), column_headings);
        results->AppendObject("Empirical Full-Width at Half-Maximum", first_results_.col(7),column_headings);
    }

    return results;
}




