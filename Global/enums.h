#ifndef ENUMS_H
#define ENUMS_H

namespace UnivariateMethod{
///
/// \brief The Method enum
/// Univariate methods
    enum Method{Intensity, Area, FWHM, AreaRatio, IntensityRatio, SignalNoise,Correlation};
}

namespace MetaMethod{
    enum Method{AverageSpectra, ConcatenateDatasets};
}

#endif // ENUMS_H
