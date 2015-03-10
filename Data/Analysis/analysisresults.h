#ifndef ANALYSISRESULTS_H
#define ANALYSISRESULTS_H
#include "Data/Dataset/vespuccidataset.h"
///
/// \brief The AnalysisResults class
/// A container for a mat object that allows a mat to be copied to a heap-allocated
/// object (this) so pointers to that mat cannot go out of scope. These objects
/// should be heap-allocated inside smart pointers.
class AnalysisResults
{
public:
    AnalysisResults(mat value);
    mat value();
    mat *value_ptr();
private:
    ///
    /// \brief value_
    /// The data
    mat value_;
};

#endif // ANALYSISRESULTS_H
