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
#include <Math/Accessory/accessory.h>
///
/// \brief spdiags analgous to the Octave/arma::matLAB function A = spdiags(B, d, m, n).
/// \param B a arma::matrix containing the new diagonal vectors as columns
/// \param d a vector containing the row numbers to set.  The first column
/// vector of B corresponds to the first entry in d.
/// \param m the number of rows of the output arma::matrix
/// \param n the number of columns of the output arma::matrix
/// \return a m by n sparase arma::matrix with the columns of B as diagonals
/// A translation of the Octave/arma::matLAB function A = spdiags(B, d, m, n).  For
/// subdiagonals (entries of d < 0) vectors are truncated at the end when they
/// are longer than the target diagonal.  For superdiagonals, vectors are
/// truncated at the beginning when they are longer than the target diagonal.
arma::mat Vespucci::Math::spdiags(arma::mat B, arma::ivec d, arma::uword m, arma::uword n)
{
    arma::uword i, j, k;
    arma::uword size = B.n_cols;
    arma::uword number = d.size();
    arma::uword diag_size;
    arma::uword column_size = B.n_rows;
    arma::colvec column;
    arma::mat output;
    output.zeros();
    output.set_size(m, n);
    arma::colvec diagonal;
    arma::ivec subdiagonals;
    arma::ivec superdiagonals;

    if (number > size){
        throw std::invalid_argument("spdiags: vector larger than diagonal of arma::matrix");
        return output;
    }

    superdiagonals = d(find (d >= 0));
    subdiagonals = d(find (d < 0));



    for (i = 0; i < subdiagonals.size(); ++i){
        k = subdiagonals(i);
        diagonal = output.diag(k);
        diag_size = diagonal.n_elem;
        column = B.col(i);

        if (diag_size < column_size){
            for (i = 0; i < diag_size; ++i){
                output.diag(k)(i) = column(i);
            }
        }

        else {
            for (i = 0; i < column_size; ++i){
                output.diag(k)(i) = column(i);
            }
        }
    }

    for (i = 0; i < superdiagonals.size(); ++i){
        k = superdiagonals(i);
        diag_size = output.diag(k).n_elem;
        column = B.col(i);
        if (diag_size < column_size){
            j = column_size;
            for (i = diag_size; i <=0; --i){
                output.diag(k)(i) = column(j);
                --j;
            }
        }

        else {
            for (i = 0; i < column_size; ++i){
                output.diag(k)(i) = column(i);
            }
        }
    }
    return output;
}

///
/// \brief orth Returns an orthonormal basis of the range space of A
/// \param X arma::matrix
/// \return a arma::matrix whose columns form an orthonormal basis for X
/// This function was written before MLPACK was used. The MLPACK method
/// mlpack::math::Orthogonalize does the same thing
arma::mat Vespucci::Math::orth(arma::mat X)
{
    double eps = arma::datum::eps;
    arma::mat U, V;
    arma::vec s;
    svd(U, s, V, X);
    double tol;
    tol = std::max(X.n_rows*s(1)*eps, X.n_cols*s(1)*eps);
    arma::uvec q1 = find(s > tol);
    double rank = q1.n_elem;

    if (rank > 0){
        return -1*U.cols(0, rank-1);
    }
    else{
        std::cerr << "orth: no basis found" << std::endl;
        //cout << "end orth" << endl;
        return arma::zeros(X.n_rows, X.n_cols);
    }
}



///
/// \brief Vespucci::Math::max
/// \param a
/// \param b
/// \return
/// std::max stopped working with arma::uwords for some reason, so I made a max function
/// It might be a better idea to just cast uwords to int and use std::max
arma::uword Vespucci::Math::max(arma::uword a, arma::uword b)
{
    return (a > b ? a : b);
}

///
/// \brief Vespucci::Math::min
/// \param a
/// \param b
/// \return
/// std::min stopped working with arma::uwords for some reason, so I made a min function
/// It might be a better idea to just cast uwords to int and use std::min
arma::uword Vespucci::Math::min(arma::uword a, arma::uword b)
{
    return (a < b ? a : b);
}






arma::vec Vespucci::Math::ExtendToNextPow(arma::vec X, arma::uword n)
{
    if (X.n_rows % n == 0){
        return X;
    }

    else{
        arma::uword nextpow = Vespucci::Math::NextPow(X.n_rows, n);
        arma::uword new_size = std::pow(n, nextpow);
        arma::vec padding;
        arma::uword extend_by = new_size - X.n_rows;
        if (extend_by == 1){
            padding = flipud(X.row(X.n_rows-1));
        }
        else{
            padding = flipud(X.rows(X.n_rows - extend_by, X.n_rows-1));
        }

        return join_vert(X, padding);
    }
}

arma::uword Vespucci::Math::NextPow(arma::uword number, arma::uword power)
{
    return std::ceil(std::log(number) / std::log(power));
}

///
/// \brief Vespucci::Math::LocalMaxima
/// \param X A matrix with signals as columns
/// \return A sparse matrix containing the value of each local maximum at the appropriate
/// position.
/// Find the local maxima of a matrix of signals using the first and second
/// derivative tests. Works best with smooth signals (like CWT coeffs)

arma::sp_mat Vespucci::Math::LocalMaxima(const arma::mat &X)
{
    arma::mat dX = Vespucci::Math::diff(X, 1);
    arma::mat d2X = Vespucci::Math::diff(dX, 1);
    //make everything line up.
    try{
        dX.insert_rows(0, 1, true);
        d2X.insert_rows(0, 2, true);
    }catch(std::exception e){
        std::cerr << "Error in LocalMaxima with diff calc" << std::endl;
        throw e;
    }

    return Vespucci::Math::LocalMaxima(X, dX, d2X);
}

///
/// \brief Vespucci::Math::LocalMaxima
/// \param X A matrix of signals
/// \param dX A matrix of first derivatives of signals (numerical, S-G, wavelet, etc)
/// \param d2X A matrix of second derivatives of signals (numerical, S-G, wavelet, etc).
/// \return
///
arma::sp_mat Vespucci::Math::LocalMaxima(const arma::mat &X, const arma::mat &dX, const arma::mat &d2X)
{

    arma::uvec maxima_indices, extrema_indices;
    arma::vec d2X_extrema;
    arma::umat locations;
    arma::vec values;
    arma::vec extrema_buf;
    arma::vec X_buf;
    arma::uvec position_buf_vec;
    arma::umat position_buf;

    for (arma::uword i = 0; i < X.n_cols; ++i){
        X_buf = X.col(i);
        //find where the first derivative crosses the x axis
        try{
            arma::vec search = (arma::conv_to<arma::vec>::from(dX.col(i).rows(0, X.n_rows - 2)) % arma::conv_to<arma::vec>::from(dX.col(i).rows(1, X.n_rows - 1)));
            extrema_indices = arma::find(search < 0);
        }catch(std::exception e){
            std::cerr << "find" << std::endl;
            throw e;
        }

        //find the maxima that are negative in the 2nd derivative
        try{
            d2X_extrema = ((arma::vec) d2X.col(i)).rows(extrema_indices);
            maxima_indices = arma::find(d2X_extrema < 0);
        }catch(std::exception e){
            std::cerr << "find second" << std::endl;
            throw e;
        }

        position_buf_vec = extrema_indices.rows(maxima_indices);
        position_buf.set_size(2, maxima_indices.n_rows);
        position_buf.row(0) = position_buf_vec.t();
        position_buf.row(1).fill(i);


        extrema_buf = X_buf.rows(position_buf_vec);
        try{
            values.insert_rows(values.n_rows, extrema_buf);
        }catch (std::exception e){
            std::cerr << "values.insert_rows failed" << std::endl;
            throw e;
        }
        try{
            locations.insert_cols(locations.n_cols, position_buf);
        }catch(std::exception e){
            std::cerr << "locations insert_Rows failed" << std::endl;
            throw e;
        }
    }

    return arma::sp_mat(locations, values, X.n_rows, X.n_cols, false, false);
}


arma::sp_mat Vespucci::Math::LocalMinima(const arma::mat &X)
{
    arma::mat dX = Vespucci::Math::diff(X, 1);
    arma::mat d2X = Vespucci::Math::diff(dX, 1);
    //make everything line up.
    try{
        dX.insert_rows(0, 1, true);
        d2X.insert_rows(0, 2, true);\
    }catch(std::exception e){
        std::cerr << "error in LocalMinima with diff calc" << std::endl;
        throw e;
    }

    return Vespucci::Math::LocalMinima(X, dX, d2X);
}

arma::sp_mat Vespucci::Math::LocalMinima(const arma::mat &X, const arma::mat &dX, const arma::mat &d2X)
{
    arma::uvec minima_indices, extrema_indices;
    arma::vec d2X_extrema;
    arma::umat locations;
    arma::vec values;
    arma::vec extrema_buf;
    arma::vec X_buf;
    arma::uvec position_buf_vec;
    arma::umat position_buf;

    for (arma::uword i = 0; i < X.n_cols; ++i){
        X_buf = X.col(i);
        //find where the first derivative crosses the x axis
        try{
            arma::vec search = (arma::conv_to<arma::vec>::from(dX.col(i).rows(0, X.n_rows - 2)) % arma::conv_to<arma::vec>::from(dX.col(i).rows(1, X.n_rows - 1)));
            extrema_indices = arma::find(search < 0);
        }catch(std::exception e){
            std::cerr << "find" << std::endl;
            throw e;
        }

        //find the extrema that are positive in the 2nd derivative
        try{
            d2X_extrema = ((arma::vec) d2X.col(i)).rows(extrema_indices);
            minima_indices = arma::find(d2X_extrema > 0);
        }catch(std::exception e){
            std::cerr << "find second" << std::endl;
        }

        position_buf_vec = extrema_indices.rows(minima_indices);
        position_buf.set_size(2, minima_indices.n_rows);
        position_buf.row(0) = position_buf_vec.t();
        position_buf.row(1).fill(i);


        extrema_buf = X_buf.rows(position_buf_vec);
        try{
            values.insert_rows(values.n_rows, extrema_buf);
        }catch (std::exception e){
            std::cerr << "values.insert_rows failed" << std::endl;
            throw e;
        }
        try{
            locations.insert_cols(locations.n_cols, position_buf);
        }catch(std::exception e){
            std::cerr << "locations insert_Rows failed" << std::endl;
            throw e;
        }
    }

    arma::sp_mat out;
    if (locations.n_rows !=2 || values.n_cols != locations.n_cols)
        out = arma::sp_mat(X.n_rows, X.n_cols); //all zeros because no minima found
    else
        out = arma::sp_mat(locations, values, X.n_rows, X.n_cols, false, false);
    return out;
}


///
/// \brief Vespucci::Math::position Find row and column numbers for index
/// \param index
/// \param n_rows Number of rows
/// \param n_cols Number of columns
/// \param i Row number
/// \param j Column number
///
void Vespucci::Math::position(arma::uword index, arma::uword n_rows, arma::uword n_cols, arma::uword &i, arma::uword &j)
{

    //first row (0) is 0 to n - 1
    //second row (1) is n to 2n - 1
    //third row (2) is 2n to 3n - 1,
    //etc.

    //the row number is thus, floor(index/n);
    //the col number is index - (row number) * n


    if (index >= n_rows*n_cols)
        throw std::invalid_argument("index out of bounds");
    i = (arma::uword) std::floor(index / n_cols);
    j = (arma::uword) std::floor(index - i*n_cols);
}


arma::umat Vespucci::Math::to_row_column(arma::uvec indices,
                                         arma::uword n_rows, arma::uword n_cols)
{
    arma::umat matrix_indices(indices.n_rows, 2);
    arma::uword row, column;
    for(arma::uword i = 0; i < indices.n_rows; ++i){
        position(indices(i), n_rows, n_cols, row, column);
        matrix_indices(i, 0) = row;
        matrix_indices(i, 1) = column;
    }

    return matrix_indices;
}


double Vespucci::Math::quantile(arma::vec &data, double probs)
{
    if (probs > 1 || probs < 0){
        throw std::invalid_argument("quantile: probs must be between 0 and 1");
    }
    if (data.n_rows < 1){
        throw std::invalid_argument("quantile: empty input vector");
    }

    double h = (data.n_rows - 1)*probs; //normally you'd add one here, but remember indexing starts at 0

    if (data.in_range(data.in_range(std::floor(h) && std::floor(h) + 1))){
        return data(std::floor(h)) + (h - std::floor(h))*(data(std::floor(h) + 1) - data(std::floor(h)));
    }
    //current or next value is too high, so take last
    else if ((std::floor(h) >= data.n_rows) || (std::floor(h) + 1 >= data.n_rows)){
        return data(data.n_rows - 1);
    }
    else{
        throw std::runtime_error("quantile not computed");
    }
}


double Vespucci::Math::mad(arma::vec &data)
{
    double median;
    double value;
    arma::vec sorted = sort(data);
    //calculate median (we do it this way because of an error in MinGW)
    arma::uword center = sorted.n_rows / 2;
    if (sorted.n_rows % 2 != 0){
        median = 0.5*(sorted(center) + sorted(center+1));
    }
    else{
        median = sorted(center);
    }

    data -= median;
    sorted = sort(data);

    if (sorted.n_rows % 2 != 0){
        value = 0.5*(sorted(center) + sorted(center+1));
    }
    else{
        value = sorted(center);
    }
    return value;
}





///
/// \brief Vespucci::Math::RecalculateAverage Recalculate the average value when
/// a new value is added to a list for which only the previous means, stddevs
/// and counts are known.
/// \param new_value
/// \param old_average
/// \param old_count
/// \return
///
double Vespucci::Math::RecalculateAverage(double new_value,
                                          double old_average,
                                          double old_count)
{
    return ((old_average * old_count) + new_value) / (old_count + 1.0);
}

///
/// \brief Vespucci::Math::RecalculateStdDev Recalculate a standard deviation
/// when a new value is added to a list for which only the previous means, stddevs
/// and counts are known.
/// \param new_value
/// \param old_mean
/// \param old_stddev
/// \param old_count
/// \return
///
double Vespucci::Math::RecalculateStdDev(double new_value,
                                         double old_mean,
                                         double old_stddev,
                                         double old_count)

{
    double sum_x = old_count*old_mean + new_value;
    double sum_squares = old_count*(std::pow(old_stddev, 2.0)
                         + std::pow(old_mean, 2.0))
                         + std::pow(new_value, 2.0);

    double new_stddev = std::sqrt(
                        ((old_count + 1.0)*sum_squares - std::pow(sum_x, 2.0))
                         / std::pow((old_count + 1.0), 2.0));
    return new_stddev;
}

///
/// \brief Vespucci::Math::GetClosestValues
/// \param query
/// \param target
/// \param k
/// \return The indices of the k values in target closest to each value in query
/// k should be odd in case query and target contain equal values
arma::umat Vespucci::Math::GetClosestValues(arma::vec query,
                                            arma::vec target,
                                            const arma::uword k)
{
    //ensure sortedness
    if(!query.is_sorted()){query = arma::sort(query);}
    if(!target.is_sorted()){target = arma::sort(target);}
    arma::umat closest_indices(k, query.n_rows);

    for (arma::uword i = 0; i < query.n_rows; ++i){
        arma::vec difference = arma::abs(target - query(i));
        arma::uvec ind = arma::stable_sort_index(difference);
        closest_indices.col(i) = ind.rows(0, k-1);
    }
    return closest_indices;
}

///
/// \brief Vespucci::Math::CalcPoly Calculate the value of a polynomial at particular point
/// \param x The value at which to evaluate a polynomial.
/// \param coefs The coefficients of the polynomial ordered from constant to highest order
/// \return The value of the polynomial at x
///
double Vespucci::Math::CalcPoly(const double x, const arma::vec &coefs)
{
    if (!coefs.n_rows){throw std::invalid_argument("coefs empty!");}
    double y = coefs(0);
    for (arma::uword i = 1; i < coefs.n_rows; ++i){y += coefs(i)*std::pow(x,i);}
    return y;
}

///
/// \brief Vespucci::Math::WavelengthToFrequency
/// \param x
/// \param freq_factor
/// \param wl_factor
/// \return
///
arma::vec Vespucci::Math::WavelengthToFrequency(const arma::vec &x, double freq_factor, double wl_factor)
{

    //convert input to meters, then Hz, then preferred wavelength
    //nu = lambda/c
    return (wl_factor * (1.0/arma::datum::c_0) * freq_factor) * x;
}

///
/// \brief Vespucci::Math::FrequencyToWavelength
/// \param x
/// \param wl_factor
/// \param freq_factor
/// \return
///
arma::vec Vespucci::Math::FrequencyToWavelength(const arma::vec &x, double wl_factor, double freq_factor)
{
    //convert input to Hz then to wavelength in meters, then to wavelength in perferred units
    //lambda = nu * c
    return (freq_factor * arma::datum::c_0 * wl_factor) * x;
}

///
/// \brief Vespucci::Math::FrequencyToEnergy
/// \param x
/// \param energy_factor
/// \param freq_factor
/// \return
///
arma::vec Vespucci::Math::FrequencyToEnergy(const arma::vec &x, double energy_factor, double freq_factor)
{
    //convert input to Hz then to energy in J, then to energy in preferred units
    //E = h * nu
    return (freq_factor * arma::datum::h * energy_factor) * x;
}

///
/// \brief Vespucci::Math::EnergyToFrequency
/// \param x
/// \param freq_factor
/// \param energy_factor
/// \return
///
arma::vec Vespucci::Math::EnergyToFrequency(const arma::vec &x, double freq_factor, double energy_factor)
{
    //convert input to Hz then to energy in J, then to energy in preferred units
    //nu = E/h
    return (energy_factor * (1.0/arma::datum::h) * freq_factor) * x;
}

///
/// \brief Vespucci::Math::WavenumberToFrequency
/// \param x
/// \param freq_factor
/// \param wn_factor
/// \return
///
arma::vec Vespucci::Math::WavenumberToFrequency(const arma::vec &x, double freq_factor, double wn_factor)
{
    //convert input to inverse meters, then meters, then preferred units
    arma::vec foo = x * wn_factor;
    foo.transform( [](double val) {return (1.0/val); } );
    return freq_factor * foo;
}

///
/// \brief Vespucci::Math::FrequencyToWavenumber
/// \param x
/// \param wn_factor
/// \param freq_factor
/// \return
///
arma::vec Vespucci::Math::FrequencyToWavenumber(const arma::vec &x, double wn_factor, double freq_factor)
{
    //convert input to meters, then inverse meters, then preferred units
    arma::vec foo = x * freq_factor;
    foo.transform( [](double val) {return (1.0/val); } );
    return wn_factor * foo;
}

///
/// \brief Vespucci::Math::WavenumberToWavelength
/// \param x
/// \param wn_factor
/// \param wl_factor
/// \return
///
arma::vec Vespucci::Math::WavenumberToWavelength(const arma::vec &x, double wn_factor, double wl_factor)
{
    //convert input to inverse meters, then to meters, then to preferred units
    arma::vec foo = x * wn_factor;
    foo.transform( [](double val) {return (1.0/val); } );
    return wl_factor * foo;
}

///
/// \brief Vespucci::Math::WavelengthToWavenumber
/// \param x
/// \param wl_factor
/// \param wn_factor
/// \return
///
arma::vec Vespucci::Math::WavelengthToWavenumber(const arma::vec &x, double wl_factor, double wn_factor)
{
    //convert to meters, then inverse meters, then desired units
    arma::vec foo = wl_factor * x;
    foo.transform( [](double val) {return (1.0/val); } );
    return wn_factor * foo;
}

///
/// \brief Vespucci::Math::WavelengthToEnergy
/// \param x
/// \param energy_factor
/// \param wl_factor
/// \return
///
arma::vec Vespucci::Math::WavelengthToEnergy(const arma::vec &x, double energy_factor, double wl_factor)
{
    arma::vec foo = wl_factor * x;
    //convert to frequency, then to energy
    foo = Vespucci::Math::WavelengthToFrequency(foo, 1, 1);
    return Vespucci::Math::FrequencyToEnergy(foo, energy_factor, 1);
}

///
/// \brief Vespucci::Math::EnergyToWavelength
/// \param x
/// \param wl_factor
/// \param energy_factor
/// \return
///
arma::vec Vespucci::Math::EnergyToWavelength(const arma::vec &x, double wl_factor, double energy_factor)
{
    arma::vec foo = energy_factor * x;
    foo = Vespucci::Math::EnergyToFrequency(x, 1, 1);
    return Vespucci::Math::FrequencyToWavelength(foo, wl_factor, 1);
}

///
/// \brief Vespucci::Math::EnergyToWavenumber
/// \param x
/// \param wn_factor
/// \param energy_factor
/// \return
///
arma::vec Vespucci::Math::EnergyToWavenumber(const arma::vec &x, double wn_factor, double energy_factor)
{
    arma::vec foo = Vespucci::Math::EnergyToWavelength(x, 1, energy_factor);
    return Vespucci::Math::WavelengthToWavenumber(foo, 1, wn_factor);
}

///
/// \brief Vespucci::Math::WavenumberToEnergy
/// \param x
/// \param energy_factor
/// \param wn_factor
/// \return
///
arma::vec Vespucci::Math::WavenumberToEnergy(const arma::vec &x, double energy_factor, double wn_factor)
{
    arma::vec foo = Vespucci::Math::WavenumberToWavelength(x, wn_factor, 1);
    return Vespucci::Math::WavelengthToEnergy(x, energy_factor, 1);
}


arma::sp_mat Vespucci::Math::LocalMaximaCWT(arma::mat coefs, arma::uvec scales, arma::uword min_window_size)
{
    arma::uword window_size;
    arma::vec current_coefs;
    arma::sp_mat local_maxima_col;
    arma::sp_mat local_maxima(coefs.n_rows, coefs.n_cols);
    for (arma::uword i = 0; i < scales.n_rows; ++i){
        current_coefs = coefs.col(scales(i));
        window_size = scales(i)*2 + 1;
        window_size = (min_window_size < window_size ? window_size: min_window_size);
        local_maxima.col(i) = Vespucci::Math::LocalMaximaWindow(current_coefs, window_size);
    }
    return local_maxima;
}

///
/// \brief Vespucci::Math::LocalMinimaWindow
/// \param X
/// \param window_size
/// \return
///
arma::sp_mat Vespucci::Math::LocalMinimaWindow(const arma::mat &X, const int window_size)
{
    arma::sp_mat local_mins = Vespucci::Math::LocalMinima(X);
    //keep only the highest value in each window:
    arma::mat local_min_col;
    arma::uword length = std::ceil(X.n_rows/window_size);
    arma::uword old_len = X.n_rows;
    for (arma::uword j = 0; j < X.n_cols; ++j){
        local_min_col = local_mins.col(j);
        local_min_col.resize(length, window_size);
        arma::vec lowest = arma::min(local_min_col, 1);
        for (arma::uword i = 0; i < local_min_col.n_rows; ++i){
            arma::uvec ind = arma::find(local_min_col.row(i) != arma::as_scalar(lowest.row(i)));
            local_min_col.elem(ind).zeros();
        }
        local_min_col.resize(old_len, 1);
        local_mins.col(j) = arma::sp_vec(local_min_col);
    }

    return local_mins;
}

///
/// \brief Vespucci::Math::LocalMaximaWindow
/// "Cleans" local maximum by window search to remove extraneous values.
/// \param X
/// \param window_size
/// \return
///
arma::sp_mat Vespucci::Math::LocalMaximaWindow(const arma::mat &X, const int window_size)
{
    arma::sp_mat local_maxes = Vespucci::Math::LocalMaxima(X);
    //keep only the highest value in each window:
    arma::mat local_max_col;
    arma::uword length = std::ceil(X.n_rows/window_size);
    arma::uword old_len = X.n_rows;
    for (arma::uword j = 0; j < X.n_cols; ++j){
        local_max_col = local_maxes.col(j);
        local_max_col.resize(length, window_size);
        arma::vec highest = arma::max(local_max_col, 1);
        for (arma::uword i = 0; i < local_max_col.n_rows; ++i){
            arma::uvec ind = arma::find(local_max_col.row(i) != arma::as_scalar(highest.row(i)));
            local_max_col.elem(ind).zeros();
        }
        local_max_col.resize(old_len, 1);
        local_maxes.col(j) = arma::sp_mat(local_max_col);
    }

    return local_maxes;
}

///
/// \brief AreEqual
/// \param a
/// \param b
/// \return Whether or not all elements in the two vectors are equal
///
bool Vespucci::Math::AreEqual(const arma::vec &a, const arma::vec &b)
{
    if (a.n_rows != b.n_rows){
        return false;
    }
    bool equal;
    arma::uword i = 0;
    do{
        equal = a(i) == b(i);
        i++;
    }while(equal && (i < a.n_rows));
    return equal;
}

///
/// \brief IsMonotonic
/// \param x
/// \return
///
bool Vespucci::Math::IsMonotonic(const arma::vec &x)
{
    arma::vec deriv = Vespucci::Math::diff(x, 1);
    double val = deriv(0);
    bool equal;
    arma::uword i = 1;
    do{
        equal = (deriv(i++) == val);
    }while(equal && (i < deriv.n_rows));
    return equal;
}

///
/// \brief IsIncreasing
/// \param x
/// \return
///
bool Vespucci::Math::IsIncreasing(const arma::vec &x)
{
    arma::vec deriv = Vespucci::Math::diff(x, 1);
    arma::uword i = 0;
    bool positive;
    do{
        positive = (deriv(i++) > 0);
    }while(positive && (i < deriv.n_rows));
    return positive;
}
