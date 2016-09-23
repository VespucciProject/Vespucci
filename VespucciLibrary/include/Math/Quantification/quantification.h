/*******************************************************************************
    Copyright (C) 2014-2016 Wright State University - All Rights Reserved
    Daniel P. Foose - Maintainer/Lead Developer

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
#ifndef QUANTIFICATION_H
#define QUANTIFICATION_H
#include <Math/Accessory/accessory.h>
#include "Global/libvespucci.h"
#include "Math/Quantification/bandwidth.h"
#include "Math/Quantification/correlation.h"
#include "Math/Quantification/integration.h"
#include "Math/Quantification/maximum.h"

namespace Vespucci{
    namespace Math{
        namespace Quantification{
            VESPUCCI_EXPORT arma::rowvec QuantifyPeak(const arma::vec &spectrum,
                                                      const arma::vec &abscissa,
                                                      double &min,
                                                      double &max,
                                                      arma::uword bound_window,
                                                      arma::mat &total_baseline,
                                                      arma::mat &inflection_baseline);

            VESPUCCI_EXPORT arma::mat QuantifyPeakMat(const arma::mat &spectra,
                                                      const arma::vec &abscissa,
                                                      double &min,
                                                      double &max,
                                                      arma::uword bound_window,
                                                      arma::mat &total_baselines,
                                                      arma::field<arma::mat> &inflection_baselines);

            VESPUCCI_EXPORT arma::mat ConvertInflectionBaselines(const arma::field<arma::mat> &inflection_baselines);


            VESPUCCI_EXPORT arma::rowvec FitGaussianPeak(const arma::vec &spectrum, const arma::vec &abscissa,
                                                         double &min,
                                                         double &max,
                                                         arma::uword iterations,
                                                         arma::mat &baseline,
                                                         arma::mat &fit,
                                                         arma::rowvec &gauss_params,
                                                         arma::rowvec &quad_params,
                                                         arma::mat residuals);

            VESPUCCI_EXPORT arma::mat FitGaussianPeakMat(const arma::mat &spectra,
                                                         const arma::vec &abscissa,
                                                         double &min,
                                                         double &max,
                                                         arma::uword iterations,
                                                         arma::mat &baselines,
                                                         arma::mat &fits,
                                                         arma::mat &gauss_params,
                                                         arma::mat &quad_params,
                                                         arma::mat &residuals);


            VESPUCCI_EXPORT arma::rowvec FitLorentzianPeak(const arma::vec &spectrum,
                                                           const arma::vec &abscissa,
                                                           double &min,
                                                           double &max,
                                                           arma::uword bound_window,
                                                           arma::mat &total_baseline,
                                                           arma::mat &inflection_baseline,
                                                           arma::rowvec &lorentz_params,
                                                           arma::rowvec &quad_params);

            VESPUCCI_EXPORT arma::mat FitLorentzianPeakMat(const arma::mat &spectra,
                                                           const arma::vec &abscissa,
                                                           double &min,
                                                           double &max,
                                                           arma::uword bound_window,
                                                           arma::mat &total_baselines,
                                                           arma::field<arma::mat> &inflection_baselines,
                                                           arma::mat &lorentz_params,
                                                           arma::mat &quad_params);

            VESPUCCI_EXPORT arma::rowvec FitPVoigtPeak(const arma::vec &spectrum,
                                                       const arma::vec &abscissa,
                                                       double &min,
                                                       double &max,
                                                       arma::uword bound_window,
                                                       arma::mat &total_baseline,
                                                       arma::mat &inflection_baseline,
                                                       arma::rowvec &gauss_params,
                                                       arma::rowvec &lorentz_params,
                                                       arma::rowvec &gauss_quad_params,
                                                       arma::rowvec &lorentz_quad_params,
                                                       arma::rowvec &pvoigt_params);

            VESPUCCI_EXPORT arma::mat FitPVoigtPeakMat(const arma::mat &spectra,
                                                       const arma::vec &abscissa,
                                                       double &min,
                                                       double &max,
                                                       arma::uword bound_window,
                                                       arma::mat &total_baselines,
                                                       arma::field<arma::mat> &inflection_baselines,
                                                       arma::mat &gauss_params,
                                                       arma::mat &lorentz_params,
                                                       arma::mat &guass_quad_params,
                                                       arma::mat &lorentz_quad_params,
                                                       arma::mat &pvoigt_params);
            arma::mat GuoX(const arma::vec &x, const arma::vec &y);
            arma::mat GuoY(const arma::vec &x, const arma::vec &y);
        }
    }
}


#endif //QUANTIFICATION_H
