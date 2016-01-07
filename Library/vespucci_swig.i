// Copyright (C) 2015 Wright State University
// Daniel P. Foose, author
//
// This file is part of Vespucci.
// It is provided without any warranty of fitness
// for any purpose. You can redistribute this file
// and/or modify it under the terms of the GNU
// General Public License (GPL) as published
// by the Free Software Foundation, either version 3
// of the License or (at your option) any later version.
// (see http://www.opensource.org/licenses for more info)
%module vespucci
%{
  #define VESPUCCI_EXPORT
  #include "include/Global/enums.h"
  #include "include/Data/Import/binaryimport.h"
  #include "include/Data/Import/textimport.h"
  #include "include/Math/DimensionReduction/dimensionreduction.h"
  #include "include/Math/Normalization/normalization.h"
  #include "include/Math/Smoothing/smoothing.h"
  #include "include/Math/Transform/cwt.h"
  #include "include/Math/PeakFinding/peakfinding.h"
  #include "include/Math/PeakFinding/cwtridge.h"
  #include "include/Math/Fitting/linleastsq.h"
  #include "include/Math/Fitting/nonlinleastsq.h"
  #include "include/Global/vespucci.h"
  #include "include/Math/Transform/fft.h"
  #include "include/Math/Smoothing/FIR.h"
  #include "include/Math/Smoothing/whittaker.h"
  #include "include/Math/Smoothing/nonlinear.h"
  #include "include/Math/Quantification/integration.h"
  #include "include/Math/Quantification/maximum.h"
  #include "include/Math/Quantification/bandwidth.h"
  #include "include/Math/Quantification/correlation.h"

  using namespace Vespucci;
  using namespace Math;
  using namespace Fitting;
  using namespace LinLeastSq;
  using namespace BinaryImport;
  using namespace TextImport;
  using namespace DimensionReduction;
  using namespace Normalization;
  using namespace PeakFinding;
  using namespace Quantification;
  using namespace Smoothing;
  using namespace Transform;
%}

/* We need this for boost_shared::ptr support */
%include <boost_shared_ptr.i>

/* Now include ArmaNpy typemaps */
%include "armanpy.i"

/* Some minimal excpetion handling */
%exception {
    try {
        $action
    } catch( char * str ) {
        PyErr_SetString( PyExc_IndexError, str );
        SWIG_fail;
    }
}

/* Parse the header files to generate wrappers */
%include "include/Global/enums.h"
%include "include/Data/Import/binaryimport.h"
%include "include/Data/Import/textimport.h"
%include "include/Math/DimensionReduction/dimensionreduction.h"
%include "include/Math/Normalization/normalization.h"
%include "include/Math/Smoothing/smoothing.h"
%include "include/Math/Transform/cwt.h"
%include "include/Math/PeakFinding/peakfinding.h"
%include "include/Math/PeakFinding/cwtridge.h"
%include "include/Math/Fitting/linleastsq.h"
%include "include/Math/Fitting/nonlinleastsq.h"
%include "include/Global/vespucci.h"
%include "include/Math/Transform/fft.h"
%include "include/Math/Smoothing/FIR.h"
%include "include/Math/Smoothing/whittaker.h"
%include "include/Math/Smoothing/nonlinear.h"
%include "include/Math/Quantification/integration.h"
%include "include/Math/Quantification/maximum.h"
%include "include/Math/Quantification/bandwidth.h"
%include "include/Math/Quantification/correlation.h"
