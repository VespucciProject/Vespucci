#include "Data/Analysis/univariatedata.h"

UnivariateData::UnivariateData(QString name):
    AnalysisResults(name, "Univariate Analysis Results"),
    name_(name)
{
    calibration_stats_["Calibrated"] = 0;
    calibrated_ = false;
}

UnivariateData::UnivariateData(QString name, vec control):
    AnalysisResults(name, "Univariate Analysis Results"),
    control_(control),
    name_(name)
{
    control_ = control;
    calibration_stats_["Calibrated"] = 0;
    calibrated_ = false;
    band_ratio_ = false;
    correlation_ = true;
}

void UnivariateData::Apply(double left_bound, double right_bound, uword bound_window, const mat &spectra, const vec &abscissa)
{
    cout << "UnivariateData::Apply" << endl;
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
    cout << "results.n_cols :" << first_results_.n_cols;
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
    cout << "results columns" << first_results_.n_cols;
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

QStringList UnivariateData::KeyList() const
{
    if (band_ratio_) return QStringList({"First Region Results", "Second Region Results"});
    else if (correlation_) return QStringList({"Correlation Coefficients"});
    else return QStringList({"Results"});
}

const mat &UnivariateData::GetMatrix(const QString &name)
{
    if (name == "Results"
            || name == "First Region Results"
            || name == "Correlation Coefficients")
        return first_results_;
    else if (name == "Second Region Results") return second_results_;
    else return EmptyMatrix();
}

QMap<QString, QString> UnivariateData::GetMetadata()
{
    return metadata_;
}

///
/// \brief UnivariateData::GetColumnHeading
/// \param key
/// \param column
/// \return
/// Will throw exception if column out of bounds (should always check first)
QString UnivariateData::GetColumnHeading(const QString &key, int column)
{
    if ((correlation_ && column > 1)
            || (key == "First Region Results" && column >= first_results_.n_cols)
            || (key == "Second Region Results" && column >= second_results_.n_cols)){
        throw invalid_argument("Column out of bounds");
    }
    QStringList headings({
                           "Peak Centers",
                           "Peak Intensities",
                           "Adjusted Peak Intensities",
                           "Total Area",
                           "Adjusted Area",
                           "Area Between Inflection Points",
                           "Adjusted Area Between Inflection Points",
                           "Empirical Full-Width at Half-Maximum"});

    if (key == "Correlation Coefficients"){return "Correlation Coefficients";}
    else if (key == "First Region Results" || key == "Second Region Results"
             || key == "Results"){return headings[column];}
    else{return QString();}
}




