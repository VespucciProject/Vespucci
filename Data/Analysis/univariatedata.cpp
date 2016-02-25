#include "Data/Analysis/univariatedata.h"

UnivariateData::UnivariateData(QSharedPointer<VespucciDataset> parent, QString name)
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

void UnivariateData::Apply(double left_bound, double right_bound, uword bound_window)
{
    bound_window_ = bound_window;
    first_left_bound_ = left_bound;
    first_right_bound_ = right_bound;

    first_results_ = Vespucci::Math::Quantification::QuantifyPeakMat(parent_->spectra(),
                                                               parent_->abscissa(),
                                                               left_bound, right_bound,
                                                               bound_window, first_baselines_,
                                                               inflection_first_baselines_);

    band_ratio_ = false;

}

void UnivariateData::Apply(double first_left_bound, double first_right_bound, double second_left_bound, double second_right_bound, uword bound_window)
{
    bound_window_ = bound_window;
    first_left_bound_ = first_left_bound;
    first_right_bound_ = first_right_bound;
    second_right_bound_ = second_right_bound;
    second_left_bound_ = second_left_bound;

    first_results_ = Vespucci::Math::Quantification::QuantifyPeakMat(parent_->spectra(),
                                                               parent_->abscissa(),
                                                               first_left_bound_, first_right_bound_,
                                                               bound_window_, first_baselines_,
                                                               inflection_first_baselines_);
    second_results_ = Vespucci::Math::Quantification::QuantifyPeakMat(parent_->spectra(),
                                                                      parent_->abscissa(),
                                                                      second_left_bound_, second_right_bound_,
                                                                      bound_window_, second_baselines_,
                                                                      inflection_second_baselines_);
    band_ratio_ = true;
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

mat UnivariateData::Intensities(bool adjust_baselines)
{
    if (band_ratio_){
        if (adjust_baselines){
            return join_horiz(first_results_.col(2), second_results_.col(2));
        }else{
            return join_horiz(first_results_.col(1), second_results_.col(1));
        }
    }
    else{
        if (adjust_baselines){
            return first_results_.col(2);
        }
        else{
            return first_results_.col(1);
        }
    }
}

vec UnivariateData::IntensityRatios(bool adjust_baselines)
{
    if (!band_ratio_){return ones(parent_->spectra_ptr()->n_rows);}
    if (adjust_baselines){
        return first_results_.col(2) / second_results_.col(2);
    }else{
        return first_results_.col(1) / second_results_.col(1);
    }
}

mat UnivariateData::Areas(bool adjust_baselines, bool detect_edges)
{
    if (band_ratio_){
        if (adjust_baselines){
            if (detect_edges){
                return join_horiz(first_results_.col(6), second_results_.col(6));
            }else{
                return join_horiz(first_results_.col(4), second_results_.col(4));
            }
        }else{
            if (detect_edges){
                return join_horiz(first_results_.col(5), second_results_.col(5));
            }
            else{
                return join_horiz(first_results_.col(3), second_results_.col(3));
            }
        }
    }else{
        if (adjust_baselines){
            if (detect_edges){
                return first_results_.col(6);
            }else{
                return first_results_.col(4);
            }
        }else{
            if (detect_edges){
                return first_results_.col(5);
            }
            else{
                return first_results_.col(3);
            }
        }
    }
}

vec UnivariateData::AreaRatios(bool adjust_baselines, bool detect_edges)
{
    if (!band_ratio_){return ones(parent_->spectra_ptr()->n_rows);}


    if (adjust_baselines){
        if (detect_edges){
            return first_results_.col(6) / second_results_.col(6);
        }else{
            return first_results_.col(4) / second_results_.col(4);
        }
    }else{
        if (detect_edges){
            return first_results_.col(5) / second_results_.col(5);
        }else{
            return first_results_.col(3) / second_results_.col(3);
        }
    }
}

mat UnivariateData::Bandwidths()
{
    if (band_ratio_){return join_horiz(first_results_.col(7), second_results_.col(7));}
    else{return first_baselines_.col(7);}
}

mat UnivariateData::PeakCenters()
{
    if (band_ratio_){return join_horiz(first_results_.col(0), second_results_.col(0));}
    else{return first_results_.col(0);}
}




