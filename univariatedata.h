#ifndef UNIVARIATEDATA_H
#define UNIVARIATEDATA_H
#include "vespuccidataset.h"
namespace Univariate{
    enum Method{
        Intensity,
        Area,
        FWHM,
        AreaRatio,
        IntensityRatio
    };
}
class VespucciDataset;
class UnivariateData
{
public:
    UnivariateData(QSharedPointer<VespucciDataset> parent);
    void Apply(double left_bound,
          double right_bound,
          Univariate::Method method);

    void Apply(double first_left_bound,
          double first_right_bound,
          double second_left_bound,
          double second_right_bound,
          Univariate::Method method);

    vec results();
    bool band_ratio();
    double left_bound();
    double right_bound();
    double first_left_bound();
    double first_right_bound();
    double second_left_bound();
    double second_right_bound();
    QString MethodDescription();

    mat Baselines(uword index);
    mat Midlines();
    uvec Boundaries();
    uvec MidlineBoundaries();

private:
    ///
    /// \brief results_
    /// The results which are turned into an image
    vec results_;

    ///
    /// \brief method_
    /// The method
    int method_;
    ///
    /// \brief band_ratio_
    /// Whether or not this is a band ratio map
    bool band_ratio_;

    ///
    /// \brief left_bound_
    /// The spectral left bound
    double left_bound_;

    ///
    /// \brief right_bound_
    /// The right bound in spectral units
    double right_bound_;

    double first_left_bound_;
    double first_right_bound_;
    double second_left_bound_;
    double second_right_bound_;


    ///
    /// \brief midlines_
    /// The midlines (band ratio)
    mat midlines_;

    ///
    /// \brief ratio_baselines_
    /// Contains the two sets of baselines for an intensity band ratio.
    field<mat> baselines_;

    ///
    /// \brief positions_
    /// The position in abcissa units of the maximum of the peaks
    mat positions_;

    QSharedPointer<VespucciDataset> parent_;
    QString method_description_;

    uvec boundaries_;
};

#endif // UNIVARIATEDATA_H
