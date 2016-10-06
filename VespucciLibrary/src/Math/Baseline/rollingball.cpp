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
#include "Math/Baseline/rollingball.h"
///
/// \brief Vespucci::Math::Baseline::RollingBallBaseline
/// \param spectrum
/// \param wm
/// \param ws
/// \return
/// based on implementation of rolling balll baseline correction in the
/// R package baseline.
arma::vec Vespucci::Math::Baseline::RollingBallBaseline(const arma::vec &spectrum,
                                                        arma::vec &baseline,
                                                        arma::uword wm,
                                                        arma::uword ws)
{
    arma::uword u1; // placeholder 1
    arma::uword u2; // placeholder 2
    double v;  // sum holder
    baseline.clear();
    baseline.set_size(spectrum.n_rows);
    arma::vec t1(spectrum.n_rows); // minimizers
    arma::vec t2(spectrum.n_rows); // maximizers

    // minimize
    u1 = arma::uword(std::ceil((wm+1)/2.0));
    t1(0) = spectrum.rows(0, u1).min();
    // start of spectrum
    for (arma::uword i = 1; i < wm; ++i){
        u2 = u1 + 1 + (i % 2);
        double min;
        if (u1 + 1 < u2)
            min = spectrum.rows(u1+1, u2).min();
        else
            min = spectrum(u1);
        t1(i) = std::min(min, t1(i - 1));
        u1 = u2;
    }

    // main part of spectrum
    for (arma::uword i = wm; i < (spectrum.n_rows - wm); ++i){
        if ((spectrum(u1 + 1) <= t1(i-1)) && (spectrum(u1 - wm) != t1(i-1)))
            t1(i) = spectrum(u1 + 1);
        else
            t1(i) = spectrum.rows(i - wm, i + wm).min();
        u1++;
    }

    // end of spectrum
    for (arma::uword i = spectrum.n_rows - wm; i < spectrum.n_rows; ++i){
        u2 = u1 + ((i + 1) % 2);
        double min;
        if (u2 - 1 > u1) min = spectrum.rows(u1, u2-1).min();
        else min = spectrum(u1);
        if (min > t1(i - 1))
            t1(i) = t1(i-1);
        else{
            if (u2 < spectrum.n_rows - 1)
                t1(i) = spectrum.rows(u2, spectrum.n_rows - 1).min();
            else
                t1(i) = spectrum(spectrum.n_rows - 1);

        }
        u1 = u2;
    }

    // maximize
    u1 = arma::uword(std::ceil((wm + 1) / 2.0));
    t2(0) = t1.rows(0, u1).max();
    // start of spectrum
    for (arma::uword i = 1; i < wm; ++i){
        u2 = u1 + (i % 2);
        t2(i) = std::max(t1.rows(u1 + 1, u2).max(), t2(i - 1));
        u1 = u2;
    }
    // main part of spectrum
    for (arma::uword i = wm; i < (spectrum.n_rows - wm); ++i){
        if ((t1(u1 + 1) >= t2(i - 1)) && (t1(u1 - wm) != t2(i - 1)))
            t2(i) = t1(u1 + 1);
        else
            t2(i) = t1.rows(i - wm, i + wm).max();
        u1++;
    }
    // end of spectrum
    for (arma::uword i = spectrum.n_rows - wm; i < spectrum.n_rows; ++i){
        u2 = u1 + ((i + 1) % 2);
        double max;
        if (u1 < u2 - 1)
            max = t1.rows(u1, u2-1).min();
        else
            max = t1(u1);
        if (max < t2(i - 1))
            t2(i) = t2(i - 1);
        else{
            if (u2 < spectrum.n_rows - 1)
                t2(i) = t1.rows(u2, spectrum.n_rows - 1).max();
            else
                t2(i) = t1(u2);
        }
        u1 = u2;
    }

    // smooth
    u1 = arma::uword(std::ceil(ws / 2) - 1);
    v = arma::accu(t2.rows(0, u1));
    // start of spectrum
    for (arma::uword i = 0; i < ws; ++i){
        u2 = u1 + (i % 2);
        v += arma::accu(t2(u1 + 1, u2));
        baseline(i) = v / double(u2);
        u1 = u2;
    }
    // main part of spectrum
    v = arma::accu(t2.rows(0, 2*ws));
    baseline(ws) = v / double(2 * ws + 1);
    for (arma::uword i = ws + 1; i < spectrum.n_rows - ws; ++i){
        v = v - t2(i - ws - 1) + t2(i + ws);
        baseline(i) = v / double(2 + ws + 1);
    }
    u1 = spectrum.n_rows - 2*ws;
    v = v - t2(u1);
    baseline(spectrum.n_rows - ws) = v/double(2*ws);

    // end of spectrum
    for (arma::uword i = spectrum.n_rows - ws + 1; i < spectrum.n_rows; ++i){
        u2 = u1 + ((i + 1) % 2);
        v = v = arma::accu(t2(u1, u2 - 1));
        baseline(i) = v / double(spectrum.n_rows - u2 + 1);
        u1 = u2;
    }

    return spectrum - baseline;
}

arma::mat Vespucci::Math::Baseline::RollingBallBaselineMat(const arma::mat &spectra,
                                                           arma::mat &baselines,
                                                           arma::uword wm,
                                                           arma::uword ws)
{
    arma::mat corrected(spectra.n_rows, spectra.n_cols);
    baselines.clear();
    baselines.set_size(spectra.n_rows, spectra.n_cols);
    #ifdef _WIN32
      #pragma omp parallel for default(none) \
          shared(spectra, baselines, wm, ws)
         for (intmax_t i = 0; i < (intmax_t) spectra.n_cols; ++i)
    #else
      #pragma omp parallel for default(none) \
          shared(spectra, baselines, wm, ws)
        for (size_t i = 0; i < spectra.n_cols; ++i)
    #endif
    {
        arma::vec baseline;
        corrected.col(i) = Vespucci::Math::Baseline::RollingBallBaseline(spectra.col(i), baseline, wm, ws);
        baselines.col(i) = baseline;
    }
    return corrected;
}
