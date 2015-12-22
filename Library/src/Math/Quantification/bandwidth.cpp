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

#include <Math/Quantification/bandwidth.h>

///
/// \brief Vespucci::Math::Quantification::FindBandwidth
/// \param X
/// \param min_index
/// \param max_index
/// \param midline
/// \param abscissa_step
/// \return
/// Finds the full-width at half maximum of a peak bound by min_index and max_index
double Vespucci::Math::Quantification::FindBandwidth(const arma::vec &X, arma::uword min_index, arma::uword max_index, arma::uword &mid_start, arma::uword &mid_end, arma::vec &midline, arma::vec &baseline, double abscissa_step)
{
   arma::vec region = X.subvec(min_index, max_index);
   arma::uword size = region.n_elem;
   double maximum, half_maximum;
   double start_value, end_value;
   midline.set_size(size);
   max_index = 0;
   mid_start = 0;
   mid_end = 0;
   start_value = X(min_index);
   end_value = X(max_index);
   baseline = arma::linspace(start_value, end_value, size);
   region -= baseline;
   maximum = region.max();
   half_maximum = maximum / 2.0;
   arma::uvec max_indices = find(region == maximum);
   max_index = max_indices(0);

   //search for left inflection point
   for (arma::uword i = max_index; i > 0; --i){
       if (X(i) - half_maximum < 0){
           mid_start = i;
           break;
       }
   }

   //search for right inflection point
   for (arma::uword i = max_index; i < size; ++i){
       if (X(i) - half_maximum < 0){
           mid_end = i;
           break;
       }
   }

   //check to make sure the values on the other side of the inflection point aren't better
   if (mid_start > 0 && mid_end < size - 1){
       if(std::fabs(X(mid_start) - half_maximum) > std::fabs(X(mid_start - 1) - half_maximum)){
           --mid_start;
       }

       if (std::fabs(X(mid_end) - half_maximum) > std::fabs(X(mid_end + 1) - half_maximum)){
           ++mid_end;
       }
   }
   arma::uword region_size = region.subvec(mid_start, mid_end).n_elem;
   midline = half_maximum * arma::ones(region_size);
   return abscissa_step * (double) region_size;
}

///
/// \brief Vespucci::Math::Quantification::FindBandwidthMat
/// \param X
/// \param abscissa
/// \param min
/// \param max
/// \param midlines
/// \param baselines
/// \return
/// Finds the bandwidth of every column of a arma::matrix.
arma::vec Vespucci::Math::Quantification::FindBandwidthMat(const arma::mat &X, arma::vec abscissa, double &min, double &max, arma::field<arma::mat> &midlines, arma::field<arma::mat> &baselines, arma::uvec &boundaries)
{
    arma::uword mid_start, mid_end;
    double delta = std::abs(abscissa(1) - abscissa(0));
    arma::uvec left_bound = find(((min-delta) <= abscissa) && (abscissa <= (min+delta)));
    arma::uvec right_bound = find(((max-delta) <= abscissa) && (abscissa <= (max+delta)));

    arma::uword min_index = left_bound(0);
    arma::uword max_index = right_bound(0);
    boundaries << min_index << arma::endr << max_index;

    min = abscissa(min_index);
    max = abscissa(max_index);

    arma::vec results(X.n_cols);
    midlines.set_size(X.n_cols);
    baselines.set_size(X.n_cols);
    arma::vec midline;
    arma::vec baseline;
    for (arma::uword i = 0; i < X.n_cols; ++i){
        results(i) = FindBandwidth(X.col(i), min_index, max_index, mid_start, mid_end, midline, baseline, delta);
        midlines(i) = arma::join_horiz(abscissa.rows(mid_start, mid_end), midline);
        baselines(i) = arma::join_horiz(abscissa.rows(min_index, max_index), baseline);
    }

    return results;
}
