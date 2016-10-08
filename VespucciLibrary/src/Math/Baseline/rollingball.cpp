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
#include "Math/Accessory/accessory.h"
///
/// \brief Vespucci::Math::Baseline::RollingBallBaseline
/// \param spectrum
/// \param wm
/// \param ws
/// \return
/// based on implementation of rolling balll baseline correction in the
/// R package baseline.
/// Indices are a bit weird. This is due to tranlation from the R baseline package.
/// R indexing starts at 1. The decision has been made to subtract one every time
/// an index is used rather than modifying other terms.
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
    arma::vec T1 = spectrum; // minimizers
    arma::vec T2 = spectrum; // maximizers

    // minimize
    u1 = std::ceil((wm + 1) / 2) + 1;
    T1(0) = Vespucci::Math::SafeRows(spectrum, 0, u1-1).min();
    for (arma::uword i = 2; i <= wm; ++i){ // start of spectrum
        u2 = u1 + 1 + (i % 2);
        T1(i - 1) = std::min(Vespucci::Math::SafeRows(spectrum, u1, u2 - 1).min(), T1(i - 2));
        u1 = u2;
    }
    for (arma::uword i = wm + 1; i <= spectrum.n_rows - wm; ++i){ // main part
        if ((spectrum(u1) <= T1(i - 2)) && spectrum(u1 - wm - 1) != T1(i - 2))
            T1(i) = spectrum(u1); // next smaller
        else
            T1(i) = Vespucci::Math::SafeRows(spectrum, i - wm - 1, i + wm - 1).min();
        u1++;
    }
    u1 = spectrum.n_rows - 2*wm - 1;
    for (arma::uword i = spectrum.n_rows - wm + 1; i <= spectrum.n_rows; ++i){ // end
        u2 = u1 + 1 + ((i + 1) % 2);
        if (Vespucci::Math::SafeRows(spectrum, u1 - 1, u2 - 2).min() > T1(i-2))
            T1(i-1) = T1(i-2); // removed larger
        else
            T1(i-1) = Vespucci::Math::SafeRows(spectrum, u2, spectrum.n_rows - 1).min();
        u1 = u2;
    }

    // maximize
    u1 = std::ceil((wm + 1) / 2.0) + 1;
    T2(0) = Vespucci::Math::SafeRows(T1, 0, u1).max();
    for (arma::uword i = 2; i <= wm; ++i){ // start
        u2 = u1 + 1 + (i % 2);
        T2(i - 1) = std::max(Vespucci::Math::SafeRows(T1, u1, u2 - 1).max(), T2(i - 1)); // check if new is larger
        u1 = u2;
    }
    for (arma::uword i = wm + 1; i <= spectrum.n_rows - wm; ++i){ // main part
        if ((T1(u1) >= T2(i - 2)) && (T1(u1 - wm - 1) != T2(i - 2)))
            T2(i - 1) = T1(u1); // next is larger
        else
            T2(i - 1) = Vespucci::Math::SafeRows(T1, i - wm - 1, i + wm - 1).max();
        u1++;
    }
    for (arma::uword i = spectrum.n_rows - wm + 1; i <= spectrum.n_rows; ++i){ // end
        u2 = u1 + 1 + ((i + 1) % 2);
        if (Vespucci::Math::SafeRows(T1, u1 - 1, u2 - 2).max() < T2(i - 2))
            T2(i - 1) = T2(i - 2); //removed is smaller
        else
            T2(i - 1) = Vespucci::Math::SafeRows(T1, u2 - 1, spectrum.n_rows - 1).max();
        u1 = u2;
    }

    // smooth
    u1 = std::ceil(ws / 2.0);
    v = arma::accu(Vespucci::Math::SafeRows(T2, 0, u1 - 1));
    for (arma::uword i = 1; i <= ws; ++i){ // start
        u2 = u1 + 1 + (i % 2);
        v += arma::accu(Vespucci::Math::SafeRows(T2, u1, u2 - 1));
        baseline(i - 1) = v / double(u2);
        u1 = u2;
    }
    v = arma::accu(Vespucci::Math::SafeRows(T2, 0, 2*ws));
    baseline(ws) = v / (2*ws+1);
    for (arma::uword i = ws + 2; i <= spectrum.n_rows - ws; ++i){
        v = v - T2(i - ws - 2) + T2(i + ws - 1);
        baseline(i - 1) = v / double(2*ws+1);
    }
    u1 = spectrum.n_rows - 2 * ws + 1;
    v -= T2(u1 - 1);
    baseline(spectrum.n_rows - ws) = v / double(2*ws);
    for (arma::uword i = spectrum.n_rows - ws + 2; i <= spectrum.n_rows; ++i){
        u2 = u1 + 1 + (i+1) % 2;
        v -= arma::accu(Vespucci::Math::SafeRows(T2, u1 - 1, u2 - 2));
        baseline(i - 1) = v / double(spectrum.n_rows - u2 + 1);
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
          shared(spectra, baselines, wm, ws, corrected)
         for (intmax_t i = 0; i < (intmax_t) spectra.n_cols; ++i)
    #else
      #pragma omp parallel for default(none) \
          shared(spectra, baselines, wm, ws, corrected)
        for (size_t i = 0; i < spectra.n_cols; ++i)
    #endif
    {
        arma::vec baseline;
        corrected.col(i) = Vespucci::Math::Baseline::RollingBallBaseline(spectra.col(i), baseline, wm, ws);
        baselines.col(i) = baseline;
    }
    return corrected;
}
