/*******************************************************************************
    Copyright (C) 2014 Wright State University - All Rights Reserved
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
#ifndef PEAKFINDING_H
#define PEAKFINDING_H
#include <Math/Accessory/accessory.h>
#include <Math/PeakFinding/cwtridge.h>
namespace Vespucci
{
    namespace Math
    {
        //Peak detection (SPF)
        namespace PeakFinding
        {
            bool RidgeTooNoisy(const Vespucci::Math::CWTRidge &ridge, std::string method, const arma::vec &noise, arma::uword window_size, double min_snr);
            bool RidgeTooShort(const Vespucci::Math::CWTRidge &ridge, arma::uword min_length);

            std::vector<Vespucci::Math::CWTRidge> FindRidges(arma::mat &coefs,
                                                             arma::uvec scales,
                                                             arma::uword gap_threshold,
                                                             arma::uword ridge_length,
                                                             arma::uword search_width, double noise_threshold);


            std::vector<Vespucci::Math::CWTRidge> LinkRidges(arma::sp_mat &maxima,
                                                             arma::uvec scales,
                                                             arma::mat &coefs,
                                                             arma::uword min_window_size,
                                                             arma::uword gap_threshold);


            arma::vec FindPeaks(arma::vec X, arma::vec dX,
                          double sel,
                          double threshold,
                          arma::vec &peak_magnitudes);

            arma::uvec FindPeakPositions(arma::vec X, arma::vec dX,
                                   double sel,
                                   double threshold,
                                   arma::uvec &local_minima);

            arma::mat FindPeaksMat(arma::mat X,
                             double sel,
                             double threshold,
                             arma::uword poly_order, arma::uword window_size,
                             arma::mat &peak_magnitudes);


            arma::vec EstimateBaseline(arma::vec X,
                                 arma::umat peaks,
                                 arma::uword window_size);
            arma::umat FindPeakPositions(arma::vec X, arma::vec dX,
                                   double threshold,
                                   std::string threshold_method,
                                   arma::vec &peak_magnitudes);
            arma::vec PeakPopulation(arma::uword vector_size, arma::umat peak_positions);
            arma::vec PeakExtrema(arma::uword vector_size, arma::umat peak_positions);
            arma::mat CWTPeakAnalysis(arma::mat X,
                                std::string wavelet, arma::uvec scales,
                                double threshold, std::string threshold_method,
                                arma::mat &transform);

        }
    }
}
#endif // PEAKFINDING_H
