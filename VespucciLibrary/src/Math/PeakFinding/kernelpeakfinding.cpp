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
#include "Math/PeakFinding/kernelpeakfinding.h"
using namespace std;
using namespace arma;
uvec Vespucci::Math::PeakFinding::FindIntercepts(const vec &signal)
{
    vec intercepts = zeros(signal.n_rows);
    for (uword i = 1; i < signal.n_rows; ++i){
        if ((signal(i-1) > 0) && (signal(i) < 0))
            intercepts(i) = 1;
    }
    return find(intercepts);
}

vec Vespucci::Math::PeakFinding::DerivGaussKernel(double abscissa_step, uword window_size, uword width, double height)
{
    double range = abscissa_step * double(window_size) / 2.0;
    vec x = linspace(-range, range, window_size);
    return -height*(x / (std::pow(width, 3) * std::sqrt(2 * datum::pi)))
            % arma::exp(-(arma::pow(x,2)/2 * std::pow(width, 2)));
}

uvec Vespucci::Math::PeakFinding::FindPeakCenters(const vec &signal, const vec &abscissa, list<umat> &ridges, uword first_scale, uword last_scale, uword search_window, uword min_length, uword max_gap, const string &max_method)
{
    using namespace Vespucci::Math::PeakFinding;
    if (!(last_scale - first_scale))
        throw invalid_argument("Invalid last_scale must be larger than first_scale!");
    if ((last_scale - first_scale) < min_length)
        throw invalid_argument("Invalid scale or minimum length");
    if (max_method != "signal" && max_method != "mexh")
        throw invalid_argument("Invalid max_method");
    field<uvec> centers(last_scale - first_scale);
    ridges.clear();
    double abscissa_step = (abscissa.max() - abscissa.min()) / double(abscissa.n_elem);
    uword window_size = 3*last_scale;
    //Finding step
    for (uword i = first_scale; i < last_scale; ++i){
        vec kernel = DerivGaussKernel(abscissa_step, window_size, i, signal.max());
        vec derivative = conv(signal, kernel, "same");
        //fix end of vector
        uword end = derivative.n_rows - window_size/2;
        derivative.rows(end, derivative.n_rows - 1).fill(derivative(end));
        centers(i-first_scale) = FindIntercepts(derivative);
    }

    //Linking step
    uword i = 0; //row to start search
    uword j = 0; //row of centers(i)
    uword k = 0; //row of centers searching from centers(i)
    uword l = 0; //row of centers(k)
    for (i = 0; i < centers.n_rows - min_length; ++i){
        for (j = 0; j < centers(i).n_rows; ++j){
            uword initial_center = centers(i)(j);
            umat current_ridge = {{i, initial_center}};
            uword min = initial_center - (search_window / 2);
            uword max = initial_center + (search_window / 2);
            uword current_gap = 0;
            for (k = i+1; (k < centers.n_elem && current_gap < max_gap); ++k){
                uvec current_centers = centers(k);
                for (l = 0; (l < current_centers.n_rows && current_centers(l) <= max); ++l){
                    Row<uword> current_pair;
                    if ((current_centers.n_rows > l) && (current_centers(l) >= min)){
                        if (!current_pair.n_elem){
                            current_pair = {k, current_centers(l)};
                        }
                        else{ //select the closest in case two are found that satisfy criteria
                            Row<uword> candidate_pair = {k, current_centers(l)};
                            uword dist1 = std::abs(int(current_centers(l)) - int(initial_center));
                            uword dist2 = std::abs(int(current_pair(1)) - int(initial_center));
                            current_pair = (dist1 < dist2 ? candidate_pair : current_pair);
                        }
                        //remove a point if it has been resolved

                        uvec query = find(current_centers == current_pair(1));
                        if (query.n_rows){
                            current_centers.shed_row(query(0));
                            centers(k) = current_centers;
                        }
                        current_ridge.insert_rows(current_ridge.n_rows, current_pair);
                    }//if statement for valid center found
                    else{
                        current_gap++;
                    }

                }//for values in centers(j)

            }//for vectors in centers (compared to centers(i))
            if (current_ridge.n_rows >= min_length) ridges.push_back(current_ridge);
        }//for values in centers(i);
    }//for vectors in centers
    //collect peak centers
    uvec peak_centers;
    for (umat &ridge : ridges){
        vec values;
        uvec max_indices;
        if (max_method == "signal"){
            for (uword i = 0; i < ridge.n_rows; ++i){
                if (!values.n_rows) values = {signal(ridge(i, 1))};
                else values = join_vert(values, vec({signal(ridge(i, 1))}) );
            }

        }
        else if (max_method == "mexh"){
            mat smoothed_signal(signal.n_rows, (last_scale - first_scale) + 1);
            for (uword i = first_scale; i <= last_scale; ++i){
                vec kernel = MexicanHatKernel(abscissa_step, window_size, i, signal.max());
                smoothed_signal.col(i-1) = conv(signal, kernel, "same");
            }
            for (uword i = 0; i < ridge.n_rows; ++i){
                if (!values.n_rows) values = vec({smoothed_signal(ridge(i, 1), ridge(i, 0))});
                else values = join_vert(values, vec({smoothed_signal(ridge(i, 1), ridge(i, 0))}) );
            }
        }
        else{
            cerr << "should have thrown invalid_argument earlier!\n";

            return uvec();
        }

        max_indices = find(values == values.max());
        if (!peak_centers.n_rows) peak_centers = {ridge(max_indices(0), 1)};
        else peak_centers = join_vert(peak_centers, uvec({ridge(max_indices(0), 1)}));
    }
    return peak_centers;
}

vec Vespucci::Math::PeakFinding::MexicanHatKernel(double abscissa_step, uword window_size, uword width, double height)
{
    uword k = window_size / 2;
    vec x = linspace(-k*abscissa_step, k*abscissa_step, window_size);
    return height * (2 / (sqrt(3*width) * pow(datum::pi, 0.25)))
            * (ones(x.n_rows) - (pow(x, 2)/pow(width, 2)))
            % exp(-(pow(x, 2) / 2*pow(width, 2)));
}
