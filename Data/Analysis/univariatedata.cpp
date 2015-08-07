#include "Data/Analysis/univariatedata.h"
#include "Math/Fitting/linleastsq.h"

UnivariateData::UnivariateData(QSharedPointer<VespucciDataset> parent, QString name) : baselines_(2)
{
    parent_ = parent;
    name_ = name;
    calibration_stats_["Calibrated"] = 0;
    calibrated_ = false;
}

UnivariateData::UnivariateData(QSharedPointer<VespucciDataset> parent, QString name, vec control)
{
    control_ = control;
    parent_ = parent;
    name_ = name;
    calibration_stats_["Calibrated"] = 0;
    calibrated_ = false;
}

///
/// \brief UnivariateData::Apply
/// \param left_bound Approximate start of peak
/// \param right_bound Approximate end of peak
/// \param method Method used
/// Performs the non band ratio univariate analysis. The left and right bounds
/// are adjusted by the determination functions.
void UnivariateData::Apply(double left_bound,
                      double right_bound,
                      UnivariateMethod::Method method)
{
    left_bound_ = left_bound;
    right_bound_ = right_bound;
    method_ = method;
    vec positions;
    switch (method_){
    case UnivariateMethod::Area :
        results_ = Vespucci::Math::Quantification::IntegratePeakMat(parent_->spectra(),
                                              parent_->wavelength(),
                                              left_bound_, right_bound_,
                                              first_baselines_, boundaries_);

        method_description_ = "Univariate Area";
        break;
    case UnivariateMethod::FWHM :
        results_ = Vespucci::Math::Quantification::FindBandwidthMat(parent_->spectra(),
                                              parent_->wavelength(),
                                              left_bound_, right_bound_,
                                              midlines_, first_baselines_, boundaries_);
        method_description_ = "Univariate Bandwidth";
        break;
    case UnivariateMethod::Correlation :
        results_ = Vespucci::Math::Quantification::CorrelationMat(parent_->spectra(), control_);
        break;
    case UnivariateMethod::SignalNoise :
    case UnivariateMethod::Intensity : default :
        results_ = Vespucci::Math::Quantification::FindPeakMaxMat(parent_->spectra(),
                                            parent_->wavelength(),
                                            left_bound, right_bound,
                                            positions);
        positions_.set_size(positions.n_elem, 1);
        positions_.col(0) = positions;
        method_description_ = "Univariate Intensity";
        break;
    }

}

void UnivariateData::Apply(double first_left_bound,
                           double first_right_bound,
                           double second_left_bound,
                           double second_right_bound,
                           UnivariateMethod::Method method)
{
    first_left_bound_ = first_left_bound;
    second_left_bound_ = second_left_bound;
    first_right_bound_ = first_right_bound;
    second_right_bound_ = second_right_bound;
    method_ = method;
    mat results;
    switch (method_){
    case UnivariateMethod::AreaRatio:
        results = Vespucci::Math::Quantification::IntegratePeaksMat(parent_->spectra(),
                                              parent_->wavelength(),
                                              first_left_bound_, first_right_bound_,
                                              second_left_bound_, second_right_bound_,
                                              first_baselines_, second_baselines_,
                                              boundaries_);

        method_description_ = "Band Ratio Area";
        break;
    case UnivariateMethod::IntensityRatio: default:
        results = Vespucci::Math::Quantification::FindPeakMaxesMat(parent_->spectra(),
                                             parent_->wavelength(),
                                             first_left_bound_, first_right_bound_,
                                             second_left_bound_, second_right_bound_,
                                             positions_);
        method_description_ = "Band Ratio Intensity";
        break;
    }
    results_ = results.col(0) / results.col(1);
}

///
/// \brief UnivariateData::Calibrate
/// \param values Intensity (or band ratio, or areas) of calibration curve)
/// \param conentrations
///
void UnivariateData::Calibrate(const vec &values, const vec &concentrations)
{
    mat X = Vespucci::Math::LinLeastSq::Vandermonde(concentrations, 1);
    vec calibration_y;
    vec coefs = Vespucci::Math::LinLeastSq::OrdinaryLeastSquares(X,
                                                                 values,
                                                                 calibration_y,
                                                                 calibration_stats_);

    calibration_stats_["Calibrated"] = 1;
    vec residuals = values - calibration_y;
    calibration_curve_ = join_horiz(concentrations, join_horiz(calibration_y, residuals));

    double b = coefs(0);
    double m = coefs(1);
    results_.transform([m, b](double val){return (val - b)/m;});
}


vec UnivariateData::results() const
{
    return results_;
}

const vec *UnivariateData::results_ptr() const
{
    return &results_;
}

bool UnivariateData::band_ratio() const
{
    return band_ratio_;
}

double UnivariateData::left_bound() const
{
    return left_bound_;
}

double UnivariateData::right_bound() const
{
    return right_bound_;
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

QString UnivariateData::MethodDescription() const
{
    return method_description_;
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

mat UnivariateData::Midlines() const
{
    return midlines_;
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
