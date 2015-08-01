#include "Data/Analysis/univariatedata.h"
#include "Math/Fitting/linleastsq.h"

UnivariateData::UnivariateData(QSharedPointer<VespucciDataset> parent, QString name) : baselines_(2)
{
    parent_ = parent;
    name_ = name;
}

UnivariateData::UnivariateData(QSharedPointer<VespucciDataset> parent, QString name, vec control)
{
    control_ = control;
    parent_ = parent;
    name_ = name;
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
    if (values.n_rows != concentrations.n_rows){
        throw std::invalid_argument("Values and concentrations must have same size!");
    }

    double n = values.n_rows;
    double dof = n - 2; //for linear fit
    double x_bar = mean(concentrations);
    double y_bar = mean(values);

    mat X = Vespucci::Math::LinLeastSq::Vandermonde(concentrations, 1);
    vec coefs = Vespucci::Math::LinLeastSq::OrdinaryLeastSquares(X, values);
    vec fit = Vespucci::Math::LinLeastSq::CalcPoly(coefs, concentrations);
    calibration_curve_residuals_ = values - fit;
    vec centered = values - y_bar;
    double residual_sumsq = sum(pow(calibration_curve_residuals_, 2.0));
    double total_sumsq = sum(pow(centered, 2.0));
    double regression_sumsq = sum(pow((fit - y_bar), 2.0));
    double R_squared = 1.0 - (residual_sumsq/total_sumsq);
    double adj_R_squared = 1 - (1 - R_squared)*(n - 1)/dof; //p==1 for deg-1 polynomial

    mat var_hat = (residual_sumsq / dof) * inv(X.t() * X);

    double s_m = var_hat(1,1);
    double s_b = var_hat(0,0);
    double s_y = std::sqrt(regression_sumsq / n);
    double F = (regression_sumsq) / (residual_sumsq/dof);
    //format of calibration_stats_ is that of Excel's linest function, plus one extra line:
    // m        b
    // s_m      s_b
    // Rsq      s(y)
    // F        DOF
    // SSreg    SSres
    // adj Rsq  norm of residuals
    calibration_stats_.set_size(6, 2);
    calibration_stats_(0,0) = coefs(1);
    calibration_stats_(0,1) = coefs(0);
    calibration_stats_(1,0) = s_m;
    calibration_stats_(1,1) = s_b;
    calibration_stats_(2,0) = R_squared;
    calibration_stats_(2,1) = s_y;
    calibration_stats_(3,0) = F;
    calibration_stats_(3,1) = dof;
    calibration_stats_(4,0) = regression_sumsq;
    calibration_stats_(4,1) = residual_sumsq;
    calibration_stats_(5,0) = adj_R_squared;
    calibration_stats_(5,1) = std::sqrt(residual_sumsq);



}


vec UnivariateData::results() const
{
    return results_;
}

const vec *UnivariateData::results_ptr() const
{
    return &results_;
}

bool UnivariateData::band_ratio()
{
    return band_ratio_;
}

double UnivariateData::left_bound()
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

mat UnivariateData::calibration_stats() const
{
    return calibration_stats_;
}

const mat *UnivariateData::calibration_stats_ptr() const
{
    return &calibration_stats_;
}

mat UnivariateData::calibration_curve_residuals() const
{
    return calibration_curve_residuals_;
}

const mat *UnivariateData::calibration_curve_residuals_ptr() const
{
    return &calibration_curve_residuals_;
}

mat UnivariateData::first_baselines() const
{
    return first_baselines_;
}

mat UnivariateData::second_baselines()
{
    return second_baselines_;
}

mat UnivariateData::Midlines()
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
