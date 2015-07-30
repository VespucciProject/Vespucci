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
#include <Math/Smoothing/smoothing.h>

#include <Math/Fitting/linleastsq.h>


///
/// \brief Vespucci::Math::Smoothing::sgolay Find a arma::matrix of Savitzky-Golay convolution coefficients
/// \param poly_order
/// \param window_size
/// \param deriv_order
/// \param scaling_factor
/// \return A arma::matrix of size window_size by window_size containing coefficients.
/// Finds a arma::matrix of Savitzky-Golay convolution coefficients, similar to the
/// sgolay function in the Octave and arma::matLAB signal packages. The central row is
/// the filter used for most of the dataset. The outer rows are used on the edges
/// of the signal to be filtered.
///
arma::mat Vespucci::Math::Smoothing::sgolay(arma::uword poly_order,
                     arma::uword window_size,
                     arma::uword deriv_order,
                     arma::uword scaling_factor)
{
    if (window_size % 2 == 0){
        std::cerr << "sgolay: invalid window size. Window size must be odd." << std::endl;
        std::cerr << "using window size poly_order + 2 or nearest odd value" << std::endl;
        window_size = poly_order + 3 - (poly_order % 2);
    }

    if (deriv_order > poly_order){
        std::cerr << "sgolay: derivative order greater than polynomial order." << std::endl;
        std::cerr << "using derivative order poly order - 1" << std::endl;
        deriv_order = poly_order - 1;
    }
    arma::mat F = arma::zeros(window_size, window_size);
    arma::uword k = (window_size - 1) / 2;
    arma::mat C(window_size, poly_order + 1);
    arma::sword signed_window_size = window_size;
    arma::vec column;
    arma::sword row; //starting at 1 not 0
    arma::mat A;
    for (arma::uword i = 0; i <= k; ++i){
        row = i + 1;
        column = arma::linspace<arma::vec>((1-row), (signed_window_size - row), window_size);
        for (arma::uword j = 0; j <= poly_order; ++j){
            C.col(j) = arma::pow(column, j);
        }
        A = pinv(C);
        F.row(i) = A.row(deriv_order);
    }
    arma::sword sign = (deriv_order % 2 == 0 ? 1: -1);
    arma::uword start_index = 0;
    for (arma::uword i = window_size - 1; i >k; --i){
        F.row(i) = sign*F.row(start_index);
        start_index++;
    }
    double product = (deriv_order == 0 ? 1 : prod(arma::linspace<arma::vec>(1, deriv_order, deriv_order)));
    double power = pow(scaling_factor, deriv_order);
    F /= (power/product);
    return F;
}

///
/// \brief sgolayfilt Apply Savitzky-Golay smoothing to each column of a arma::matrix
/// \param x Input arma::matrix. Each column is a signal
/// \param poly_order Polynomial order for smoothing
/// \param window_size Size of filter window. Must be odd and larger than poly order
/// \param deriv_order Derivative order, to extract derivatized data directly
/// \param scaling_factor A scaling factor
/// \return Smoothed data
///
arma::mat Vespucci::Math::Smoothing::sgolayfilt(const arma::mat &x, arma::uword poly_order, arma::uword window_size, arma::uword deriv_order, arma::uword scaling_factor)
{
    arma::mat return_value(x.n_rows, x.n_cols);

    //this function filters by column.
    //any invalid window size is set to preferred poly_order + 2 or poly_order + 3 for even poly_order
    if ((poly_order > window_size) || (window_size % 2 == 0)){
        window_size = poly_order + 3 - (poly_order % 2);
    }
    //if deriv_order is too high, make it one less than polynomial order
    if (deriv_order > poly_order){
        deriv_order = poly_order - 1;
    }
    if (x.n_rows < window_size){
        std::cerr << "sgolayfilt: not enough data for filter window of this size" << std::endl;
        return x;
    }

    arma::mat coefficients = sgolay(poly_order, window_size, deriv_order, scaling_factor);

    for (arma::uword i = 0; i < x.n_cols; ++i)
        return_value.col(i) = ApplyFilter(x.col(i), coefficients, window_size);

    return return_value;
}

///
/// \brief Vespucci::Math::Smoothing::ApplyFilter Apply FIR filters to a column vector.
/// \param x Data to be filtered
/// \param coefficients A arma::matrix of FIR filters.
/// \param window_size Filter window size
/// \return Filtered data
/// Apply FIR filters to a column vector. The central row of coefficients
/// contains the filter used for most of the data. The first (window_size - 1)/2
/// rows are filtered with the lower rows ofccoefficients, and likewise for the
/// last (window_size - 1)/2 rows and the first rows of coefficients.
///
arma::vec Vespucci::Math::Smoothing::ApplyFilter(const arma::vec &x, arma::mat coefficients, arma::uword window_size)
{
    arma::uword k = (window_size - 1) / 2;
    arma::vec filter = trans(coefficients.row(k));
    //coefficients output by sgolay are in reverse order expected.
    filter = fliplr(filter);
    arma::vec filtered_data = conv(filter, x);
    //for conv() to be equivalent to filter, we have to chop off the last window_size - 1 elements
    filtered_data = filtered_data.rows(0, x.n_rows - 1);
    //Now we have to fix the ends of the filtered data
    filtered_data.rows(0, k) = coefficients.rows(k, window_size-1)*x.rows(0, window_size-1);
    filtered_data.rows(x.n_rows - k, x.n_rows - 1) = coefficients.rows(0, k - 1)*x.rows(x.n_rows - window_size, x.n_rows - 1);
    return filtered_data;
}

///
/// \brief Vespucci::Math::Smoothing::ApplyFilter
/// \param x The vector to filter
/// \param filter The filter
/// \return Filtered data
/// Applies an FIR filter to x
arma::vec Vespucci::Math::Smoothing::ApplyFilter(const arma::vec &x, arma::vec filter)
{
    arma::uword k = (filter.n_elem - 1)/ 2;
    arma::vec out = conv(filter, x);
    //cut off the part that hangs over
    out = out.rows(0, x.n_elem - 1);
    //replace boundaries with values from the original
    out.rows(0, k) = x.rows(0, k);
    out.rows(x.n_elem - k, x.n_elem - 1) = x.rows(x.n_elem - k, x.n_elem - 1);
    return out;
}

///
/// \brief Vespucci::Math::Smoothing::CreateMovingAverageFilter
/// \param window_size The window size of the filter.
/// \return A moving average filter
/// Create a moving average filter of a certain window size.
arma::vec Vespucci::Math::Smoothing::CreateMovingAverageFilter(arma::uword window_size)
{
    double value = 1 / (double) window_size;
    arma::vec filter(window_size);
    filter.fill(value);
    return filter;
}

///
/// \brief InterpolateToNewAbscissa
/// \param spectra
/// \param old_abscissa
/// \param new_abscissa
/// \param window_size
/// \param order
/// \return
///
arma::mat Vespucci::Math::Smoothing::InterpolateToNewAbscissa(const arma::mat &spectra,
                                                              const arma::vec &old_abscissa,
                                                              const arma::vec &new_abscissa,
                                                              const int window_size,
                                                              const int order)
{
    mat new_spectra(new_abscissa.n_rows, spectra.n_cols);

    arma::umat closest_indices = Vespucci::Math::GetClosestValues(new_abscissa,
                                                                  old_abscissa,
                                                                  window_size);


    for (arma::uword j = 0; j < spectra.n_cols; ++j){
        arma::vec spectrum = spectra.col(j);

        for (arma::uword i = 0; i < new_abscissa.n_rows; ++i){
            arma::vec x = old_abscissa(closest_indices.col(i));
            arma::vec y = spectrum(cloest_indices.col(i));
            arma::mat X = Vespucci::Math::LinLeastSq::Vandermonde(x, order);
            arma::vec coefs = Vespucci::Math::LinLeastSq::OrdinaryLeastSquares(X, y);
            new_spectra(i, j) = Vespucci::Math::CalcPoly(new_abscissa(i), coefs);
        }//over abscissa values
    }//over spectra

    return new_spectra;
}
