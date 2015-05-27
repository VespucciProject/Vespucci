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

#include <Math/PeakFinding/peakfinding.h>
#include <Math/Smoothing/smoothing.h>
#include <Math/PeakFinding/cwtridge.h>
#include <Math/Transform/cwt.h>




///
/// \brief Vespucci::Math::PeakFinding::FindPeaks an implementation of the peakfinder arma::matLAB routine
/// \param X A vector representing a spectrum
/// \param dX A first-derivative spectrum of X
/// \param sel The amount above surrounding data for a peak to be identified
/// \param threshold A threshold value which peaks must be larger than to be
/// maxima
/// \param poly_order The polynomial order of the Savitzky-Golay filter used for
/// derivatization
/// \param window_size The window size of the Savitzky-Golay filter used for
/// derivatization
/// \param peak_locations A vector containing one and arma::zeros corresponding to
/// whether or not a peak center exists at that wavelength
/// \param peak_magnitudes A vector containing values at peak centers.
/// \return
///
arma::vec Vespucci::Math::PeakFinding::FindPeaks(arma::vec X,
                        arma::vec dX,
                        double sel,
                        double threshold,
                        arma::vec &peak_magnitudes)
{
    arma::uvec peak_indices;
    arma::uvec peak_locations;
    peak_magnitudes = arma::zeros(X.n_rows, 1);

    dX.elem( find(dX == 0) ).fill(-arma::datum::eps); //to find first of repeated values
    double minimum_magnitude = X.min();
    double left_min = X(0);
    arma::uword length = X.n_elem;
    arma::uword temporary_location = 0;

    //Find where derivative changes sign:
    arma::uvec extrema_indices = find( (dX.subvec(0, length - 2) % dX.subvec(1, length - 1) )< 0);
    arma::vec X_extrema = X.elem(extrema_indices);
    length = X_extrema.n_elem;
    double temporary_magnitude = minimum_magnitude;
    bool peak_found = false;
    arma::uword ii;
    if (X(1) >= X(2))
        ii = 1;
    else
        ii = 2;

    //This loop finds the peak locations
    while (ii < length){
        //reset peak finding if we had a peak earlier and the next peak is
        //bigger than the last or the left minimum was small enough to reset
        if (peak_found){
            temporary_magnitude = minimum_magnitude;
            peak_found = false;
        }

        if ( (X_extrema(ii) > temporary_magnitude) && (X_extrema(ii) > left_min + sel) ){
            std::cout << "temporary_location = " << ii << std::endl;
            temporary_magnitude = X_extrema(ii);
        }

        ++ii; //move into the valley
        if (ii >= length){
            break;
        }
        //come down at least sel from peak
        if (!peak_found && (temporary_magnitude > sel + X_extrema(ii) ) ){
            peak_found = true;
            std::cout << "peak found!" << std::endl;
            left_min = X_extrema(ii);
            peak_locations << temporary_location;
        }
        else if(X(ii) < left_min)
            left_min = X_extrema(ii);

        ++ii;


    }
    //remove all peaks below the threshold

    arma::vec results = arma::zeros(X.n_elem, 1);
    peak_indices = extrema_indices.elem(peak_locations);
    results.elem(peak_indices).ones();
    peak_magnitudes.elem(peak_indices) = X(peak_indices);



    arma::uvec indices = find(peak_magnitudes <= threshold);
    peak_magnitudes.elem(indices).zeros();
    results.elem(indices).zeros();
    return results;
}


///
/// \brief Vespucci::Math::PeakFinding::FindPeaksMat Performs FindPeaks on a spectra arma::matrix
/// \param x
/// \param sel
/// \param threshold
/// \param poly_order
/// \param window_size
/// \param peak_locations
/// \param peak_magnitudes
/// \return
///
arma::mat Vespucci::Math::PeakFinding::FindPeaksMat(arma::mat X, double sel, double threshold, arma::uword poly_order, arma::uword window_size, arma::mat &peak_magnitudes)
{
    arma::mat peak_locations(X.n_rows, X.n_cols); //value that gets returned
    peak_magnitudes.set_size(X.n_rows, X.n_cols);

    //calculate a smoothed first derivative:
    arma::mat derivatized = Vespucci::Math::Smoothing::sgolayfilt(X, poly_order, window_size, 1, 1);
    arma::vec thresh(X.n_cols);
    arma::vec sels(X.n_cols);
    if (std::isnan(sel)){
        for (arma::uword i = 0; i < X.n_cols; ++i){
            sels(i) = (X.col(i).max() - X.col(i).min() ) / 16.0;
        }
    }
    else{
        sels.fill(sel);
    }

    if (std::isnan(threshold)){
        arma::uword k = (X.n_rows % 2 == 0 ? (X.n_rows / 2) : (X.n_rows + 1 / 2 ));
        arma::vec buffer(X.n_rows);
        arma::uvec sorted;
        //we calculate median this way due to a problem with my MinGW compiler that
        //causes crashes when arma::median is called.
        for (arma::uword i = 0; i < X.n_cols; ++i){
            buffer = Vespucci::Math::diff(X, 1); //use arithmetic derivative, rather than smoothed derivative
            sorted = stable_sort_index(buffer);
            thresh(i) = buffer(sorted(k));
        }
    }
    else{
        thresh.fill(threshold);
    }
    arma::vec current_spectrum;
    arma::vec current_derivative;
    arma::vec current_magnitudes;
    arma::vec current_peaks;
    for (arma::uword i = 0; i < X.n_cols; ++i){
        current_spectrum = X.col(i);
        current_derivative = derivatized.col(i);
        current_peaks = Vespucci::Math::PeakFinding::FindPeaks(current_spectrum, current_derivative, sels(i), thresh(i), current_magnitudes);
        peak_locations.col(i) = current_peaks;
        peak_magnitudes.col(i) = current_magnitudes;
    }
    return peak_locations;
}

///
/// \brief Vespucci::Math::PeakFinding::FindPeakPositions
/// \param X
/// \param dX A buffered first derivative (same size of X with value of X(i) equal
/// to the derivative of X at i. Taken as a parameter incase a smoothed derivative is to be used
/// \param threshold A value for threshold of significance. Depending on type
/// \param threshold_method Describes the method to be used (see below)
/// \return A arma::umat in which each row represents a peak. The first column contains
/// indices of peak centers, the second column contains the left bound of the peak
/// and the third column contains the right bound of the peak. A arma::matrix of this
/// forarma::mat is the expected input for Vespucci::Math::PeakFinding::EstimateBaseline. The peak determination
/// may be carried out on a transformed spectra (such as heavy S-G smoothing, kernel
/// convolution or CWT) then EstimateBaseline called with these peak centers and
/// the original spectrum. EstimateBaseline will then exclude the larger peaks
/// from the baseline. Since EstimateBaseline uses a local minimum filter, smaller
/// peaks will be retained while preventing wider peaks from being cut into by the
/// filter.
///
/// threshold_method can be the following:
/// "magnitude" - a minimum peak-height threshold, arbitrary double
/// "count" - maximum number of peaks found, largest magnitude first (1 to many)
/// "countpercentage" - largest percentage of all peaks found (0 to 1)
/// "ratio" - ratio of largest magnitude (0 to 1)
arma::umat Vespucci::Math::PeakFinding::FindPeakPositions(arma::vec X, arma::vec dX,
                                 double threshold,
                                 std::string threshold_method,
                                 arma::vec &peak_magnitudes)
{
    //threshold can be arbitrary or calculated

    arma::vec d2X = Vespucci::Math::diff(dX, 1);
    d2X.insert_rows(0, 1, true);
    if (threshold_method == "count" && threshold <= 0){
        throw std::runtime_error("Invalid threshold value for given method");
    }
    if ((threshold_method == "countpercentage" || threshold_method == "ratio") && threshold > 1){
        throw std::runtime_error("Invalid threshold value for given method");
    }

    //find where first derivative changes sign
    arma::uvec extrema_indices = find( (dX.subvec(0, X.n_rows - 2) % dX.subvec(1, X.n_rows - 1) )< 0);
    arma::vec d2X_extrema = d2X(extrema_indices);
    arma::uvec maxima_indices = find(d2X_extrema < 0); //the indices in d2X_extrema, X_extrema and extrema_indices that correspond to maxima
    arma::umat results(maxima_indices.n_elem, 3);
    arma::uvec buffer(3);
    peak_magnitudes.set_size(maxima_indices.n_elem); //each maximum corresponds to a magnitude

    //we have the indices of the maxima, now we need to find magnitudes and positions
    //of the minima associated with each maximum
    arma::uword center_index = 0;
    arma::uword left_index = 0;
    arma::uword right_index = 0;

    //find peak edges for each center and the magnitude of the peaks
    arma::uword i;
    try{
        for (i = 0; i < maxima_indices.n_elem; ++i){

            if(maxima_indices(i) == 0){
                //no left index (at beginning)
                center_index = extrema_indices(maxima_indices(i)); //index of the center of peak
                right_index = extrema_indices(maxima_indices(i) + 1); //index of right edge
                left_index = center_index; //left edge is same as center
            }

            else if(maxima_indices(i) + 1 >= extrema_indices.n_elem){
                //no right_index (we're at the end)
                center_index = extrema_indices(maxima_indices(i)); //index of the center of peak
                left_index = extrema_indices(maxima_indices(i) - 1); //index of left edge
                right_index = center_index;
            }

            else{
                //normal
                center_index = extrema_indices(maxima_indices(i)); //index of the center of peak
                left_index = extrema_indices(maxima_indices(i) - 1); //index of left edge
                right_index = extrema_indices(maxima_indices(i) + 1); //index of right edge
            }

            //if left_index == right_index, average is the same as difference...
            peak_magnitudes(i) = (2.0*X(center_index) - X(left_index) - X(right_index))/2.0;
            buffer(0) = center_index;
            buffer(1) = left_index;
            buffer(2) = right_index;
            results.row(i) = buffer.t();
        }
    }catch(std::exception e){
        std::cout << "Exception!" << std::endl;
        std::cout << e.what() << std::endl;
        std::cout << "iterator index = " << i << std::endl;
        std::cout << "maxima index = " << maxima_indices(i) << std::endl;
        std::cout << "extrema_indices.n_rows = " << extrema_indices.n_rows << std::endl;
        std::cout << "center_index = " << center_index << std::endl;
        std::cout << "left_index = " << left_index << std::endl;
        std::cout << "right_index = " << right_index << std::endl;
        throw std::runtime_error(e.what());
    }

    //Fix thresholds//
    //sort with largest first:
    arma::uvec sorted_peak_indices = stable_sort_index(peak_magnitudes, "descend");
    peak_magnitudes = peak_magnitudes.rows(sorted_peak_indices);
    results = results.rows(sorted_peak_indices);

    if (threshold_method == "magnitude"){
        arma::uvec indices = find(peak_magnitudes > threshold);
        peak_magnitudes = peak_magnitudes.rows(indices);
        return results.rows(indices);
    }
    else if (threshold_method == "count"){
        arma::uword uthresh = (std::ceil(threshold) < results.n_rows - 1 ? std::ceil(threshold) : results.n_rows - 1);
        peak_magnitudes = peak_magnitudes.rows(0, uthresh);
        return results.rows(0, uthresh);
    }
    else if (threshold_method == "ratio"){
        if (threshold == 1){
            peak_magnitudes = peak_magnitudes.row(0);
            return results.row(0);
        }
        else{
            double cutoff = peak_magnitudes(0) * threshold;
            arma::uvec indices = find(peak_magnitudes > cutoff);
            peak_magnitudes = peak_magnitudes.rows(indices);
            return results.rows(indices);
        }
    }
    else{ //(threshold_method == "countpercentage")
        arma::uword count = std::floor(threshold*results.n_rows);
        peak_magnitudes = peak_magnitudes.rows(0, count);
        return results.rows(0, count);
    }

}


arma::vec Vespucci::Math::PeakFinding::PeakPopulation(arma::uword vector_size, arma::umat peak_positions)
{
    arma::uvec peaks = peak_positions.col(0);
    arma::vec population = arma::zeros(vector_size);
    population.elem(peaks) = arma::ones(peaks.n_elem);
    return population;
}


arma::vec Vespucci::Math::PeakFinding::PeakExtrema(arma::uword vector_size, arma::umat peak_positions)
{
    if (peak_positions.n_cols != 3){
        return arma::zeros(vector_size);
    }
    else{
        arma::vec extrema = arma::zeros(vector_size);
        arma::uvec peak_centers = peak_positions.col(0);
        arma::uvec left_boundaries = peak_positions.col(1);
        arma::uvec right_boundaries = peak_positions.col(2);
        extrema.elem(peak_centers) = arma::ones(peak_centers.n_elem);
        extrema.elem(left_boundaries) = -1 * arma::ones(left_boundaries.n_elem);
        extrema.elem(right_boundaries) = -1 * arma::ones(right_boundaries.n_elem);
        return extrema;
    }
}

///
/// \brief EstimateBaseline
/// \param X Signal
/// \param peaks Positions of peaks to exclude from spectrum to calculate baseline
/// \param window_size Size of local minimum filter applied to baseline
/// \return
///
arma::vec Vespucci::Math::PeakFinding::EstimateBaseline(arma::vec X,
                               arma::umat peaks,
                               arma::uword window_size)
{
    if (window_size % 2 == 0){
        window_size--;
        std::cerr << "invalid window_size, using one less" << std::endl;
    }

    arma::vec baseline = X;
    arma::uword start, end, center, size;
    //linearize across bases of peaks
    arma::uword i;
    try{
        for (i = 0; i < peaks.n_rows; ++i){
            center = peaks(i, 0);
            start = peaks(i, 1);
            end = peaks(i, 2);
            //cut accross in a straight line when peak is on edge of spectrum
            size = end - start;

            //linearize baseline across base of peak
            // if the start or end of the peak is the center, take a flat line
            // from the minimum
            if (start == center){
                baseline.subvec(start, end-1) = X(end) * arma::ones(size);
            }
            else if (end == center){
                baseline.subvec(start, end-1) = X(start) * arma::ones(size);
            }
            else{
                baseline.subvec(start, end-1) = arma::linspace(X(start), X(end), size);
            }

        }
    }catch(std::exception e){
        std::cout << "Exception! (peak exclusion)" << std::endl;
        std::cout << "size of peaks = " << peaks.n_rows << " " << peaks.n_cols << std::endl;
        std::cout << "i = " << i << std::endl;
        throw std::runtime_error(e.what());
    }

    //apply local minimum filtering to baseline

    arma::uword k = (window_size - 1) / 2; //mid-point of window
    arma::vec buffer;
    arma::vec filtered(baseline.n_elem);
    try{
        for (arma::uword i = k; i < (X.n_rows - k); ++i){
            buffer = baseline.subvec(i-k, i+k);
            filtered(i) = buffer.min();
        }
        //fill edges with first and last values
        for (arma::uword i = 0; i < k; ++i){
            filtered(i) = filtered(k);
        }
        for (arma::uword i = filtered.n_rows - k; i < filtered.n_elem; ++i){
            filtered(i) = filtered(filtered.n_rows - k - 2);
        }
    }catch(std::exception e){
        std::cout << "Exception! (filtering)" << std::endl;
        std::cout << "i = " << i << std::endl;
        throw std::runtime_error(e.what());
    }

    return filtered;


}



///
/// \brief Vespucci::Math::PeakFinding::FindRidges
/// \param coefs
/// \param gap_threshold
/// \param ridge_length
/// \param search_width
/// \return
/// Identify "ridges" in a CWT transform matrix.
std::vector<Vespucci::Math::CWTRidge>
    Vespucci::Math::PeakFinding::FindRidges(arma::mat &coefs,
                                             arma::uvec scales,
                                             arma::uword gap_threshold,
                                             arma::uword ridge_length,
                                             arma::uword search_width,
                                             double noise_threshold,
                                             std::string noise_method,
                                             arma::uword noise_window)
{
    arma::sp_mat maxima;
    try{
        maxima = Vespucci::Math::LocalMaxima(coefs);
    }catch(std::exception e){
        std::cerr << "LocalMaxima" << std::endl;
        std::cerr << e.what() << std::endl;
        throw e;
    }
    arma::mat maxima_sum = arma::sum( (arma::mat) maxima, 1);
    ( (arma::mat) maxima).save("C:/Users/Dan/Documents/maxima.csv", arma::csv_ascii);
    maxima_sum.save("C:/Users/Dan/Documents/maxima_sum.csv", arma::csv_ascii);
    std::vector<Vespucci::Math::CWTRidge> ridges;
    try{
        ridges = Vespucci::Math::PeakFinding::LinkRidges(maxima,
                                                         scales,
                                                         coefs,
                                                         search_width,
                                                         gap_threshold);
    }catch(std::exception e){
        std::cerr << "LinkRidges error" << std::endl;
        std::cerr << e.what();
        throw e;
    }




    //merge degenerate ridges into first ridge with that center

    //sort ridges by index
    std::sort(ridges.begin(), ridges.end());

    //iterate through the first ridges of a particular center
    //concatenating degenerate ridges

    std::vector<Vespucci::Math::CWTRidge>::iterator last;


    //iterate over sorted list, then merge subsequent into first if same
    for (std::vector<Vespucci::Math::CWTRidge>::iterator i = ridges.begin(); i != ridges.end(); ++i)
        while ((i != ridges.end() - 1) && *(i+1) == *i)
            (*i).Merge(*(++i));


    //remove degenerate ridges
    last = std::unique(ridges.begin(), ridges.end());
    ridges.erase(last, ridges.end());

    //filter out ridges that are too short or too noisy
    //the lowest coefficient is the "noise" vector
    arma::vec noise = coefs.col(0);

    //estimate SNR for every ridge
    std::for_each(ridges.begin(), ridges.end(),
                  [noise, noise_method, noise_window](Vespucci::Math::CWTRidge &r)
                  {r.EstimateSNR(noise_method, noise_window, noise);});

    //remove too noisy and too short
    last = std::remove_if(ridges.begin(), ridges.end(),
                          [noise_threshold, ridge_length]
                          (const Vespucci::Math::CWTRidge &r)
                          { return (r.SNR() < noise_threshold) || (r.length() < ridge_length); });
    ridges.erase(last, ridges.end());
    return ridges;
}


std::vector<Vespucci::Math::CWTRidge> Vespucci::Math::PeakFinding::LinkRidges(const arma::sp_mat &maxima, arma::uvec scales, arma::mat &coefs, arma::uword min_window_size, arma::uword gap_threshold)
{
    //return value
    std::vector<Vespucci::Math::CWTRidge> ridges;

    //find the indices of all local maxima
    arma::field<arma::uvec> maxima_indices(maxima.n_cols);
    for (arma::uword i = 0; i < maxima.n_cols; ++i){
        arma::vec column = (arma::vec) maxima.col(i);
        arma::uvec query = arma::find(column);
        maxima_indices(i) = query;
    }

    arma::uword skip_count = 0;
    //iterate through "seed columns"
    for (arma::uword seed_it = maxima_indices.n_rows - 1; seed_it > 2; --seed_it){
        arma::uword window_size = 1 + (2*scales(seed_it));
        if (window_size < min_window_size)
            window_size = min_window_size;



        //iterates through current seed column to link down columns
        for (arma::uword max_it = 0; max_it < maxima_indices(seed_it).n_rows; ++max_it){

            //each one of these "seeds" gets a new CWTRidge
            ridges.push_back(Vespucci::Math::CWTRidge(max_it));

            arma::uword current_index = maxima_indices(seed_it)(max_it);
            ridges.back().InsertPoint(current_index, seed_it, coefs(current_index, seed_it));
            //maxima_indices(seed_it)(max_it);
            try{
                maxima_indices(seed_it).shed_row(max_it); //remove from search
            }catch(std::exception e){
                std::cerr << "first shed_row" << std::endl;
                throw e;
            }

            skip_count = 0;
            int link_it = seed_it - 1;

            //go across columns to link current index
            while( (skip_count <= gap_threshold) && (link_it >= 0) ){
                arma::uword window_size = 1 + (2*scales(link_it));
                if (window_size < min_window_size)
                    window_size = min_window_size;
                arma::uword start = current_index - window_size;
                arma::uword end = current_index + window_size;
                arma::uvec query;
                try{
                    arma::uvec current_col = maxima_indices(link_it);
                    query = arma::find(current_col >= start && current_col <= end);
                }catch(std::exception e){
                    std::cerr << "query" << std::endl;
                    std::cerr << "link_it = " << link_it << std::endl;
                    std::cerr << "maxima_indices.n_cols= " << maxima_indices.n_cols << std::endl;
                    throw e;
                }

                if (query.n_rows > 1){
                    //determine which is closer
                    arma::uvec distance(query.n_rows);
                    //making sure subtraction works with unsigned...
                    arma::uword b;
                    try{
                        for (b = 0; b < query.n_elem; ++b){
                            distance(b) = std::abs((int) query(b) - (int) current_index);
                        }
                    }catch(std::exception e){
                        std::cerr << "for loop for found one+" << std::endl;
                        std::cerr << "distance.n_rows = " << distance.n_rows << std::endl;
                        std::cerr << "b = " << b << std::endl;
                        throw e;
                    }

                    arma::uvec dis_query = arma::find(distance == distance.min());
                    arma::uword ind = query(dis_query(0));
                    ridges.back().InsertPoint(ind, link_it, coefs(ind, link_it));
                    try{
                        maxima_indices(link_it).shed_row(ind);
                    }catch(std::exception e){
                        maxima_indices(link_it).print();
                        std::cerr << "maxima_indices.n_rows = " << maxima_indices.n_rows << std::endl;
                        std::cerr << "ind = " << ind << std::endl;
                        std::cerr << "inner shed_row" << std::endl;
                        throw e;
                    }
                }
                else if (query.n_rows == 1){
                    ridges.back().InsertPoint(query(0), link_it, coefs(query(0), link_it));
                }
                else{
                    ++skip_count;
                }
                --link_it;
            } //end while
        }//end inner for
    }//end outer for
    return ridges;
}


void Vespucci::Math::PeakFinding::EstimateWidth(const arma::vec &spectrum, const arma::vec &abscissa, std::vector<Vespucci::Math::CWTRidge> &ridges)
{
    //perform width estimation on every ridge
    std::cout << "PeakFinding" << std::endl;

    try{
        for (arma::uword i = 0; i < ridges.size(); ++i){
            ridges[i].EstimateWidth(spectrum, abscissa);
        }
    }catch(std::exception e){
        std::cout << "error calling CWTRidge::EstimateWidth" << std::endl;
        throw e;
    }


}
