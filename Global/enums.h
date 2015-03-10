#ifndef ENUMS_H
#define ENUMS_H

namespace UnivariateMethod{
///
/// \brief The Method enum
/// Univariate methods
enum Method{Intensity, Area, FWHM, AreaRatio, IntensityRatio};


///
/// \brief The InputFileFormat namespace
/// Types of files that can be imported
///
namespace InputFileFormat
{
///
/// \brief The Formats enum
///
    enum Format{
        LongTabDel,
        LongCSV,
        WideTabDel,
        WideCSV,
        VespucciBinary,
        Witec,
        Invalid
    };
}

}

#endif // ENUMS_H
