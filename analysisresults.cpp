#include "analysisresults.h"


AnalysisResults::AnalysisResults(mat value)
{
    value_ = value;
}

mat AnalysisResults::value()
{
    return value_;
}

mat *AnalysisResults::value_ptr()
{
    return &value_;
}

