#ifndef ENUMS_H
#define ENUMS_H

namespace UnivariateMethod{
///
/// \brief The Method enum
/// Univariate methods
enum Method{Intensity, Area, FWHM, AreaRatio, IntensityRatio, Correlation};
}

namespace MetaMethod{
///
/// \brief MetaMethod enum
/// Different methods for extracting data for a MetaDataset
enum Method{AverageSpectra, ConcatenateDatasets};
}

#endif // ENUMS_H
