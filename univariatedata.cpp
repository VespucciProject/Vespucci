#include "univariatedata.h"

UnivariateData::UnivariateData(QSharedPointer<VespucciDataset> parent) : baselines_(2)
{
    parent_ = parent;
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
        results_ = arma_ext::IntegratePeakMat(trans(parent_->spectra()),
                                              trans(parent_->wavelength()),
                                              left_bound_, right_bound_,
                                              first_baselines_, boundaries_);

        method_description_ = "Univariate Area";
        break;
    case UnivariateMethod::FWHM :
        results_ = arma_ext::FindBandwidthMat(trans(parent_->spectra()),
                                             trans(parent_->wavelength()),
                                             left_bound_, right_bound_,
                                             midlines_, first_baselines_, boundaries_);
        method_description_ = "Univariate Bandwidth";
        break;
    case UnivariateMethod::Intensity : default :
        results_ = arma_ext::FindPeakMaxMat(trans(parent_->spectra()),
                                            trans(parent_->wavelength()),
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
        results = arma_ext::IntegratePeaksMat(trans(parent_->spectra()),
                                               trans(parent_->wavelength()),
                                               first_left_bound_, first_right_bound_,
                                               second_left_bound_, second_right_bound_,
                                               first_baselines_, second_baselines_,
                                               boundaries_);

        method_description_ = "Band Ratio Area";
        break;
    case UnivariateMethod::IntensityRatio: default:
        results = arma_ext::FindPeakMaxesMat(trans(parent_->spectra()),
                                              trans(parent_->wavelength()),
                                              first_left_bound_, first_right_bound_,
                                              second_left_bound_, second_right_bound_,
                                              positions_);
        method_description_ = "Band Ratio Intensity";
        break;
    }
    results_ = results.col(0) / results.col(1);
}


vec UnivariateData::results()
{
    return results_;
}


bool UnivariateData::band_ratio()
{
    return band_ratio_;
}

double UnivariateData::left_bound()
{
    return left_bound_;
}

double UnivariateData::right_bound()
{
    return right_bound_;
}

double UnivariateData::first_left_bound()
{
    return first_left_bound_;
}

double UnivariateData::first_right_bound()
{
    return first_right_bound_;
}

double UnivariateData::second_left_bound()
{
    return second_left_bound_;
}

double UnivariateData::second_right_bound()
{
    return second_right_bound_;
}

QString UnivariateData::MethodDescription()
{
    return method_description_;
}

mat UnivariateData::first_baselines()
{
    return first_baselines_.t();
}

mat UnivariateData::second_baselines()
{
    return second_baselines_.t();
}

mat UnivariateData::Midlines()
{
    return midlines_;
}

uvec UnivariateData::Boundaries()
{
    return boundaries_;
}
