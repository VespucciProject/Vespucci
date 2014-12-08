#ifndef UNIVARIATEDATA_H
#define UNIVARIATEDATA_H
#include "enums.h"
#include "vespuccidataset.h"


class UnivariateData
{
public:
    UnivariateData(QSharedPointer<VespucciDataset> parent);
    UnivariateData(QSharedPointer<VespucciDataset> parent, vec control);
    void Apply(double left_bound,
          double right_bound,
          UnivariateMethod::Method method);

    void Apply(double first_left_bound,
          double first_right_bound,
          double second_left_bound,
          double second_right_bound,
          UnivariateMethod::Method method);

    vec results();
    bool band_ratio();
    double left_bound();
    double right_bound();
    double first_left_bound();
    double first_right_bound();
    double second_left_bound();
    double second_right_bound();
    QString MethodDescription();

    mat Midlines();
    uvec Boundaries();
    uvec MidlineBoundaries();
    mat first_baselines();
    mat second_baselines();

private:
    QString method_description_;

    ///
    /// \brief results_
    /// The results which are turned into an image
    vec results_;

    ///
    /// \brief control_
    /// Control correlation is calculated for.
    vec control_;

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

    mat first_baselines_;
    mat second_baselines_;

    ///
    /// \brief positions_
    /// The position in abscissa units of the maximum of the peaks
    mat positions_;

    QSharedPointer<VespucciDataset> parent_;

    uvec boundaries_;
};

#endif // UNIVARIATEDATA_H
