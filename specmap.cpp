/************************************************************************************
    Copyright (C) 2014 Daniel P. Foose - All Rights Reserved

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
***************************************************************************************/

#include "specmap.h" //specmap includes all necessary headers.
#include <cmath>
#include <fstream>
#include <QtConcurrent/QtConcurrentRun>
#include "arma_ext.h"
#include "mainwindow.h"



using namespace arma;
using namespace std;

SpecMap::SpecMap()
{

}

SpecMap::~SpecMap()
{
    //make sure principal components stats are deleted properly.
    if (principal_components_calculated_)
        delete principal_components_data_;

    //make sure all maps are delted properly.
    for (int i = 0; i < maps_.size(); ++i)
        RemoveMapAt(i);
}


///
/// \brief SpecMap::SpecMap
/// \param binary_file_name file name of the binary
/// \param main_window the main window
/// \param directory the working directory
/// This constructor loads a previously saved dataset.  The dataset is saved
/// as an armadillo binary in the same format as the long text.
///
SpecMap::SpecMap(QString binary_file_name, MainWindow *main_window, QString *directory)
{
    //Set up variables unrelated to hyperspectral data:
    map_list_widget_ = main_window->findChild<QListWidget *>("mapsListWidget");
    map_loading_count_ = 0;
    principal_components_calculated_ = false;
    partial_least_squares_calculated_ = false;
    z_scores_calculated_ = false;
    directory_ = directory;

    mat input_data;
    input_data.load(binary_file_name.toStdString());
    int cols = input_data.n_cols;
    int rows = input_data.n_rows;
    int wavelength_size = cols - 2;
    int spatial_size = rows - 1;
    x_.set_size(spatial_size);
    y_.set_size(spatial_size);
    wavelength_.set_size(wavelength_size);
    spectra_.set_size(spatial_size, wavelength_size);

    wavelength_ = input_data(0, span(2, cols));
    x_ = input_data(span(1, rows), 0);
    y_ = input_data(span(1, rows), 1);
    spectra_ = input_data(span(1, rows), span(2, cols));
    main_window_ = main_window;
}




///
/// \brief SpecMap::SpecMap
/// Main function for processing data from text files to create SpecMap objects.
/// Currently written to accept files in "wide" format, will be expanded to deal
/// with different ASCII formats later with conditionals.
/// \param inputstream a text stream derived from the input file
/// \param main_window the main window of the app
/// \param directory the working directory
///
SpecMap::SpecMap(QTextStream &inputstream, MainWindow *main_window, QString *directory, bool swap_spatial)
{
    //Set up variables unrelated to hyperspectral data:
    map_list_widget_ = main_window->findChild<QListWidget *>("mapsListWidget");
    map_loading_count_ = 0;
    principal_components_calculated_ = false;
    partial_least_squares_calculated_ = false;
    z_scores_calculated_ = false;
    directory_ = directory;
    flipped_ = swap_spatial;
    int i, j;
    wall_clock timer;

    /*Read the first line to get the wavelength*/
    inputstream.seek(0);
    cout << "Loading wavelength vector..." << endl;
    timer.tic();
    QString wavelength_string = inputstream.readLine();

    QStringList wavelength_string_list =
            wavelength_string.split("\t",  QString::SkipEmptyParts);

    int columns = wavelength_string_list.size();
    wavelength_.set_size(columns);

    for(i=0; i<columns; ++i){
        wavelength_(i) = wavelength_string_list.at(i).toDouble();
    }
    double seconds = timer.toc();
    cout << "Reading wavelength took " << seconds <<" s." << endl;
    i=0;
    j=0;

    QString spectra_string;

    QStringList spectra_string_list;
    QProgressDialog progress("Counting rows...", "Cancel", 0, 100, NULL);
    progress.setWindowTitle("Loading Dataset");
    progress.setWindowModality(Qt::WindowModal);

    int rows = 0;
    cout << "Counting rows..." << endl;
    timer.tic();
    while(inputstream.readLine()!=NULL){
        ++rows;
    }
    progress.setValue(1);
    progress.setRange(0,rows+1);

    spectra_.set_size(rows, columns);
    x_.set_size(rows);
    y_.set_size(rows);
    seconds = timer.toc();
    cout << "Counting rows and resizing took " << seconds << " s." << endl;
    cout << "Reading spectra, x, and y..." << endl;

    progress.setLabelText("Parsing spectra...");

    timer.tic();
    inputstream.seek(0);
    inputstream.readLine(); //discard it to advance to next line

    for(i=0; i<rows; ++i){
        spectra_string=inputstream.readLine();
        spectra_string_list =
                spectra_string.split("\t", QString::SkipEmptyParts);

        if (swap_spatial){
            y_(i) = spectra_string_list.at(0).toDouble();
            spectra_string_list.removeAt(0);

            x_(i) = spectra_string_list.at(0).toDouble();
            spectra_string_list.removeAt(0);
        }
        else{
            x_(i) = spectra_string_list.at(0).toDouble();
            spectra_string_list.removeAt(0);

            y_(i) = spectra_string_list.at(0).toDouble();
            spectra_string_list.removeAt(0);
        }

        for (j=0; j<columns; ++j){
            spectra_(i,j) = spectra_string_list.at(j).toDouble();
        }
        if (progress.wasCanceled()){
            constructor_canceled_ = true;
            return;
        }
        progress.setValue(i);
    }
    seconds = timer.toc();
    constructor_canceled_ = false;
    cout << "Reading x, y, and spectra took " << seconds << " s." << endl;
    main_window_ = main_window;
}

// PRE-PROCESSING FUNCTIONS //

///
/// \brief SpecMap::CropSpectra
/// \param x_min value of x below which spectra are deleted
/// \param x_max value of x above which spectra are deleted
/// \param y_min value of y below which spectra are deleted
/// \param y_max value of y above which spectra are deleted
/// removes all data points outside of the range.
void SpecMap::CropSpectra(double x_min, double x_max, double y_min, double y_max)
{
    int max = x_.n_rows;
    QProgressDialog progress("Cropping...", "Cancel", 0, max);
    progress.setWindowModality(Qt::WindowModal);

    for (int i = 0; i < max; ++i){
        if ((x_(i) < x_min) || (x_(i) > x_max) || (y_(i) < y_min) || (y_(i) > y_max)){
            spectra_.shed_row(i);
            x_.shed_row(i);
            y_.shed_row(i);
            --i; //forces repeat of same index after deletion
            --max;
        }
        progress.setValue(i);
    }
}


///
/// \brief SpecMap::MinMaxNormalize
///normalizes data so that smallest value is 0 and highest is 1 through the
/// entire spectra_ matrix.  If the minimum of spectra_ is negative, it subtracts
/// this minimum from all points.  The entire spectra_ matrix is then divided
/// by the maximum of spectra_
void SpecMap::MinMaxNormalize()
{
    int n_elem = spectra_.n_elem;
    double minimum = spectra_.min();
    if (minimum < 0)
        for (int i = 0; i < n_elem; ++i)
            spectra_(i) = spectra_(i) - minimum;
    double maximum = spectra_.max();
    spectra_ = spectra_/maximum;
}
///
/// \brief SpecMap::UnitAreaNormalize
///normalizes the spectral data so that the area under each point spectrum is 1
void SpecMap::UnitAreaNormalize()
{
    int num_rows = spectra_.n_rows;
    int num_cols = spectra_.n_cols;
    for (int i = 0; i < num_rows; ++i){
        rowvec row = spectra_.row(i);
        double row_sum = sum(row);
        for (int j = 0; j < num_cols; ++j){
            spectra_(i, j) = spectra_(i, j) / row_sum;
        }
    }
}

mat SpecMap::ZScoreNormCopy()
{
    int num_rows = spectra_.n_rows;
    int num_cols = spectra_.n_cols;
    mat normalized_copy(num_rows, num_cols);
    for (int j = 0; j < num_cols; ++j){
        double mean = arma::mean(spectra_.col(j));
        double standard_deviation = arma::stddev(spectra_.col(j));
        for (int i = 0; i < num_rows; ++i){
            normalized_copy(i, j) = (spectra_(i, j) - mean) / standard_deviation;
        }
    }

    return normalized_copy;
}

///
/// \brief SpecMap::ZScoreNormalize
///Computes a Z score for every entry based on the distribution of its column,
/// assuming normality of "population".  Because some values will be negative,
/// this must be accounted for in Univariate Mapping Functions.
///
void SpecMap::ZScoreNormalize()
{
    int num_rows = spectra_.n_rows;
    int num_cols = spectra_.n_cols;
    for (int j = 0; j < num_cols; ++j){
        double mean = arma::mean(spectra_.col(j));
        double standard_deviation = arma::stddev(spectra_.col(j));
        for (int i = 0; i < num_rows; ++i){
            spectra_(i, j) = (spectra_(i, j) - mean) / standard_deviation;
        }
    }
    z_scores_calculated_ = true;
}

void SpecMap::SubtractBackground(mat background)
{
    if (background.n_cols != spectra_.n_cols){
        QMessageBox::warning(0,
                             "Improper Dimensions!",
                             "The background spectrum has a different number of"
                             " points than the map data."
                             " No subtraction can be performed");
        return;
    }
    else{
        spectra_.each_row() -= background.row(0);
    }
}

void SpecMap::Baseline(QString method, int window_size)
{
    if (method == "Median Filter"){
        int starting_index = (window_size - 1) / 2;
        int ending_index = wavelength_.n_cols - starting_index;
        int i, j;
        int rows = spectra_.n_rows;
        int columns = spectra_.n_cols;
        rowvec window;
        mat processed;
        window.set_size(window_size);
        processed.set_size(spectra_.n_rows, spectra_.n_cols);

        for (i = 0; i < rows; ++i){
            for (j = 0; j < starting_index; ++j){
                processed(i, j) = spectra_(i, j);
            }
            for (j = ending_index; j < columns; ++j){
                processed(i, j) = spectra_(i, j);
            }
            for (j = starting_index; j < ending_index; ++j){
                window = spectra_(i, span((j - starting_index), (j+starting_index)));
                processed(i, j) = median(window);
            }
        }
        spectra_ -= processed;
    }
}

//Filtering functions

///
/// \brief SpecMap::MedianFilter
/// \param window_size - an odd number representing the width of the window.
///performs median filtering on the spectral data.  Entries near the boundaries
/// are not processed.
void SpecMap::MedianFilter(int window_size)
{
    int starting_index = (window_size - 1) / 2;
    int ending_index = wavelength_.n_cols - starting_index;
    int i, j;
    int rows = spectra_.n_rows;
    int columns = spectra_.n_cols;
    rowvec window;
    mat processed;
    window.set_size(window_size);
    processed.set_size(spectra_.n_rows, spectra_.n_cols);

    for (i = 0; i < rows; ++i){
        for (j = 0; j < starting_index; ++j){
            processed(i, j) = spectra_(i, j);
        }
        for (j = ending_index; j < columns; ++j){
            processed(i, j) = spectra_(i, j);
        }
        for (j = starting_index; j < ending_index; ++j){
            window = spectra_(i, span((j - starting_index), (j+starting_index)));
            processed(i, j) = median(window);
        }
    }
    spectra_ = processed;
}

///
/// \brief SpecMap::LinearMovingAverage
/// \param window_size - an odd number representing the width of the window.
/// performs moving average filtering on the spectral data.  Entries near the
/// boundaries are not processed.  See also SpecMap::MedianFilter.
void SpecMap::LinearMovingAverage(int window_size)
{
    int starting_index = (window_size - 1) / 2;
    int ending_index = wavelength_.n_cols - starting_index;
    int i, j;
    int rows = spectra_.n_rows;
    int columns = spectra_.n_cols;
    rowvec window;
    mat processed;
    window.set_size(window_size);
    processed.set_size(spectra_.n_rows, spectra_.n_cols);

    for (i = 0; i < rows; ++i){
        for (j = 0; j < starting_index; ++j){
            processed(i, j) = spectra_(i, j);
        }
        for (j = ending_index; j < columns; ++j){
            processed(i, j) = spectra_(i, j);
        }
        for (j = starting_index; j < ending_index; ++j){
            window = spectra_(i, span((j - starting_index), (j+starting_index)));
            processed(i, j) = mean(window);
        }
    }
    spectra_ = processed;
}

///
/// \brief SpecMap::SingularValue
/// Denoises the spectra matrix using a singular value decomposition.  The first
/// 5 singular values are used.
void SpecMap::SingularValue()
{
    bool ok = QMessageBox::question(0,
                                    "Singular Value Decomposition",
                                    "The singular value decomposition takes"
                                    " several seconds to complete.  The program"
                                    " may appear to freeze during this time."
                                    " Are you sure you want to continue?");
    if (!ok)
        return;
    mat U;
    vec s;
    mat V;

    wall_clock timer;
    timer.tic();
    cout << "call to svds" << endl;
    bool success = arma_ext::svds(spectra_, 6, U, s, V);
    cout << "took " << timer.toc() << " s" << endl;
    timer.tic();
    spectra_ = -1 * U * diagmat(s) * V.t();
    cout << "reconstruction took " << timer.toc() << " s" << endl;

}


void SpecMap::Derivatize(int derivative_order,
                         int polynomial_order,
                         int window_size)
{
    int i, j;
    int columns = wavelength_.n_elem;
    int p = (window_size - 1) / 2;
    mat x;
    x.set_size(window_size, 1 + polynomial_order);
    int p_buf = -p;

    for (i = 0; i <window_size; ++i){
        for (j = 0; j <= polynomial_order; ++j){
            x(i, j) = p_buf ^ j;
        }
        ++p_buf;
    }

    mat weights = solve(x, eye(window_size, window_size));

    mat coeff_mat;

    coeff_mat.set_size(derivative_order, polynomial_order + 1 - derivative_order);

    for (i = 0; i < derivative_order; ++i){
        for (j = 0; j < polynomial_order + 1 - derivative_order; ++j){
            coeff_mat(i, j) = j + 1.0 + i;
        }
    }



    rowvec coeff = prod(coeff_mat);
    mat diagonals;
    diagonals.set_size(columns, window_size);
    for (i = 0; i < columns; ++i){
        diagonals.row(i) =
                weights.row(derivative_order) * coeff(0);
    }

    QVector<int> p_range;
    for (i = -p; i <= p; ++i){
        p_range.append(i);
    }



    mat SG_Coefficients =
            arma_ext::spdiags(diagonals,
                              p_range,
                              columns,
                              columns);

    mat weights_submatrix;
    weights_submatrix.set_size(polynomial_order - derivative_order + 1, window_size);

    for (i = 0; i < polynomial_order - derivative_order + 1; ++i){
        for (j = 0; j < window_size; ++j){
            weights_submatrix(i, j) = weights(i + derivative_order, j);
        }
    }

    mat w1 = diagmat(coeff)*weights_submatrix;

    mat x_submatrix_1;
    x_submatrix_1.set_size(p, 1+ polynomial_order - derivative_order);
    mat x_submatrix_2;
    x_submatrix_2.set_size(p, 1 + polynomial_order - derivative_order);

    for (i = 0; i < p; ++i){
        for (j = 0; j <= polynomial_order - derivative_order; ++j){
            x_submatrix_1(i, j) = x(i, j);
        }
    }

    int x_row = p;

    for (i = 0; i < p; ++i){
        for (j = 0; j <= polynomial_order - derivative_order; ++j){
            x_submatrix_2(i, j) = x(x_row, j);
        }
        ++x_row;
    }


    mat x_product_1 = x_submatrix_1 * w1;
    mat x_product_2 =x_submatrix_2 * w1;

    mat x_product_1_transpose = x_product_1.t();
    mat x_product_2_transpose = x_product_2.t();

    for (i = 0; i < window_size; ++i){
        for (j = 0; j < p; ++j){
            SG_Coefficients(i, j) = x_product_1_transpose(i, j);
        }
    }
    for (i = columns-window_size; i < window_size; ++i){
        for (j = columns - p; j < columns; ++j){
            SG_Coefficients(i, j) = x_product_2_transpose(i, j);
        }
    }

    spectra_ = spectra_ * SG_Coefficients;
    spectra_ /= -1;
}


void SpecMap::SavitzkyGolay(int polynomial_order, int window_size)
{
    polynomial_order = 0;
    window_size = 0;
    QMessageBox::information(0, "Feature not Implemented", "This feature not yet implemented");
}

// MAPPING FUNCTIONS //

///
/// \brief SpecMap::Univariate
/// Creates a univariate image
/// \param min left bound of spectral region of interest
/// \param max right bound of spectral region of interest
/// \param name name of MapData object to be created
/// \param value_method method of determining peak (intensity, derivative, or area)
/// \param gradient_index index of color scheme in master list (GetGradient());
///
void SpecMap::Univariate(int min,
                         int max,
                         QString name,
                         QString value_method,
                         QString integration_method,
                         int gradient_index)
{

    int size = x_.n_elem;
    int i;

    rowvec region;
    colvec results;
    results.set_size(size);
    QString map_type;
    rowvec abcissa;
    mat baselines;
    mat mid_lines;

    if (value_method == "Bandwidth"){
        double maximum, half_maximum, width, region_size;
        double start_value, end_value, slope;
        cout << "set size of baseline matrix" << endl;
        baselines.set_size(size, max-min + 1);
        int max_index, left_index, right_index;
        map_type = "1-Region Univariate (Bandwidth (FWHM))";
        int columns = spectra_.n_cols;
        cout << "set size of abcissa" << endl;
        abcissa.set_size(max-min + 1);
        abcissa = wavelength_.subvec(span(min, max));
        cout << "set size of mid lines matrix" << endl;
        mid_lines.set_size(x_.n_rows, 4);
        for (i = 0; i < size; ++i){

            start_value = spectra_(i, min);
            end_value = spectra_(i, max);
            slope = (end_value - start_value) / (max - min);
            int j;
            for (j = 0; j <= (max - min); ++j)
                baselines(i, j) = j*slope + start_value;


            //find maximum and half-maximum
            region = spectra_(i, span(min, max));
            region_size = region.n_elem;
            maximum = region.max();

            //find index of maximum
            for (j = 0; j < columns; ++j){
                if (maximum == spectra_(i, j) && j >= min && j <= max){
                    max_index = j;
                    break;
                }
            }

            int local_max_index = max_index-min;
            half_maximum = (maximum - baselines(i, local_max_index) / 2.0) +
                    baselines(i, local_max_index);

            //find index of left limit
            for (j = max_index; j > 0; --j){
                if (spectra_(i, j) - half_maximum < 0){
                    left_index = j;
                    break;
                }
            }

            //find index of right limit
            for (j = max_index; j < columns; ++j){
                if (spectra_(i, j) - half_maximum < 0){
                    right_index = j;
                    break;
                }
            }

            //make sure adjacent points on other side of inflection aren't better
            if (fabs(spectra_(i, left_index) - half_maximum) <
                    fabs(spectra_(i, left_index - 1) - half_maximum)){
                --left_index;
            }
            if (fabs(spectra_(i, right_index) - half_maximum) <
                     fabs(spectra_(i, right_index + 1) - half_maximum)){
                ++right_index;
            }

            //record to results.  using fabs because order of wavelength unknown
            width = fabs(wavelength_(right_index) - wavelength_(left_index));
            results(i) = width;
            mid_lines(i, 0) = wavelength_(left_index);
            mid_lines(i, 1) = spectra_(left_index);
            mid_lines(i, 2) = wavelength_(right_index);
            mid_lines(i, 3) = spectra_(right_index);
        }

    }

    else if(value_method == "Area"){
        // Do peak fitting stuff here.
        map_type = "1-Region Univariate (Area)";
        cout << "set size of abcissa" << endl;
        abcissa.set_size(max - min + 1);
        abcissa = wavelength_.subvec(span(min, max));
        if (integration_method == "Riemann Sum"){
            double start_value, end_value, slope;
            cout << "set size of baseline matrix" <<endl;
            baselines.set_size(size, abcissa.n_cols);

            for (i=0; i<size; ++i){
                start_value = spectra_(i, min);
                end_value = spectra_(i, max);
                slope = (end_value - start_value) / (max - min);
                int j;
                for (j = 0; j <= (max - min); ++j)
                    baselines(i, j) = j*slope + start_value;

                region = spectra_(i, span(min, max));
                region -= baselines.row(i);
                results(i) = sum(region);
            }
        }
    }

    else if(value_method == "Derivative"){
        // Do derivative stuff here
        map_type = "1-Region Univariate (Derivative)";
    }

    else{
        // Makes an intensity map
        map_type = "1-Region Univariate (Intensity)";
        if (z_scores_calculated_){
            int elements = spectra_.n_elem;
            rowvec region_temp;
            double peak_height;
            double peak_height_temp;
            mat spectra_temp(spectra_.n_rows, spectra_.n_cols);
            for (i = 0; i < elements; ++i)
                spectra_temp(i) = fabs(spectra_(i));

            for (i = 0; i < size; ++i){
                region = spectra_(i, span(min, max));
                region_temp = spectra_temp(i, span(min, max));
                peak_height_temp = region_temp.max();
                peak_height = region.max();

                // If the maxes aren't equal, then we know the peak is negative
                if (peak_height_temp != peak_height)
                    peak_height = peak_height_temp * -1.0;

                results(i) = peak_height;
            }

        }
        else{
            for (i=0; i < size; ++i){
                region = spectra_(i, span(min, max));
                results(i)=region.max();
            }

        }

    }

    QSharedPointer<MapData> map(new MapData(x_axis_description_,
                                            y_axis_description_,
                                            x_, y_, results,
                                            this, directory_,
                                            this->GetGradient(gradient_index),
                                            maps_.size(),
                                            main_window_));


    map.data()->set_name(name, map_type);

    if(baselines.n_rows !=0){
        map->set_baseline(abcissa, baselines);
    }

    if(mid_lines.n_rows != 0){
        map->set_fwhm(mid_lines);
    }

    this->AddMap(map);
    maps_.last().data()->ShowMapWindow();
}

///
/// \brief SpecMap::BandRatio
/// Creates a band ratio univariate map.
/// \param first_min index of left bound of first region of interest
/// \param first_max index of right bound of first region of interest
/// \param second_min index of left bound of second region of interest
/// \param second_max index of right bound of second region of interest
/// \param name name of the MapData object to be created.  This becomes name of the map to the user
/// \param value_method how the maxima are to be determined (area, derivative, or intensity)
/// \param gradient_index index of gradient in the master list (GetGradient())
///
void SpecMap::BandRatio(int first_min,
                        int first_max,
                        int second_min,
                        int second_max,
                        QString name,
                        QString value_method,
                        QString integration_method,
                        int gradient_index)
{

    QString map_type;

    unsigned int size = x_.n_elem;
    unsigned int i;

    rowvec first_region;
    rowvec second_region;
    colvec results;

    rowvec first_abcissa;
    rowvec second_abcissa;
    mat first_baselines;
    mat second_baselines;

    results.set_size(size);

    if(value_method == "Area"){
        // Do peak fitting stuff here.
        map_type = "2-Region Band Ratio Map (Area)";
        if (integration_method == "Riemann Sum"){
            double first_start_value, first_end_value, second_start_value,
                    second_end_value, first_slope, second_slope, first_sum,
                    second_sum;
            first_abcissa.set_size(first_max - first_min + 1);
            second_abcissa.set_size(second_max - second_min + 1);
            first_abcissa = wavelength_.subvec(span(first_min, first_max));
            second_abcissa = wavelength_.subvec(span(second_min, second_max));\
            first_baselines.set_size(size, first_max - first_min + 1);
            second_baselines.set_size(size, second_max - second_min + 1);

            for (i=0; i<size; ++i){
                first_start_value = spectra_(i, first_min);
                second_start_value = spectra_(i, second_min);
                first_end_value = spectra_(i, first_max);
                second_end_value = spectra_(i, second_max);
                first_slope = (first_end_value - first_start_value) / (first_max - first_min);
                second_slope = (second_end_value - second_start_value) / (second_max - second_min);
                int j;
                for (j = 0; j <= (first_max - first_min); ++j)
                    first_baselines(i, j) = j*first_slope + first_start_value;
                for (j = 0; j <= (second_max - second_min); ++j)
                    second_baselines(i, j) = j*second_slope + second_start_value;

                first_region = spectra_(i, span(first_min, first_max));
                second_region = spectra_(i, span(second_min, second_max));

                first_sum = sum(first_region - first_baselines.row(i));
                second_sum = sum(second_region - second_baselines.row(i));

                results(i)= first_sum / second_sum;
            }

        }


    }

    else if(value_method == "Derivative"){
        // Do derivative stuff here
        map_type = "2-Region Band Ratio Map (Derivative)";
    }

    else{
        // Makes an intensity map
        map_type = "2-Region Band Ratio Map (Intensity)";
        cout << "line 157" <<endl;
        for (i=0; i<size; ++i){
            first_region = spectra_(i, span(first_min, first_max));
            second_region = spectra_(i, span(second_min, second_max));
            results(i)= first_region.max()/second_region.max();
        }
    }

    QSharedPointer<MapData> new_map(new MapData(x_axis_description_,
                                            y_axis_description_,
                                            x_, y_, results,
                                            this, directory_,
                                            this->GetGradient(gradient_index),
                                            maps_.size(),
                                            main_window_));


    new_map.data()->set_name(name, map_type);
    if (first_baselines.n_rows != 0){
        new_map->set_baselines(first_abcissa, second_abcissa, first_baselines, second_baselines);
    }
    this->AddMap(new_map);
    maps_.last().data()->ShowMapWindow();
}



///
/// \brief SpecMap::PrincipalComponents
/// Performs principal component analysis on the data.  Uses armadillo's pca routine.
/// This function both calculates and plots principal components maps
/// \param component the PCA component from which the image will be produced
/// \param include_negative_scores if false, all negative scores are changed to 0
/// \param name the name of the MapData object to be created
/// \param gradient_index the index of the gradient in the master list (in function GetGradient)
///
void SpecMap::PrincipalComponents(int component,
                                  bool include_negative_scores,
                                  QString name,
                                  int gradient_index, bool recalculate)
{
    if (recalculate || !principal_components_calculated_){

        component -= 1;
        cout << "SpecMap::PrincipalComponents" << endl;

        mat coefficients;
        mat score;
        vec latent;
        vec tsquared;

        QMessageBox alert;
        alert.setText("Calculating principal components may take a while.");
        alert.setInformativeText("When complete, the image will appear in a new window. "
                                 "The program may appear not to respond.  Principal "
                                 "components only need to be calculated once per dataset. "
                                 "OK to continue");

        alert.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        alert.setWindowTitle("Principal Components Analysis");
        alert.setIcon(QMessageBox::Question);

        int ret = alert.exec();


        if (ret == QMessageBox::Cancel){
            return;
        }

        if (ret == QMessageBox::Ok){


            cout << "call to arma::princomp" << endl;
            wall_clock timer;
            timer.tic();
            princomp(coefficients, score, latent, tsquared, spectra_);
            int seconds = timer.toc();
            cout << "call to arma::princomp successful. Took " << seconds << " s" << endl;

            principal_components_data_ =
                    new PrincipalComponentsData(this,
                                                directory_,
                                                coefficients,
                                                score,
                                                latent,
                                                tsquared);
            principal_components_calculated_ = true;
        }
    }

    QString map_type;
    QTextStream(&map_type) << "(Principal Component " << component + 1 << ")";

    colvec results = principal_components_data_->Results(component);

    if (!include_negative_scores){
        for (unsigned int i=0; i<results.n_rows; ++i){
            if (results(i) < 0){
                results(i) = 0;
            }
        }
    }

    QSharedPointer<MapData> new_map(new MapData(x_axis_description_,
                                            y_axis_description_,
                                            x_, y_, results,
                                            this, directory_,
                                            this->GetGradient(gradient_index),
                                            maps_.size(),
                                            main_window_));
    new_map.data()->set_name(name, map_type);
    this->AddMap(new_map);
    maps_.last().data()->ShowMapWindow();
}

void SpecMap::VertexComponents(int endmembers, bool include_negative_scores, QString name)
{

}

///
/// \brief SpecMap::PartialLeastSquares
/// Performs PLS regression on data.  Resulting map is score for one PLS Component,
/// taken from one column of the projected data.
/// PLS is performed once.  Subsequent maps use data from first call, stored
/// as PartialLeastSquaresData object, unless
/// \param components the number of components/response variables of the regression data
/// \param include_negative_scores if false, program sets all negative values in the results to 0
/// \param name the name of the MapData object to be created.
/// \param gradient_index the index of the color gradient in the color gradient list
/// \param recalculate whether or not to recalculate PLS regression.
///
void SpecMap::PartialLeastSquares(int components,
                                  bool include_negative_scores,
                                  QString name,
                                  int gradient_index,
                                  bool recalculate)
{
    if (recalculate || !partial_least_squares_calculated_){
        //implementing simpls
        //The only data we need is X loadings, X scores, and Variance data.
        mat x_loadings;
        mat y_loadings;


    }
}


///
/// \brief SpecMap::FindRange.
/// Finds the index of the wavelength value closest
/// to the specified wavelength range.
/// \param start the first wavelength in the spectral region of interest
/// \param end the second wavelength in the spectral region of interest
/// \return
///
vector<int> SpecMap::FindRange(double start, double end)
{
    vector<int> indices(2,0);
    int length = wavelength_.size();
    int i;
    for (i=0; i<length; ++i){
        if(wavelength_(i)>=start){
            break;
        }
    }
    if (i==length-1){
        cerr << "Can't find index of start of range!" << endl;
        return indices; //in this case, indices contains only zeroes
    }

    if (fabs(wavelength_(i)-start)>fabs(wavelength_(i-1)-start)){
        indices[0]=i-1;
    }

    else{
        indices[0]= i;
    }
    int it = i;
    for (i = it; i < length; ++i){
        if(wavelength_(i)>=end){
            break;
        }
    }

    if(i==length-1){
        cerr << "Can't find index of upper wavelength limit!" << endl;
        cerr << "Setting upper limit equal to lower limit (point ROI)" << endl;
        indices[1]=indices[0];
        return indices;
    }

    if (fabs(wavelength_(i)-start)>fabs(wavelength_(i-1)-start)){
        indices[1] = i-1;
    }

    else{
        indices[1] = i;
    }
    return indices;
}

/// HELPER FUNCTIONS (Will go in own file later to speed compilation? ///
/// \brief SpecMap::PointSpectrum
/// \param index
/// \return
///
QVector<double> SpecMap::PointSpectrum(int index)
{
    std::vector<double> spectrum_stdvector =
            conv_to< std::vector<double> >::from(spectra_.row(index));

    QVector<double> spectrum_qvector =
            QVector<double>::fromStdVector(spectrum_stdvector);

    return spectrum_qvector;
}

QVector<double> SpecMap::WavelengthQVector()
{
    std::vector<double> wavelength_stdvector =
            conv_to< std::vector<double> >::from(wavelength_);

    QVector<double> wavelength_qvector =
            QVector<double>::fromStdVector(wavelength_stdvector);

    return wavelength_qvector;
}

///
/// \brief SpecMap::ValueRange
/// Finds the minima and maxima of y variable to properly set axes
///  of QCustomPlot objects
/// \return
///
QCPRange SpecMap::ValueRange()
{
    double lower = y_.min();
    double upper = y_.max();
    QCPRange range(upper, lower);
    return range;
}
///
/// \brief SpecMap::KeyRange
/// Finds the minima and maxima of x variable to properly set axes
///  of QCustomPlot objects
/// \return
///
QCPRange SpecMap::KeyRange()
{
    double lower = x_.min();
    double upper = x_.max();
    QCPRange range(upper, lower);
    return range;
}

///
/// \brief SpecMap::KeySize
/// Finds the number of data points in x variable to properly set axes
///  of QCustomPlot objects
/// \return number of unique x values
///
int SpecMap::KeySize()
{
    unsigned int i;
    int x_count=1;
    double x_buf;

    //loop through x until a value different then the first is met.
    if (!flipped_){
        x_count = 1; //this counts the first entry in x_
        x_buf = x_(0);
        for(i=0; i<x_.n_elem; ++i){
            if(x_(i)!=x_buf){
                ++x_count;
                x_buf=x_(i);
            }
        }
    } else{
        x_count = 0;
        for (i=0; i<x_.n_elem; ++i){
            if(y_(i)!=y_(0)){
                break;
            } else{
                ++x_count;
            }
        }
    }

    return x_count;
}

///
/// \brief SpecMap::ValueSize
/// Finds number of unique y values for properly setting QCPAxis
/// \return number of unique y values
///
int SpecMap::ValueSize()
{

    unsigned int i = 0;
    int y_count;


    //long-text files hold x constant and vary y
    //until x is different, count y
    //reverse if flipped
    if (!flipped_){
        y_count = 0;
        for (i=0; i<x_.n_elem; ++i){
            if(x_(i)!=x_(0)){
                break;
            }
            else{
                ++y_count;
            }
        }
    } else{
        y_count = 1;
        double y_buf = y_(0);
        for(i=0; i<y_.n_elem; ++i){
            if(y_(i)!=y_buf){
                ++y_count;
                y_buf=y_(i);
            }
        }
    }


    return y_count;
}


// MEMBER ACCESS FUNCTIONS
///
/// \brief SpecMap::wavelength
/// \return member wavelength_ (spectrum key values)
///
rowvec SpecMap::wavelength()
{
    return wavelength_;
}

///
/// \brief SpecMap::x
/// \return member x_
///
colvec SpecMap::x()
{
    return x_;
}

///
/// \brief SpecMap::y
/// \return member y_
///
colvec SpecMap::y()
{
    return y_;
}

///
/// \brief SpecMap::spectra
/// \return member spectra_
///
mat SpecMap::spectra()
{
    return spectra_;
}

///
/// \brief SpecMap::name
/// \return member name_, the name of the dataset as seen by the user
///
const QString SpecMap::name()
{
    return name_;
}

///
/// \brief SpecMap::SetName
/// \param new_name new name of dataset
///
void SpecMap::SetName(QString new_name)
{
    name_ = new_name;
}



//MAP HANDLING FUNCTIONS
///
/// \brief SpecMap::map_names
/// \return list of names of the maps created from this dataset
///
QStringList SpecMap::map_names()
{
    return map_names_;
}

///
/// \brief SpecMap::map_loading_count
/// \return number of maps created for this dataset
///
int SpecMap::map_loading_count()
{
    return map_loading_count_;
}

///
/// \brief SpecMap::RemoveMapAt
/// \param i index of map in the relevant lists
///
void SpecMap::RemoveMapAt(int i)
{

    QListWidgetItem *item = map_list_widget_->takeItem(i);
    map_list_widget_->removeItemWidget(item);
    maps_.removeAt(i); //map falls out of scope and memory freed!

}


///
/// \brief SpecMap::RemoveMap
/// Removes a map by its name.
/// Useful when only the name is easily known, and index won't be used.
/// Probably useless, might be removed later.
/// \param name
///
void SpecMap::RemoveMap(QString name)
{
    int i;
    for(i=0; i<map_names_.size(); ++i){
        if(map_names_.at(i)==name){
            this->RemoveMapAt(i);
        }
    }
}

///
/// \brief SpecMap::AddMap
/// Adds a map to the list of map pointers and adds its name to relevant lists
/// \param map
///
void SpecMap::AddMap(QSharedPointer<MapData> map)
{
    QString name = map->name();
    maps_.append(map);
    map_names_.append(name);

    map_list_widget_->addItem(name);
    ++map_loading_count_;
}

///
/// \brief SpecMap::WavelengthRange
/// \return the range of the wavlength vector (for plotting point spectra)
///
QCPRange SpecMap::WavelengthRange()
{
    double min = wavelength_.min();
    double max = wavelength_.max();
    QCPRange range(min, max);
    return range;
}

///
/// \brief SpecMap::PointSpectrumRange
/// \param i row of spectra_ containing desired spectrum
/// \return the range of y values for the point spectra at i
///
QCPRange SpecMap::PointSpectrumRange(int i)
{
    rowvec row = spectra_.row(i);
    double min = row.min();
    double max = row.max();

    QCPRange range(min, max);
    return range;
}

///
/// \brief SpecMap::GetGradient
/// Selects the color gradient from list of presets
/// \param gradient_number
/// \return
///
QCPColorGradient SpecMap::GetGradient(int gradient_number)
{
    switch (gradient_number)
    {
    case 0: return QCPColorGradient::cbBuGn;
    case 1: return QCPColorGradient::cbBuPu;
    case 2: return QCPColorGradient::cbGnBu;
    case 3: return QCPColorGradient::cbOrRd;
    case 4: return QCPColorGradient::cbPuBu;
    case 5: return QCPColorGradient::cbPuBuGn;
    case 6: return QCPColorGradient::cbPuRd;
    case 7: return QCPColorGradient::cbRdPu;
    case 8: return QCPColorGradient::cbYlGn;
    case 9: return QCPColorGradient::cbYlGnBu;
    case 10: return QCPColorGradient::cbYlOrBr;
    case 11: return QCPColorGradient::cbYlOrRd;
    case 12: return QCPColorGradient::cbBlues;
    case 13: return QCPColorGradient::cbGreens;
    case 14: return QCPColorGradient::cbOranges;
    case 15: return QCPColorGradient::cbPurples;
    case 16: return QCPColorGradient::cbReds;
    case 17: return QCPColorGradient::cbGreys;
    case 18: return QCPColorGradient::gpGrayscale;
    case 19: return QCPColorGradient::gpNight;
    case 20: return QCPColorGradient::gpCandy;
    case 21: return QCPColorGradient::gpIon;
    case 22: return QCPColorGradient::gpThermal;
    case 23: return QCPColorGradient::gpPolar;
    case 24: return QCPColorGradient::gpSpectrum;
    case 25: return QCPColorGradient::gpJet;
    case 26: return QCPColorGradient::gpHues;
    case 27: return QCPColorGradient::gpHot;
    case 28: return QCPColorGradient::gpCold;
    case 29: return QCPColorGradient::cbBrBG;
    case 30: return QCPColorGradient::cbPiYG;
    case 31: return QCPColorGradient::cbPRGn;
    case 32: return QCPColorGradient::cbPuOr;
    case 33: return QCPColorGradient::cbRdBu;
    case 34: return QCPColorGradient::cbRdGy;
    case 35: return QCPColorGradient::cbRdYlBu;
    case 36: return QCPColorGradient::cbRdYlGn;
    case 37: return QCPColorGradient::cbSpectral;
    case 38: return QCPColorGradient::vSpectral;
    case 39: return QCPColorGradient::cbCluster;
    default: return QCPColorGradient::gpCold;
    }
}

bool SpecMap::ConstructorCancelled()
{
    return constructor_canceled_;
}

mat SpecMap::AverageSpectrum(bool stats)
{
    mat spectrum;
    int size = spectra_.n_cols;
    double spec_mean;
    double spec_std;

    if (stats)
        spectrum.set_size(2, size);
    else
        spectrum.set_size(1, size);

    for (int j = 0; j < size; ++j){
        spec_mean = mean(spectra_.col(j));
        spectrum(0, j) = spec_mean;
        if (stats){
            spec_std = stddev(spectra_.col(j));
            spectrum(1, j) = spec_std;
        }
    }

    return spectrum;
}

bool SpecMap::principal_components_calculated()
{
    return principal_components_calculated_;
}

const QString SpecMap::x_axis_description()
{
    return x_axis_description_;
}

void SpecMap::SetXDescription(QString description)
{
    x_axis_description_ = description;
}

void SpecMap::SetYDescription(QString description)
{
    y_axis_description_ = description;
}

const QString SpecMap::y_axis_description()
{
    return y_axis_description_;
}


bool SpecMap::vertex_components_calculated()
{
    return vertex_components_calculated_;
}

bool SpecMap::partial_least_squares_calculated()
{
    return partial_least_squares_calculated_;
}

PrincipalComponentsData* SpecMap::principal_components_data()
{
    return principal_components_data_;
}

mat* SpecMap::spectra_ptr()
{
    return &spectra_;
}

mat* SpecMap::wavelength_ptr()
{
    return &wavelength_;
}

mat* SpecMap::x_ptr()
{
    return &x_;
}

mat* SpecMap::y_ptr()
{
    return &y_;
}
