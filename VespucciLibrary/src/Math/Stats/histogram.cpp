/*******************************************************************************
    Copyright (C) 2014-2017 Wright State University - All Rights Reserved
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
#include "Math/Stats/histogram.h"
arma::uvec Vespucci::Math::Stats::GenerateHistogram(const arma::mat &data,
                                                    arma::vec &edges,
                                                    uint bins)
{
    if (!data.n_elem) return arma::uvec();
    uint bin_count = bins;

    if (!bins){
        bin_count = Vespucci::Math::Stats::EstimateBinCount(data);
    }

    bin_count = (bin_count > 2 ? bin_count : 3); //histogram will always have at least three bins
    edges = arma::linspace<arma::vec>(data.min() - 1.0, data.max() + 1.0, bin_count + 1);
    arma::umat hist_data = arma::histc(data, edges);
    hist_data = arma::sum(hist_data, 1);

    return arma::conv_to<arma::uvec>::from(hist_data);
}

uint Vespucci::Math::Stats::EstimateBinCount(const arma::mat &data)
{
    //Sturges' rule for finding bin counts automagically
    return uint(std::round(1.0 + 3.332*std::log10(double(data.n_elem)));
}
