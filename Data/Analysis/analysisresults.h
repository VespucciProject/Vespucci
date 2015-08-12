/*******************************************************************************
    Copyright (C) 2015 Wright State University - All Rights Reserved
    Daniel P. Foose - Author

    This file is part of Vespucci.

    Vespucci is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Vespucci is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Vespucci.  If not, see <http://www.gnu.org/licenses/>.
*******************************************************************************/
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
