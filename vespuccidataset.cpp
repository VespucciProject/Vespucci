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
#include "vespuccidataset.h" //VespucciDataset includes all necessary headers.
#include <mlpack/methods/kmeans/kmeans.hpp>
#include <mlpack/methods/quic_svd/quic_svd.hpp>


using namespace arma;
using namespace std;


///
/// \brief VespucciDataset::~VespucciDataset
/// Destructor deletes everything allocated with new that isn't a smart pointer
VespucciDataset::~VespucciDataset()
{
    cout << "VespucciDataset Destructor" << endl;
    //make sure principal components stats are deleted properly.
    if (principal_components_calculated_)
        delete principal_components_data_;
    if (partial_least_squares_calculated_)
        delete partial_least_squares_data_;

    //make sure all maps are delted properly.
    map_list_model_->ClearMaps();

    DestroyLogFile();
    delete map_list_model_;

}

bool VespucciDataset::Save(QString filename)
{
    bool success;
    try{
        field<mat> dataset(4);
        dataset(0) = spectra_;
        dataset(1) = wavelength_;
        dataset(2) = x_;
        dataset(3) = y_;
        success = dataset.save(filename.toStdString(), arma_binary);
    }
    catch(exception e){
        cerr << "See armadillo exception" << endl;

        char str[50];
        strcat(str, "VespucciDataset::Save: ");
        strcat(str, e.what());
        throw std::runtime_error(str);
    }

    return success;

}

///
/// \brief VespucciDataset::DestroyLogFile
/// Deletes log file unless user decides to save it elsewhere
void VespucciDataset::DestroyLogFile()
{

    QMessageBox::StandardButton reply =
            QMessageBox::question(main_window_,
                                  "Save log?",
                                  "Would you like to save the log for " + name_
                                  + "?",
                                  QMessageBox::Yes|QMessageBox::No);
    QString filename;


    if (reply == QMessageBox::No){
        log_file_->remove();
        return;
    }
    else{
        filename = QFileDialog::getSaveFileName(0, QTranslator::tr("Save File"),
                                   *directory_,
                                   QTranslator::tr("Text Files (*.txt)"));        
        bool success = log_file_->copy(filename);
        if (!success){
            bool remove_success = QFile::remove(filename); //delete old file
            if (!remove_success){
                QMessageBox::warning(main_window_, "Failure", "Previous file could not be removed");
            }
            success = log_file_->copy(filename);
        }
        //new log file falls out of scope
        log_file_->remove();

        if (success)
            QMessageBox::information(main_window_, "Success!", "File " + filename + " written successfully");
        else
            QMessageBox::warning(main_window_, "Failure", "File not written successfully.");
    }
    return;
}

///
/// \brief VespucciDataset::VespucciDataset
/// \param binary_filename The filename of the binary input file
/// \param main_window The main window of the program
/// \param directory The current working directory
/// \param name The name of the dataset displayed to the user
/// \param log_file The log file
/// Constructor for loading saved spectral/spatial data in armadillo format
VespucciDataset::VespucciDataset(QString vespucci_binary_filename,
                                 MainWindow *main_window,
                                 QString *directory,
                                 QFile *log_file,
                                 QString name,
                                 QString x_axis_description,
                                 QString y_axis_description)
    : log_stream_(log_file)
{
    QDateTime datetime = QDateTime::currentDateTimeUtc();
    log_file_ = log_file;
    log_stream_ << "Vespucci, a free, cross-platform tool for spectroscopic imaging" << endl;
    log_stream_ << "Version 0.4" << endl << endl;
    log_stream_ << "Dataset " << name << "created "
                << datetime.date().toString("yyyy-MM-dd") << "T"
                << datetime.time().toString("hh:mm:ss") << "Z" << endl;
    log_stream_ << "Imported from binary file " << vespucci_binary_filename << endl << endl;

    non_spatial_ = false;
    meta_ = false;
    //Set up variables unrelated to hyperspectral data:
    map_list_model_ = new MapListModel(main_window, this);

    map_loading_count_ = 0;
    principal_components_calculated_ = false;
    partial_least_squares_calculated_ = false;
    vertex_components_calculated_ = false;
    k_means_calculated_ = false;
    z_scores_calculated_ = false;
    directory_ = directory;
    main_window_ = main_window;
    x_axis_description_ = x_axis_description;
    y_axis_description_ = y_axis_description;
    name_ = name;
    vec indices_temp;
    try{
        BinaryImport::ImportVespucciBinary(vespucci_binary_filename,
                                           spectra_,
                                           wavelength_,
                                           x_, y_);
        indices_.set_size(x_.n_elem);
        for (uword i = 0; i < indices_.n_elem; ++i)
            indices_(i) = i;

    }
    catch(exception e) {
        char str[50];
        strcat(str, "BinaryImport: ");
        strcat(str, e.what());
        throw std::runtime_error(str);

    }

}




///
/// \brief VespucciDataset::VespucciDataset
/// \param text_filename The filename of the text file from which data is imported
/// \param main_window The main window of the program
/// \param directory The current global working directory
/// \param log_file The log file
/// \param name The name of the dataset displayed to the user
/// \param x_axis_description A description of the spectral abscissa
/// \param y_axis_description A description of the spectral ordinate
/// \param swap_spatial Whether or not y is located in the first column instead of the second (some Horiba spectrometers do this).
/// Main function for processing data from text files to create VespucciDataset objects.
/// Currently written to accept files in "wide" format, will be expanded to deal
/// with different ASCII formats later with conditionals.
VespucciDataset::VespucciDataset(QString text_filename,
                                 MainWindow *main_window,
                                 QString *directory,
                                 QFile *log_file,
                                 QString name,
                                 QString x_axis_description,
                                 QString y_axis_description,
                                 bool swap_spatial,
                                 InputFileFormat::Format format)
    : log_stream_(log_file)
{
    QDateTime datetime = QDateTime::currentDateTimeUtc();
    log_file_ = log_file;

    log_stream_ << "Vespucci, a free, cross-platform tool for spectroscopic imaging" << endl;
    log_stream_ << "Version 0.4" << endl << endl;
    log_stream_ << "Dataset " << name << " created "
                << datetime.date().toString("yyyy-MM-dd") << "T"
                << datetime.time().toString("hh:mm:ss") << "Z" << endl;
    log_stream_ << "Imported from text file " << text_filename << endl << endl;


    non_spatial_ = false;
    meta_ = false;
    //Set up variables unrelated to hyperspectral data:
    map_list_model_ = new MapListModel(main_window, this);
    map_loading_count_ = 0;
    principal_components_calculated_ = false;
    partial_least_squares_calculated_ = false;
    vertex_components_calculated_ = false;
    k_means_calculated_ = false;
    z_scores_calculated_ = false;
    directory_ = directory;
    flipped_ = swap_spatial;

    QProgressDialog progress("Loading Dataset...", "Cancel", 0, 100, NULL);
    vec indices_temp;


    switch (format){
    case InputFileFormat::WideTabDel :
        try{
            constructor_canceled_ = TextImport::ImportWideText(text_filename,
                                                                   spectra_,
                                                                   wavelength_,
                                                                   x_, y_,
                                                                   swap_spatial,
                                                                   &progress,
                                                                   "\t");
            indices_.set_size(x_.n_elem);
            for (uword i = 0; i < indices_.n_elem; ++i)
                indices_(i) = i;

        }
        catch(exception e){
            char str[50];
            strcat(str, "Text import constructor: ");
            strcat(str, e.what());
            throw std::runtime_error(str);
        }
        break;
    case InputFileFormat::WideCSV :
        try{
            constructor_canceled_ = TextImport::ImportWideText(text_filename,
                                                               spectra_,
                                                               wavelength_,
                                                               x_, y_,
                                                               swap_spatial,
                                                               &progress,
                                                               ",");
            indices_.set_size(x_.n_elem);
            for (uword i = 0; i < indices_.n_elem; ++i)
                indices_(i) = i;

        }
        catch(exception e){
            char str[50];
            strcat(str, "Text import constructor: ");
            strcat(str, e.what());
            throw std::runtime_error(str);
        }
        break;
    case InputFileFormat::LongTabDel : case InputFileFormat::LongCSV :
        try{
            constructor_canceled_ = TextImport::ImportLongText(text_filename,
                                                               spectra_,
                                                               wavelength_,
                                                               x_, y_,
                                                               swap_spatial,
                                                               &progress);
            indices_.set_size(x_.n_elem);
            for (uword i = 0; i < indices_.n_elem; ++i)
                indices_(i) = i;
        }
        catch(exception e){
            char str[50];
            strcat(str, "Text import constructor: ");
            strcat(str, e.what());
            throw std::runtime_error(str);
        }
        break;

    default :
        throw std::runtime_error("Invalid file format for text import constructor");
        break;

    }//end of switch statement

    constructor_canceled_ = false;
    name_ = name;
    x_axis_description_ = x_axis_description;
    y_axis_description_ = y_axis_description;
    main_window_ = main_window;
}

///
/// \brief VespucciDataset::VespucciDataset
/// \param name The name of the dataset displayed to the user
/// \param main_window The main window of the program
/// \param directory The current global working directory
/// \param log_file The log file
/// \param original The dataset from which this dataset is "extracted"
/// \param indices The indices of the spectra in the previous dataset that will form this one
/// This is a constructor to create a new dataset by "extracting" spectra from a
/// another dataset based on values on an image.
VespucciDataset::VespucciDataset(QString name,
                                 MainWindow *main_window,
                                 QString *directory,
                                 QFile *log_file,
                                 QSharedPointer<VespucciDataset> original,
                                 uvec indices)
    : log_stream_(log_file)

{
    log_file_ = log_file;
    map_list_model_ = new MapListModel(main_window, this);
    QDateTime datetime = QDateTime::currentDateTimeUtc();
    log_stream_ << "Dataset " << name << "created "
                << datetime.date().toString("yyyy-MM-dd") << "T"
                << datetime.time().toString("hh:mm:ss") << "Z" << endl;
    log_stream_ << "Created from previous dataset " << original->name() << endl;

    non_spatial_ = true;
    meta_ = original->meta();
    map_loading_count_ = 0;
    principal_components_calculated_ = false;
    partial_least_squares_calculated_ = false;
    vertex_components_calculated_ = false;
    k_means_calculated_ = false;
    z_scores_calculated_ = false;
    directory_ = directory;
    vec parent_indices;

    try{
        spectra_ = original->spectra(indices);
        wavelength_ = original->wavelength();
        x_ = original->x(indices);
        y_ = original->y(indices);
        parent_indices = original->indices();
        indices_ = parent_indices(indices);
    }
    catch(exception e){
        char str[50];
        strcat(str, "Extraction constructor: ");
        strcat(str, e.what());
        throw std::runtime_error(str);
    }

    name_ = name;
    main_window_ = main_window;
    directory_ = directory;
}

///
/// \brief VespucciDataset::VespucciDataset
/// \param name Dataset name
/// \param main_window The
/// \param directory
/// \param log_file
/// Constructor to create a dataset without spatial and spectral data set (i.e.
/// when using MetaDataset).
VespucciDataset::VespucciDataset(QString name,
                                 MainWindow *main_window,
                                 QString *directory,
                                 QFile *log_file)
    : log_stream_(log_file)
{
    map_list_model_ = new MapListModel(main_window, this);
    log_file_ = log_file;
    non_spatial_ = true;
    meta_ = true;
    map_loading_count_ = 0;
    principal_components_calculated_ = false;
    partial_least_squares_calculated_ = false;
    vertex_components_calculated_ = false;
    k_means_calculated_ = false;
    z_scores_calculated_ = false;
    directory_ = directory;
    name_ = name;
    main_window_ = main_window;
    directory_ = directory;
}


// PRE-PROCESSING FUNCTIONS //
///
/// \brief VespucciDataset::Undo Swap spectra_ and spectra_old_ to undo an action.
/// Calling this function again re-does the action that was undid.
///
void VespucciDataset::Undo()
{

    try{
        mat buffer = spectra_;
        spectra_ = spectra_old_;
        spectra_old_ = buffer;
    }
    catch(exception e){
        char str[50];
        strcat(str, "Undo: ");
        strcat(str, e.what());
        throw std::runtime_error(str);
    }

    log_stream_ << "Undo: " << last_operation_ << endl << endl;
    last_operation_ = "Undo";
}


///
/// \brief VespucciDataset::CropSpectra
/// Crops spectra_ based on
/// \param x_min value of x below which spectra are deleted
/// \param x_max value of x above which spectra are deleted
/// \param y_min value of y below which spectra are deleted
/// \param y_max value of y above which spectra are deleted
/// Removes all data points outside of the range. Cannot be undone. It is preferable
/// to create a new dataset rather than crop an old one.
void VespucciDataset::CropSpectra(double x_min, double x_max,
                                  double y_min, double y_max,
                                  double wl_min, double wl_max)
{
    spectra_old_ = spectra_;
    uvec valid_indices = find ((x_ > x_min) && (x_ < x_max));
    try{
        spectra_ = spectra_.rows(valid_indices);
        y_ = y_.rows(valid_indices);
        x_ = x_.rows(valid_indices);
        valid_indices = find((y_ > y_min) && (y_ < y_max));
        y_ = y_.rows(valid_indices);
        x_ = x_.rows(valid_indices);
        spectra_ = spectra_.rows(valid_indices);
    }catch(exception e){
        char str[50];
        strcat(str, "CropSpectra: ");
        strcat(str, e.what());
        throw std::runtime_error(str);
    }
    valid_indices = find(wavelength_ > wl_min && wavelength_ < wl_max);
    try{
        spectra_ = spectra_.cols(valid_indices);
        wavelength_ = wavelength_.cols(valid_indices);

    }catch(exception e){
        char str[50];
        strcat(str, "CropSpectra: ");
        strcat(str, e.what());
        throw std::runtime_error(str);
    }


    last_operation_ = "crop";
    log_stream_ << "CropSpectra" << endl;
    log_stream_ << "x_min == " << x_min << endl;
    log_stream_ << "x_max == " << x_max << endl;
    log_stream_ << "y_min == " << y_min << endl;
    log_stream_ << "y_max == " << y_max << endl;
    log_stream_ << "wl_min == " << wl_min << endl;
    log_stream_ << "wl_max == " << wl_max << endl << endl;
}


///
/// \brief VespucciDataset::MinMaxNormalize
///normalizes data so that smallest value is 0 and highest is 1 through the
/// entire spectra_ matrix.  If the minimum of spectra_ is negative, it subtracts
/// this minimum from all points.  The entire spectra_ matrix is then divided
/// by the maximum of spectra_
void VespucciDataset::MinMaxNormalize()
{
    try{
        spectra_old_ = spectra_;
        int n_elem = spectra_.n_elem;
        double minimum = spectra_.min();
        if (minimum < 0)
            for (int i = 0; i < n_elem; ++i)
                spectra_(i) = spectra_(i) - minimum;
        double maximum = spectra_.max();
        spectra_ = spectra_/maximum;
    }
    catch(exception e){
        char str[50];
        strcat(str, "MinMaxNormalize: ");
        strcat(str, e.what());
        throw std::runtime_error(str);
    }

    last_operation_ = "min/max normalize";
    log_stream_ << "MinMaxNormalize" << endl << endl;

}

///
/// \brief VespucciDataset::UnitAreaNormalize
///normalizes the spectral data so that the area under each point spectrum is 1
void VespucciDataset::UnitAreaNormalize()
{
    log_stream_ << "UnitAreaNormalize" << endl << endl;
    spectra_old_ = spectra_;
    uword num_rows = spectra_.n_rows;
    uword num_cols = spectra_.n_cols;
    try{
        for (uword i = 0; i < num_rows; ++i){
            rowvec row = spectra_.row(i);
            double row_sum = sum(row);
            for (uword j = 0; j < num_cols; ++j){
                spectra_(i, j) = spectra_(i, j) / row_sum;
            }
        }
    }
    catch(exception e){
        char str[50];
        strcat(str, "UnitAreaNormalize: ");
        strcat(str, e.what());
        throw std::runtime_error(str);
    }

    last_operation_ = "unit area normalize";
}

///
/// \brief VespucciDataset::PeakIntensityNormalize
/// \param left_bound
/// \param right_bound
///
void VespucciDataset::PeakIntensityNormalize(double left_bound, double right_bound)
{
    spectra_old_ = spectra_;
    vec positions;
    vec peak_maxes = arma_ext::FindPeakMaxMat(trans(spectra_), trans(wavelength_), left_bound, right_bound, positions);
    for (uword i = 0; i < spectra_.n_rows; ++i){
        spectra_.row(i) /= peak_maxes(i);
    }
    last_operation_ = "Peak intensity normalize";
}

///
/// \brief VespucciDataset::ZScoreNormCopy
/// For when you want to Z-score normalize without changing spectra_
/// \return A normalized copy of the matrix.
///
mat VespucciDataset::ZScoreNormCopy()
{

    mat normalized;
    //mat normalized_copy(num_rows, num_cols);
    try{
        normalized = arma_ext::StandardScore(trans(spectra_));
    }
    catch(exception e){
        char str[50];
        strcat(str, "ZScoreNormCopy: ");
        strcat(str, e.what());
        throw std::runtime_error(str);
    }
    return trans(normalized);

}

///
/// \brief VespucciDataset::ZScoreNormalize
/// Normalizes each row using the score of the normal distribution
///
void VespucciDataset::ZScoreNormalize()
{
    log_stream_ << "ZScoreNormalize" << endl;
    spectra_old_ = spectra_;

    mat normalized;
    try{
        normalized = arma_ext::StandardScoreMat(trans(spectra_));
        spectra_ = trans(normalized);
    }
    catch(exception e){
        char str[50];
        strcat(str, "ZScoreNormalize: ");
        strcat(str, e.what());
        throw std::runtime_error(str);
    }

    z_scores_calculated_ = true;
    last_operation_ = "Z-score normalize";

}

void VespucciDataset::AbsoluteValue()
{
    log_stream_ << "AbsoluteValue" << endl;
    spectra_old_ = spectra_;

    try{
        spectra_ = arma::abs(spectra_);
    }
    catch(exception e){
        char str[50];
        strcat(str, "AbsoluteValue: ");
        strcat(str, e.what());
        throw std::runtime_error(str);
    }

    last_operation_ = "Absolute value";

}


///
/// \brief VespucciDataset::SubtractBackground
/// Subtracts a known background spectrum. This can be extracted from a control
/// map using this software (using * component analysis endmember extraction or
/// average spectrum).
/// \param background A matrix consisting of a single spectrum representing the
/// background.
///
void VespucciDataset::SubtractBackground(mat background, QString filename)
{
    log_stream_ << "SubtractBackground" << endl;
    log_stream_ << "filename == " << filename << endl << endl;
    spectra_old_ = spectra_;
    if (background.n_cols != spectra_.n_cols){
        QMessageBox::warning(0,
                             "Improper Dimensions!",
                             "The background spectrum has a different number of"
                             " points than the map data."
                             " No subtraction can be performed");
        return;
    }
    else{
        try{
            spectra_.each_row() -= background.row(0);
        }
        catch(exception e){
            char str[50];
            strcat(str, "SubtractBackground: ");
            strcat(str, e.what());
            throw std::runtime_error(str);
        }
    }
    last_operation_ = "background correction";
}

///
/// \brief VespucciDataset::Baseline
/// Baseline-adjusts the data. This function uses a median filter with a large
/// window to determine the baseline on the assumption that the median value
/// is more likely to be basline than spectrum. This will complicate things if
/// there are many peaks. Additionally, this significantly narrows the shape of
/// peaks. Other baseline methods may be implemented later.
/// \param method
/// \param window_size
///
void VespucciDataset::Baseline(QString method, int window_size)
{
    log_stream_ << "Baseline" << endl;
    log_stream_ << "method == " << method << endl;
    log_stream_ << "window_size == " << window_size << endl << endl;
    spectra_old_ = spectra_;
    try{
        if (method == "Median Filter"){
            mat processed = arma_ext::MedianFilterMat(trans(spectra_), window_size);
            spectra_ -= trans(processed);
        }
    }
    catch(exception e){
        char str[50];
        strcat(str, "Baseline: ");
        strcat(str, e.what());
        throw std::runtime_error(str);
    }

    last_operation_ = "baseline correction";
}

///
/// \brief VespucciDataset::RemoveClippedSpectra
/// \param threshold
/// Removes spectra with a maximum value at or above the threshold
void VespucciDataset::RemoveClippedSpectra(double threshold)
{
    spectra_old_ = spectra_;
    vec spectra_max = max(spectra_, 1);
    uvec valid_indices = find(spectra_max < threshold);
    if (valid_indices.n_elem == 0)
        return;

    try{
        spectra_ = spectra_.rows(valid_indices);
        x_ = x_.rows(valid_indices);
        y_ = y_.rows(valid_indices);
    }
    catch(exception e){
        char str[50];
        strcat(str, "RemoveClipped Spectra: ");
        strcat(str, e.what());
        throw std::runtime_error(str);
    }

    if (spectra_.n_rows != spectra_old_.n_rows)
        non_spatial_ = true;
}

//Filtering functions
///
/// \brief VespucciDataset::MedianFilter
/// performs median filtering on the spectral data.  Entries near the boundaries
/// of spectra are not processed. See also VespucciDataset::LinearMovingAverage
/// \param window_size - an odd number representing the width of the window.

void VespucciDataset::MedianFilter(unsigned int window_size)
{
    log_stream_ << "MedianFilter" << endl;
    log_stream_ << "window_size == " << window_size << endl << endl;
    mat processed;
    spectra_old_ = spectra_;
    try{
        processed = arma_ext::MedianFilterMat(trans(spectra_), window_size);
        spectra_ = trans(processed);
    }
    catch(exception e){
        char str[50];
        strcat(str, "MedianFilter: ");
        strcat(str, e.what());
        throw std::runtime_error(str);
    }

    last_operation_ = "median filter";
}

///
/// \brief VespucciDataset::LinearMovingAverage
/// Performs moving average filtering on the spectral data.  Entries near the
/// boundaries of spectra are not processed.  See also VespucciDataset::MedianFilter.
/// \param window_size - an odd number representing the width of the window.

void VespucciDataset::LinearMovingAverage(unsigned int window_size)
{
    log_stream_ << "LinearMovingAverage" << endl;
    log_stream_ << "window_size == " << window_size << endl << endl;

    spectra_old_ = spectra_;
    try{
        vec filter = arma_ext::CreateMovingAverageFilter(window_size);
        //because armadillo is column-major, it is faster to filter by columns than rows
        mat buffer = trans(spectra_);
        mat filtered(buffer.n_rows, buffer.n_cols);
        for (uword j = 0; j < buffer.n_cols; ++j){
            filtered.col(j) = arma_ext::ApplyFilter(buffer.col(j), filter);
        }
        spectra_ = trans(filtered);
    }
    catch(exception e){
        char str[50];
        strcat(str, "LinearMovingAverage: ");
        strcat(str, e.what());
        throw std::runtime_error(str);
    }

    last_operation_ = "moving average filter";
}

///
/// \brief VespucciDataset::SingularValue
/// Denoises the spectra matrix using a truncated singular value decomposition.
/// The first singular_values singular values are used to "reconstruct" the
/// spectra matrix. The function used to find the truncated SVD is
/// arma_ext::svds.
/// \param singular_values Number of singular values to use.
///
void VespucciDataset::SingularValue(unsigned int singular_values)
{
    log_stream_ << "SingularValue" << endl;
    log_stream_ << "singular_values == " << singular_values << endl << endl;
    spectra_old_ = spectra_;
    mat U;
    vec s;
    mat V;
    try{
        arma_ext::svds(spectra_, singular_values, U, s, V);
        spectra_ = -1 * U * diagmat(s) * V.t();
    }
    catch(exception e){
        char str[50];
        strcat(str, "SingularValue: ");
        strcat(str, e.what());
        throw std::runtime_error(str);
    }

    last_operation_ = "truncated SVD de-noise";
}


///
/// \brief VespucciDataset::QUIC_SVD
/// \param epsilon Error tolerance fraction for calculated subspace
/// Use the QUIC-SVD algorithm to denoise the spectra by finding a lower-rank approximation
/// of the matrix.
void VespucciDataset::QUIC_SVD(double epsilon)
{
    log_stream_ << "QUIC_SVD" << endl;
    log_stream_ << "epsilon == " << epsilon << endl << endl;
    mat u, sigma, v, copy;
    cout << "Call QUIC_SVD" << endl;
    mlpack::svd::QUIC_SVD svd_obj(spectra_, u, v, sigma, epsilon, 0.1);
    cout << "create copy" << endl;
    copy = u * sigma * v.t();
    last_operation_ = "truncated SVD de-noise";
    cout << "Copy operations" << endl;
    spectra_old_ = spectra_;
    spectra_ = copy;

}

///
/// \brief VespucciDataset::SavitzkyGolay
/// Performs derivatization/Savitzky-Golay smoothing
/// \param derivative_order The order of the derivative.
/// \param polynomial_order The order of the polynomial
/// \param window_size The size of the filter window.
///
void VespucciDataset::SavitzkyGolay(unsigned int derivative_order,
                                 unsigned int polynomial_order,
                                 unsigned int window_size)
{
    log_stream_ << "SavitzkyGolay" << endl;
    log_stream_ << "derivative_order == " << derivative_order << endl;
    log_stream_ << "polynomial_order == " << polynomial_order << endl;
    log_stream_ << "window_size == " << window_size << endl << endl;
    spectra_old_ = spectra_;
    try{
        mat temp = arma_ext::sgolayfilt(trans(spectra_),
                                        polynomial_order,
                                        window_size,
                                        derivative_order,
                                        1);
        spectra_ = trans(temp);
    }
    catch(exception e){
        char str[50];
        strcat(str, "SavitzkyGolay: ");
        strcat(str, e.what());
        throw std::runtime_error(str);
    }

    last_operation_ = "Savitzky-Golay filtering";
}

void VespucciDataset::Scale(double scaling_factor)
{
    log_stream_ << "Scale" << endl;
    log_stream_ << "scaling_factor = " << scaling_factor << endl;
    spectra_old_ = spectra_;
    try{
        spectra_ = spectra_ * scaling_factor;
    }catch(exception e){
     char str[50];
     strcat(str, "Scale(): ");
     strcat(str, e.what());
     throw std::runtime_error(str);
    }

    last_operation_ = "Scaling";
}

///
/// \brief VespucciDataset::HySime
/// \return Dimensionality predicted by HySime algorithm
///
int VespucciDataset::HySime()
{
    mat noise, noise_correlation, subspace;
    arma_ext::EstimateAdditiveNoise(noise, noise_correlation, trans(spectra_));
    int k = arma_ext::HySime(trans(spectra_), noise, noise_correlation, subspace);
    return k;
}

// MAPPING FUNCTIONS //

///
/// \brief VespucciDataset::Univariate
/// Creates a univariate image. Several peak-determination methods are availible.
/// All methods except for "Intensity" estimate a local baseline. This is done
/// by drawing a straight line from the left to right endpoint. This can cause
/// problems when the endpoints are near other local maxima.
///
/// The "Bandwidth" method calculates the full-width at half maximum of the peak
/// near the range specified.
///
/// The "Intensity" method calculates the local maximum of the spectrum within
/// the range specified.
///
/// The "Area" method takes a right Riemann sum of the spectrum
///
/// The "Derivative" method is misleadingly named (this is based on in-house
/// MATLAB code previously used by my group). The derivative method is actually
/// and area method which finds the edges of the peak by taking a second derivative.
/// It then determines the peak in an identical fashion to the "Area" method///
///
/// \param min left bound of spectral region of interest
/// \param max right bound of spectral region of interest
/// \param name name of MapData object to be created
/// \param value_method method of determining peak (intensity, derivative, or area)
/// \param gradient_index index of color scheme in master list (GetGradient());
///
void VespucciDataset::Univariate(double min,
                                 double max,
                                 QString name,
                                 UnivariateMethod::Method method,
                                 QString integration_method,
                                 uword gradient_index)
{
    //if dataset is non spatial, just quit
    if(non_spatial_){
        QMessageBox::warning(0, "Non-spatial dataset", "Dataset is non-spatial or non-contiguous! Mapping functions are not available");
        return;
    }
    QSharedPointer<UnivariateData> univariate_data(new UnivariateData(QSharedPointer<VespucciDataset>(this), name));
    univariate_data->Apply(min, max, method);
    QSharedPointer<MapData> new_map(new MapData(x_axis_description_,
                                            y_axis_description_,
                                            x_, y_, univariate_data->results(),
                                            QSharedPointer<VespucciDataset>(this),
                                            directory_,
                                            this->GetGradient(gradient_index),
                                            map_list_model_->rowCount(QModelIndex()),
                                            6,
                                            main_window_));
    cout << "set name" << endl;
    new_map->set_name(name, univariate_data->MethodDescription());

    uvec boundaries;
    if(method == UnivariateMethod::Area || method == UnivariateMethod::FWHM){
        boundaries = univariate_data->Boundaries();
        new_map->set_baseline(wavelength_.subvec(boundaries(0), boundaries(1)),
                              univariate_data->first_baselines());
    }


    /*if(method == UnivariateMethod::FWHM){
        boundaries = univariate_data->Boundaries();
        new_map->set_fwhm(univariate_data->Midlines());
    }*/


    log_stream_ << "Univariate" << endl;
    log_stream_ << "min == " << min << endl;
    log_stream_ << "max == " << max << endl;
    log_stream_ << "name == " << name << endl;
    log_stream_ << "method == " << (method == UnivariateMethod::Area ? "Area" : (method == UnivariateMethod::FWHM ? "Bandwidth" : "Intensity")) << endl;
    log_stream_ << "integration_method == " << integration_method << endl;
    log_stream_ << "gradient_index == " << gradient_index << endl;
    univariate_datas_.append(univariate_data);
    map_list_model_->AddMap(new_map);
    new_map->ShowMapWindow();
}


void VespucciDataset::Univariate(double min,
                                 double max,
                                 QString name,
                                 UnivariateMethod::Method method,
                                 QString integration_method)
{

    QSharedPointer<UnivariateData> univariate_data(new UnivariateData(QSharedPointer<VespucciDataset>(this), name));
    univariate_data->Apply(min, max, method);


    log_stream_ << "Univariate (no image)" << endl;
    log_stream_ << "min == " << min << endl;
    log_stream_ << "max == " << max << endl;
    log_stream_ << "name == " << name << endl;
    log_stream_ << "method == " << (method == UnivariateMethod::Area ? "Area" : (method == UnivariateMethod::FWHM ? "Bandwidth" : "Intensity")) << endl;
    log_stream_ << "integration_method == " << integration_method << endl;
    univariate_datas_.append(univariate_data);}

///
/// \brief VespucciDataset::CorrelationMap
/// \param control The "control" vector to which all spectra are compared
/// \param name The name of the image
/// \param gradient_index The index of the color gradient in the global list
///
void VespucciDataset::CorrelationMap(vec control, QString name, uword gradient_index)
{
    //if dataset is non spatial, just quit
    if(non_spatial_){
        QMessageBox::warning(0, "Non-spatial dataset", "Dataset is non-spatial or non-contiguous! Mapping functions are not available");
        return;
    }
    QSharedPointer<UnivariateData> univariate_data(new UnivariateData(QSharedPointer<VespucciDataset>(this), name, control));
    univariate_data->Apply(0, 0, UnivariateMethod::Correlation);
    QSharedPointer<MapData> new_map(new MapData(x_axis_description_,
                                            y_axis_description_,
                                            x_, y_, univariate_data->results(),
                                            QSharedPointer<VespucciDataset>(this),
                                            directory_,
                                            this->GetGradient(gradient_index),
                                            map_list_model_->rowCount(QModelIndex()),
                                            6,
                                            main_window_));

    new_map->set_name(name, univariate_data->MethodDescription());


    log_stream_ << "Univariate" << endl;
    log_stream_ << "name == " << name << endl;
    log_stream_ << "method == Correlation" << endl;
    log_stream_ << "gradient_index == " << gradient_index << endl;
    univariate_datas_.append(univariate_data);
    map_list_model_->AddMap(new_map);
    new_map->ShowMapWindow();
}

///
/// \brief VespucciDataset::BandRatio
/// Creates a band ratio univariate map. Band ratio maps represent the ratio of
/// two peaks. The determination methods here are identical to those in
/// VespucciDataset::Univariate.
/// \param first_min index of left bound of first region of interest
/// \param first_max index of right bound of first region of interest
/// \param second_min index of left bound of second region of interest
/// \param second_max index of right bound of second region of interest
/// \param name name of the MapData object to be created.  This becomes name of the map to the user
/// \param value_method how the maxima are to be determined (area, derivative, or intensity)
/// \param gradient_index index of gradient in the master list (GetGradient())
///
void VespucciDataset::BandRatio(double first_min, double first_max,
                                double second_min, double second_max,
                                QString name,
                                UnivariateMethod::Method method,
                                unsigned int gradient_index)
{

    //if dataset is non spatial, just quit
    if(non_spatial_){
        QMessageBox::warning(0, "Non-spatial dataset", "Dataset is non-spatial or non-contiguous! Mapping functions are not available");
        return;
    }

    log_stream_ << "BandRatio" << endl;
    log_stream_ << "first_min == " << first_min << endl;
    log_stream_ << "first_max == " << first_max << endl;
    log_stream_ << "second_min == " << second_min << endl;
    log_stream_ << "second_max == " << second_max << endl;
    log_stream_ << "name == " << name << endl;
    log_stream_ << "value_method == " << (method == UnivariateMethod::Area ? "Area Ratio" : "Intensity Ratio") << endl;
    //log_stream_ << "integration_method == " << integration_method << endl;
    log_stream_ << "gradient_index == " << gradient_index << endl << endl;


    QSharedPointer<UnivariateData> univariate_data(new UnivariateData(QSharedPointer<VespucciDataset>(this), name));
    univariate_datas_.append(univariate_data);

    univariate_data->Apply(first_min, first_max, second_min, second_max, method);
\

    QSharedPointer<MapData> new_map(new MapData(x_axis_description_,
                                            y_axis_description_,
                                            x_, y_, univariate_data->results(),
                                            QSharedPointer<VespucciDataset>(this), directory_,
                                            this->GetGradient(gradient_index),
                                            map_list_model_->rowCount(QModelIndex()),
                                            6,
                                            main_window_));


    uvec boundaries = univariate_data->Boundaries();

    if (method == UnivariateMethod::AreaRatio){
        new_map->set_baselines(wavelength_.subvec(boundaries(0), boundaries(1)),
                               wavelength_.subvec(boundaries(2), boundaries(3)),
                               univariate_data->first_baselines(),
                               univariate_data->second_baselines());
    }

    map_list_model_->AddMap(new_map);
    new_map->ShowMapWindow();
}


///
/// \brief BandRatio Band ratio analysis without imaging
/// \param first_min First spectral region of interest left bound.
/// \param first_max First spectral region of interest right bound.
/// \param second_min Second spectral region of interest left bound.
/// \param second_max Second spectral region of interest right bound.
/// \param name Descriptive name exposed to the user.
/// \param method The method.
/// Perform a band ratio analysis without creating an image.
void VespucciDataset::BandRatio(double first_min,
                                double first_max,
                                double second_min,
                                double second_max,
                                QString name,
                                UnivariateMethod::Method method)
{
    log_stream_ << "BandRatio (no image)" << endl;
    log_stream_ << "first_min == " << first_min << endl;
    log_stream_ << "first_max == " << first_max << endl;
    log_stream_ << "second_min == " << second_min << endl;
    log_stream_ << "second_max == " << second_max << endl;
    log_stream_ << "name == " << name << endl;
    log_stream_ << "value_method == " << (method == UnivariateMethod::Area ? "Area Ratio" : "Intensity Ratio") << endl;
    //log_stream_ << "integration_method == " << integration_method << endl;

    QSharedPointer<UnivariateData> univariate_data(new UnivariateData(QSharedPointer<VespucciDataset>(this), name));
    univariate_data->Apply(first_min, first_max, second_min, second_max, method);
    univariate_data->SetName(name);
    univariate_datas_.append(univariate_data);
}

///
/// \brief VespucciDataset::PrincipalComponents
/// Performs principal component analysis on the data.  Uses armadillo's pca routine.
/// This function both calculates and plots principal components maps.
/// \param component the PCA component from which the image will be produced
/// \param name the name of the MapData object to be created
/// \param gradient_index the index of the gradient in the master list (in function GetGradient)
///
void VespucciDataset::PrincipalComponents(int component,
                                  QString name,
                                  int gradient_index, bool recalculate)
{
    //if dataset is non spatial, just quit
    if(non_spatial_){
        QMessageBox::warning(0, "Non-spatial dataset", "Dataset is non-spatial or non-contiguous! Mapping functions are not available. Please use the analysis dialog.");
        return;
    }
    log_stream_ << "PrincipalComponents" << endl;
    log_stream_ << "component == " << component << endl;
    log_stream_ << "name == " << name << endl;
    log_stream_ << "gradient_index == " << gradient_index << endl;
    log_stream_ << "recalculate == " << (recalculate ? "true" : "false") << endl << endl;

    if (recalculate || !principal_components_calculated_){

        component--;

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
            principal_components_data_ = new PrincipalComponentsData(QSharedPointer<VespucciDataset>(this),
                                                                     directory_);
            try{
                principal_components_data_->Apply(spectra_);
            }
            catch(exception e){
                char str[50];
                strcat(str, "PrincipalComponents: ");
                strcat(str, e.what());
                throw std::runtime_error(str);
            }

            int seconds = timer.toc();
            cout << "call to arma::princomp successful. Took " << seconds << " s" << endl;
            principal_components_calculated_ = true;
        }
    }

    QString map_type;
    QTextStream(&map_type) << "(Principal Component " << component + 1 << ")";

    colvec results;
    try{
        results = principal_components_data_->Results(component);
    }
    catch(exception e){
        char str[50];
        strcat(str, "PrincipalComponents: ");
        strcat(str, e.what());
        throw std::runtime_error(str);
    }

    QSharedPointer<MapData> new_map(new MapData(x_axis_description_,
                                            y_axis_description_,
                                            x_, y_, results,
                                            QSharedPointer<VespucciDataset>(this), directory_,
                                            GetGradient(gradient_index),
                                            map_list_model_->rowCount(QModelIndex()),
                                            6,
                                            main_window_));
    new_map->set_name(name, map_type);
    AddMap(new_map);
    new_map->ShowMapWindow();
}

///
/// \brief VespucciDataset::PrincipalComponents
/// Perform PCA without creating an image
void VespucciDataset::PrincipalComponents()
{
    log_stream_ << "PrincipalComponents (no image)" << endl;
    int ok = QMessageBox::question(main_window_, "Principal Components Analysis",
                                   "Principal Components Analysis may take over "
                                   "a minute. During this time, Vespucci will "
                                   "appear to freeze. Would you like to continue?",
                                   QMessageBox::Yes, QMessageBox::No);
    if (ok != QMessageBox::Yes)
        return;


    principal_components_data_ =
            new PrincipalComponentsData(QSharedPointer<VespucciDataset>(this), directory_);
    try{
        principal_components_data_->Apply(spectra_);
        principal_components_calculated_ = true;
    }catch(exception e){
        char str[50];
        strcat(str, "PrincipalComponents: ");
        strcat(str, e.what());
        throw std::runtime_error(str);
    }
}

///
/// \brief VespucciDataset::VertexComponents
/// \param endmembers
/// \param image_component
/// \param name
/// \param gradient_index
/// \param recalculate
///
void VespucciDataset::VertexComponents(uword endmembers,
                               uword image_component,
                               QString name,
                               unsigned int gradient_index,
                               bool recalculate)
{
    //if dataset is non spatial, just quit
    if(non_spatial_){
        QMessageBox::warning(0, "Non-spatial dataset", "Dataset is non-spatial or non-contiguous! Mapping functions are not available");
        return;
    }
    log_stream_ << "VertexComponents" << endl;
    log_stream_ << "endmembers == " << endmembers << endl;
    log_stream_ << "image_component == " << image_component << endl;
    log_stream_ << "gradient_index == " << gradient_index;
    log_stream_ << "recalculate == " << (recalculate ? "true" : "false") << endl << endl;

    if(endmembers == 0){
        log_stream_ << "Endmember count predicted using HySime algorithm: ";
        endmembers = this->HySime();
        log_stream_ << endmembers << endl;
    }

    QString map_type;
    QTextStream(&map_type) << "(Vertex Component " << image_component << ")";
    try{
        if (recalculate || !vertex_components_calculated_){
            vertex_components_data_ = new VCAData(QSharedPointer<VespucciDataset>(this), directory_);
            vertex_components_data_->Apply(spectra_, endmembers);
            vertex_components_calculated_ = true;
        }
    }
    catch(exception e){
        char str[50];
        strcat(str, "VertexComponents: ");
        strcat(str, e.what());
        throw std::runtime_error(str);
    }
    colvec results;
    try{
        results = vertex_components_data_->Results(image_component-1);
    }
    catch(exception e){
        char str[50];
        strcat(str, "VertexComponents: ");
        strcat(str, e.what());
        throw std::runtime_error(str);
    }

    QSharedPointer<MapData> new_map(new MapData(x_axis_description_,
                                                y_axis_description_,
                                                x_, y_, results,
                                                QSharedPointer<VespucciDataset>(this), directory_,
                                                GetGradient(gradient_index),
                                                map_list_model_->rowCount(QModelIndex()),
                                                6,
                                                main_window_));
    new_map->set_name(name, map_type);
    AddMap(new_map);
    new_map->ShowMapWindow();
}

///
/// \brief VespucciDataset::VertexComponents
/// \param endmembers
/// Perform VCA without creating an image
void VespucciDataset::VertexComponents(uword endmembers)
{
    log_stream_ << "VertexComponents (no image)" << endl;
    log_stream_ << "endmembers == " << endmembers << endl;

    if(endmembers == 0){
        log_stream_ << "Endmember count predicted using HySime algorithm: ";
        endmembers = this->HySime();
        log_stream_ << endmembers << endl;
    }

    try{
        vertex_components_data_ = new VCAData(QSharedPointer<VespucciDataset>(this), directory_);
        vertex_components_data_->Apply(spectra_, endmembers);
        vertex_components_calculated_ = true;
    }catch(exception e){
        char str[50];
        strcat(str, "VertexComponents: ");
        strcat(str, e.what());
        throw std::runtime_error(str);
    }
}


///
/// \brief VespucciDataset::PartialLeastSquares
/// Performs PLS regression on data.  Resulting map is score for one PLS Component,
/// taken from one column of the X loadings.
/// PLS is performed once.  Subsequent maps use data from first call, stored
/// as PartialLeastSquaresData object, unless user requests recalculation.
/// \param components the number of components/response variables of the regression data
/// \param name the name of the MapData object to be created.
/// \param gradient_index the index of the color gradient in the color gradient list
/// \param recalculate whether or not to recalculate PLS regression.
///
void VespucciDataset::PartialLeastSquares(uword components,
                                  uword image_component,
                                  QString name,
                                  unsigned int gradient_index,
                                  bool recalculate)
{
    //if dataset is non spatial, just quit
    if(non_spatial_){
        QMessageBox::warning(0, "Non-spatial dataset", "Dataset is non-spatial or non-contiguous! Mapping functions are not available");
        return;
    }
    log_stream_ << "PartialLeastSqares" << endl;
    log_stream_ << "components == " << components << endl;
    log_stream_ << "image_component == " << image_component << endl;
    log_stream_ << "name == " << name << endl;
    log_stream_ << "gradient_index == " << gradient_index << endl;
    log_stream_ << "recalculate == " << (recalculate ? "true" : "false") << endl << endl;

    if(components == 0){
        log_stream_ << "Component count predicted using HySime algorithm: ";
        components = this->HySime();
        log_stream_ << components << endl;
        if (image_component > components){
            image_component = components;
        }
    }

    image_component--;
    QString map_type = "Partial Least Squares Map number of components = ";
    try{
        if (recalculate || !partial_least_squares_calculated_){
            map_type += QString::number(components);
            partial_least_squares_data_ = new PLSData(QSharedPointer<VespucciDataset>(this), directory_);
            bool success = partial_least_squares_data_->Apply(spectra_, wavelength_, components);
            if (success){
                partial_least_squares_calculated_ = true;
            }
        }
    }
    catch(exception e){
        char str[50];
        strcat(str, "PartialLeastSquares: ");
        strcat(str, e.what());
        throw std::runtime_error(str);
    }


    bool valid;
    colvec results;
    try{
        results = partial_least_squares_data_->Results(image_component, valid);
        if (!valid){
        QMessageBox::warning(main_window_, "Index out of Bounds",
                             "The component number requested is greater than"
                             "the number of components calculated. Map generated"
                             "corresponds to the highest component number"
                             "calculated");
        }
    }
    catch(exception e){
        char str[50];
        strcat(str, "PartialLeastSquares: ");
        strcat(str, e.what());
        throw std::runtime_error(str);
    }


    map_type += QString::number(partial_least_squares_data_->NumberComponents());
    map_type += ". Component number " + QString::number(image_component);

    QSharedPointer<MapData> new_map(new MapData(x_axis_description_,
                                            y_axis_description_,
                                            x_, y_, results,
                                            QSharedPointer<VespucciDataset>(this), directory_,
                                            this->GetGradient(gradient_index),
                                            map_list_model_->rowCount(QModelIndex()),
                                            6,
                                            main_window_));
    new_map->set_name(name, map_type);
    map_list_model_->AddMap(new_map);
    new_map->ShowMapWindow();
}

///
/// \brief VespucciDataset::PartialLeastSquares
/// \param components
/// Perform partial least squares without creating an image
void VespucciDataset::PartialLeastSquares(uword components)
{
    log_stream_ << "PartialLeastSqares (no image)" << endl;
    log_stream_ << "components == " << components << endl;

    if(components == 0){
        log_stream_ << "Component count predicted using HySime algorithm: ";
        components = this->HySime();
        log_stream_ << components << endl;
    }


    try{
        partial_least_squares_data_ = new PLSData(QSharedPointer<VespucciDataset>(this), directory_);
        bool success = partial_least_squares_data_->Apply(spectra_, wavelength_, components);
        if (success){
            partial_least_squares_calculated_ = true;
        }
    }catch(exception e){
        char str[50];
        strcat(str, "PartialLeastSquares: ");
        strcat(str, e.what());
        throw std::runtime_error(str);
    }
}


///
/// \brief VespucciDataset::KMeans
/// Implements K-means clustering using MLPACK
/// \param clusters Number of clusters to find
/// \param metric Distance metric
/// \param name Name of map in workspace.
///
void VespucciDataset::KMeans(size_t clusters, QString metric_text, QString name)
{
    if (metric_text == "Euclidean"){
        mlpack::kmeans::KMeans<mlpack::metric::EuclideanDistance> k;
        try{
            Col<size_t> assignments;
            mat data = trans(spectra_);
            k.Cluster(data, clusters, assignments);
            k_means_data_.set_size(assignments.n_elem, 1);
            k_means_calculated_ = true;
            //assignments += ones(assignments.n_elem);
            //k_means_data_ = assignments;
            //The assignment operator no longer works in armadillo this is temp kludge
            for (uword i = 0; i < k_means_data_.n_elem; ++i){
                k_means_data_(i) = assignments(i) + 1.0;
            }
        }
        catch(exception e){
            char str[50];
            strcat(str, "KMeans: ");
            strcat(str, e.what());
            throw std::runtime_error(str);
        }

    }

    else if (metric_text == "Manhattan"){
        mlpack::kmeans::KMeans<mlpack::metric::ManhattanDistance> k;
        try{
            Col<size_t> assignments;
            mat data = trans(spectra_);
            k.Cluster(data, clusters, assignments);
            k_means_data_.set_size(assignments.n_elem, 1);
            k_means_calculated_ = true;
            //assignments += ones(assignments.n_elem);
            //k_means_data_ = assignments;
            //The assignment operator no longer works in armadillo this is temp kludge
            for (uword i = 0; i < k_means_data_.n_elem; ++i){
                k_means_data_(i) = assignments(i) + 1.0;
            }
        }
        catch(exception e){
            char str[50];
            strcat(str, "KMeans: ");
            strcat(str, e.what());
            throw std::runtime_error(str);
        }

    }

    else if (metric_text == "Chebyshev"){
        mlpack::kmeans::KMeans<mlpack::metric::ChebyshevDistance> k;
        try{
            Col<size_t> assignments;
            mat data = trans(spectra_);
            k.Cluster(data, clusters, assignments);
            k_means_data_.set_size(assignments.n_elem, 1);
            k_means_calculated_ = true;
            //assignments += ones(assignments.n_elem);
            //k_means_data_ = assignments;
            //The assignment operator no longer works in armadillo this is temp kludge
            for (uword i = 0; i < k_means_data_.n_elem; ++i){
                k_means_data_(i) = assignments(i) + 1.0;
            }
        }
        catch(exception e){
            char str[50];
            strcat(str, "KMeans: ");
            strcat(str, e.what());
            throw std::runtime_error(str);
        }

    }

    else{
        mlpack::kmeans::KMeans<mlpack::metric::SquaredEuclideanDistance> k;
        try{
            Col<size_t> assignments;
            mat data = trans(spectra_);
            k.Cluster(data, clusters, assignments);
            k_means_data_.set_size(assignments.n_elem, 1);
            k_means_calculated_ = true;
            //assignments += ones(assignments.n_elem);
            //k_means_data_ = assignments;
            //The assignment operator no longer works in armadillo this is temp kludge
            for (uword i = 0; i < k_means_data_.n_elem; ++i){
                k_means_data_(i) = assignments(i) + 1.0;
            }
        }
        catch(exception e){
            char str[50];
            strcat(str, "KMeans: ");
            strcat(str, e.what());
            throw std::runtime_error(str);
        }

    }

    //if dataset is non spatial, just quit
    if(non_spatial_){
        QMessageBox::warning(0, "Non-spatial dataset", "Dataset is non-spatial or non-contiguous! Mapping functions are not available");
        return;
    }
    log_stream_ << "KMeans" << endl;
    log_stream_ << "clusters == " << clusters << endl;
    log_stream_ << "name == " << name << endl << endl;

    if(clusters == 0){
        log_stream_ << "Cluster count predicted using HySime algorithm: ";
        clusters = this->HySime();
        log_stream_ << clusters << endl;
    }


    QString map_type = "K-means clustering map. Number of clusters = ";
    map_type += QString::number(clusters);


    QCPColorGradient gradient = GetClusterGradient(clusters);
    QSharedPointer<MapData> new_map(new MapData(x_axis_description_,
                                            y_axis_description_,
                                            x_, y_, k_means_data_.col(0),
                                            QSharedPointer<VespucciDataset>(this), directory_,
                                            gradient,
                                            map_list_model_->rowCount(QModelIndex()),
                                            clusters,
                                            main_window_));
    new_map->set_name(name, map_type);
    new_map->SetCrispClusters(true);
    map_list_model_->AddMap(new_map);
    new_map->ShowMapWindow();

}

///
/// \brief VespucciDataset::KMeans
/// \param clusters
/// Perform k-Means clustering without creating an image
void VespucciDataset::KMeans(size_t clusters)
{
    log_stream_ << "KMeans (no image)" << endl;
    log_stream_ << "clusters == " << clusters << endl;

    if(clusters == 0){
        log_stream_ << "Cluster count predicted using HySime algorithm: ";
        clusters = this->HySime();
        log_stream_ << clusters << endl;
    }

    try{
        Col<size_t> assignments;
        mlpack::kmeans::KMeans<> k;
        mat data = trans(spectra_);
        k.Cluster(data, clusters, assignments);
        k_means_data_.set_size(assignments.n_elem, 1);
        k_means_calculated_ = true;
        for (uword i = 0; i < k_means_data_.n_elem; ++i){
            k_means_data_(i) = assignments(i) + 1.0;
        }
    }
    catch(exception e){
        char str[50];
        strcat(str, "KMeans: ");
        strcat(str, e.what());
        throw std::runtime_error(str);
    }
}

// HELPER FUNCTIONS //

///
/// \brief VespucciDataset::FindRange.
/// Finds the index of the wavelength value closest
/// to the specified wavelength range.
/// \param start the first wavelength in the spectral region of interest
/// \param end the second wavelength in the spectral region of interest
/// \return
///
uvec VespucciDataset::FindRange(double start, double end)
{
    uvec indices(2);
    indices(0) = FindIndex(start);
    indices(1) = FindIndex(end);
    return indices;
}

///
/// \brief VespucciDataset::FindOrigin
/// \return
/// Find the point closest to (0,0) in the map. If the function fails, find the
/// index in the middle of the spatial data.
uword VespucciDataset::FindOrigin()
{
    double delta = std::max(std::abs((x_(1) - x(0))), std::abs((y_(1) - y_(0))));
    uvec zero_x = find(((0-delta) <= x_) && (0 + delta) >= x_);
    vec sub_y = y_.elem(zero_x);
    uvec zero_y = find(((0-delta) <= sub_y) && ((0+delta) >= sub_y));

    // If for some reason this doesn't work, also find the point halfway down.
    uword mid = ((x_.n_rows % 2 == 0) ? x_.n_rows : (2*(x_.n_rows + 1)) / 2);
    return (zero_y.n_rows > 0 ? zero_y(0) : mid);
}

/// \brief VespucciDataset::PointSpectrum
/// \param index
/// \return
///
QVector<double> VespucciDataset::PointSpectrum(const uword index)
{
    //perform bounds check.
    std::vector<double> spectrum_stdvector;
    QVector<double> spectrum_qvector;
    cout << "index  = " << index << endl;
    try{
        if (index > spectra_.n_rows){
            spectrum_stdvector =
                    conv_to< std::vector<double> >::from(spectra_.row(spectra_.n_rows - 1));
        }
        else{
             spectrum_stdvector =
                     conv_to< std::vector<double> >::from(spectra_.row(index));
        }

        spectrum_qvector = QVector<double>::fromStdVector(spectrum_stdvector);
    }
    catch(exception e){
        main_window_->DisplayExceptionWarning(e);
    }

    return spectrum_qvector;
}

QVector<double> VespucciDataset::WavelengthQVector()
{
    std::vector<double> wavelength_stdvector =
            conv_to< std::vector<double> >::from(wavelength_);

    QVector<double> wavelength_qvector =
            QVector<double>::fromStdVector(wavelength_stdvector);

    return wavelength_qvector;
}

uword VespucciDataset::FindIndex(double abscissa_value)
{
    double delta = std::fabs(wavelength_(1) - wavelength_(0));
    uvec indices = find((abscissa_value - delta) < wavelength_ <= (abscissa_value + delta));
    return indices(0);
}

///
/// \brief VespucciDataset::ValueRange
/// Finds the minima and maxima of y variable to properly set axes
///  of QCustomPlot objects
/// \return
///
QCPRange VespucciDataset::ValueRange()
{
    double lower = y_.min();
    double upper = y_.max();
    QCPRange range(upper, lower);
    return range;
}
///
/// \brief VespucciDataset::KeyRange
/// Finds the minima and maxima of x variable to properly set axes
///  of QCustomPlot objects
/// \return
///
QCPRange VespucciDataset::KeyRange()
{
    double lower = x_.min();
    double upper = x_.max();
    QCPRange range(upper, lower);
    return range;
}

///
/// \brief VespucciDataset::KeySize
/// Finds the number of data points in x variable to properly set axes
///  of QCustomPlot objects
/// \return number of unique x values
///
int VespucciDataset::KeySize()
{
    uword i;
    uword x_count=1;
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
/// \brief VespucciDataset::ValueSize
/// Finds number of unique y values for properly setting QCPAxis
/// \return number of unique y values
///
int VespucciDataset::ValueSize()
{

    uword i = 0;
    uword y_count;


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


// MEMBER ACCESS FUNCTIONS //
///
/// \brief VespucciDataset::wavelength
/// \return member wavelength_ (spectrum key values)
///
rowvec VespucciDataset::wavelength()
{
    return wavelength_;
}

rowvec VespucciDataset::wavelength(uvec indices)
{
    return wavelength_.cols(indices);
}

///
/// \brief VespucciDataset::x
/// \return member x_
///
colvec VespucciDataset::x()
{
    return x_;
}

///
/// \brief VespucciDataset::indices
/// \return The indices_ vector
///
vec VespucciDataset::indices()
{
    return indices_;
}

///
/// \brief VespucciDataset::indices_ptr
/// \return A pointer to the indices_ vector
///
mat *VespucciDataset::indices_ptr()
{
    return (mat *) &indices_;
}

///
/// \brief VespucciDataset::SetIndices
/// \param indices A new indices_ vector
///
void VespucciDataset::SetIndices(vec indices)
{
    indices_ = indices;
}

///
/// \brief VespucciDataset::x
/// \param indices Vector of indices
/// \return Subvec of x corresponding to valeus in indices
///
colvec VespucciDataset::x(uvec indices)
{
    return x_(indices);
}

double VespucciDataset::x(uword index)
{
    if (index >= x_.n_rows)
        return x_(x_.n_rows - 1);
    else
        return x_(index);
}



///
/// \brief VespucciDataset::y
/// \return member y_
///
colvec VespucciDataset::y()
{
    return y_;
}

///
/// \brief VespucciDataset::y
/// \param indices Vector of indices
/// \return Subvec of y at indices
///
colvec VespucciDataset::y(uvec indices)
{
    return y_(indices);
}

double VespucciDataset::y(uword index)
{
    if (index >= y_.n_rows)
        return y_(y_.n_rows - 1);
    else
        return y_(index);
}

///
/// \brief VespucciDataset::spectra
/// \return member spectra_
///
mat VespucciDataset::spectra()
{
    return spectra_;
}

///
/// \brief VespucciDataset::spectra
/// \param indices Vector of indices
/// \return Submat of spectra at indices
///
mat VespucciDataset::spectra(uvec indices)
{
    return spectra_.rows(indices);
}

///
/// \brief VespucciDataset::name
/// \return member name_, the name of the dataset as seen by the user
///
const QString VespucciDataset::name()
{
    return name_;
}

///
/// \brief VespucciDataset::SetName
/// \param new_name new name of dataset
///
void VespucciDataset::SetName(QString new_name)
{
    name_ = new_name;
}

///
/// \brief VespucciDataset::SetData
/// \param spectra Spectra
/// \param wavelength Spectral abscissa
/// \param x Colvec of horizontal axis spatial data
/// \param y Colvec of vertical axis spatial data
/// Set the data of the dataset. Used by the MetaDataset constructor
void VespucciDataset::SetData(mat spectra, rowvec wavelength, colvec x, colvec y)
{
    spectra_ = spectra;
    wavelength_ = wavelength;
    x_ = x;
    y_ = y;
}

//MAP HANDLING FUNCTIONS
///
/// \brief VespucciDataset::map_names
/// \return list of names of the maps created from this dataset
///
//QStringList VespucciDataset::map_names()
//{
//    return map_names_;
//}

///
/// \brief VespucciDataset::map_loading_count
/// \return number of maps created for this dataset
///
int VespucciDataset::map_loading_count()
{
    return map_loading_count_;
}

///
/// \brief VespucciDataset::RemoveMapAt
/// \param i index of map in the relevant lists
///
void VespucciDataset::RemoveMapAt(unsigned int i)
{
    map_list_model_->removeRow(i, QModelIndex());

}


///
/// \brief VespucciDataset::AddMap
/// Adds a map to the list of map pointers and adds its name to relevant lists
/// \param map
///
void VespucciDataset::AddMap(QSharedPointer<MapData> map)
{
    map_list_model_->AddMap(map);
}

///
/// \brief VespucciDataset::WavelengthRange
/// \return the range of the wavlength vector (for plotting point spectra)
///
QCPRange VespucciDataset::WavelengthRange()
{
    double min = wavelength_.min();
    double max = wavelength_.max();
    QCPRange range(min, max);
    return range;
}

///
/// \brief VespucciDataset::PointSpectrumRange
/// \param i row of spectra_ containing desired spectrum
/// \return the range of y values for the point spectra at i
///
QCPRange VespucciDataset::PointSpectrumRange(int i)
{
    rowvec row = spectra_.row(i);
    double min = row.min();
    double max = row.max();

    QCPRange range(min, max);
    return range;
}

///
/// \brief VespucciDataset::GetGradient
/// Selects the color gradient from list of presets
/// \param gradient_number
/// \return
///
QCPColorGradient VespucciDataset::GetGradient(int gradient_number)
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
    default: return QCPColorGradient::gpCold;
    }
}

///
/// \brief VespucciDataset::GetClusterGradient
/// Cluster gradients are slightly different from the continuous gradients. This
/// selects the right gradient based on the number of clusters.
/// \param clusters Number of clusters
/// \return Proper color gradient for number of clusters
///
QCPColorGradient VespucciDataset::GetClusterGradient(int clusters)
{
    switch (clusters)
    {
    case 2: return QCPColorGradient::cbCluster2;
    case 3: return QCPColorGradient::cbCluster3;
    case 4: return QCPColorGradient::cbCluster4;
    case 5: return QCPColorGradient::cbCluster5;
    case 6: return QCPColorGradient::cbCluster6;
    case 7: return QCPColorGradient::cbCluster7;
    case 8: return QCPColorGradient::cbCluster8;
    case 9: return QCPColorGradient::cbCluster9;
    default: return QCPColorGradient::cbCluster9;
    }
}

///
/// \brief VespucciDataset::ConstructorCancelled
/// Specifies whether or not the constructor has been canceled. The constructor
/// asks this and cleans everything up in case it is canceled.
/// \return
///
bool VespucciDataset::ConstructorCancelled()
{
    return constructor_canceled_;
}

///
/// \brief VespucciDataset::AverageSpectrum
/// Finds the average of the spectrum. This can be saved by the user.
/// Probably not all that useful, except for determining a spectrum to use as a
/// background spectrum for other maps.
/// \param stats Whether or not to include standard deviations on the second row.
/// \return The average spectrum
///
mat VespucciDataset::AverageSpectrum(bool stats)
{
    mat spec_mean = mean(spectra_, 0);
    rowvec spec_stddev;
    spec_mean = mean(spectra_, 0);
    //insert stddevs on next line if requested
    if (stats){
        spec_stddev = stddev(spectra_, 0);
        spec_mean.insert_rows(1, spec_stddev);
    }
    return spec_mean.t();
}



///
/// \brief VespucciDataset::x_axis_description
/// The x_axis_description is printed on the spectrum viewer.
/// \return Spectral abscissa description.
///
const QString VespucciDataset::x_axis_description()
{
    return x_axis_description_;
}

///
/// \brief VespucciDataset::SetXDescription
/// Sets the value of the spectral abscissa description.
/// \param description
///
void VespucciDataset::SetXDescription(QString description)
{
    x_axis_description_ = description;
}

///
/// \brief VespucciDataset::SetYDescription
/// \param description
/// Sets the value of the spectral ordinate axis description
void VespucciDataset::SetYDescription(QString description)
{
    y_axis_description_ = description;
}

///
/// \brief VespucciDataset::y_axis_description
/// \return The spectral ordinate axis description.
///
const QString VespucciDataset::y_axis_description()
{
    return y_axis_description_;
}

///
/// \brief VespucciDataset::principal_components_calculated
/// Accessor for principal_components_calculated_. The PCA dialog requests this
/// to make sure that the same PCA is not calculated twice.
/// \return Whether or not PCA has been calculated.
///
bool VespucciDataset::principal_components_calculated()
{
    return principal_components_calculated_;
}

///
/// \brief VespucciDataset::vertex_components_calculated
/// Accessor for vertex_components_calculated_. The VCA dialog requests this to
/// make sure that the same VCA is not calculated twice.
/// \return Whether or not VCA has been computed.
///
bool VespucciDataset::vertex_components_calculated()
{
    return vertex_components_calculated_;
}

///
/// \brief VespucciDataset::partial_least_squares_calculated
/// Accessor for partial_least_squares_calculated. The PLS dialog requests this
/// to make sure that the same PLS is not calculated twice.
/// \return Whether or not PLS has been computed.
///
bool VespucciDataset::partial_least_squares_calculated()
{
    return partial_least_squares_calculated_;
}

///
/// \brief VespucciDataset::k_means_calculated
/// Accessor for k_means_calculated_. Used for filling dataviewer.
/// \return Whether or not k means have been calculated.
///
bool VespucciDataset::k_means_calculated()
{
    return k_means_calculated_;
}

///
/// \brief VespucciDataset::principal_components_data
/// Accessor for principal_components_data_
/// \return Pointer to the class that handles the output of arma::princomp
///
PrincipalComponentsData* VespucciDataset::principal_components_data()
{
    return principal_components_data_;
}

///
/// \brief VespucciDataset::vertex_components_data
/// Accessor for vertex_components_data_
/// \return
///
VCAData* VespucciDataset::vertex_components_data()
{
    return vertex_components_data_;
}

///
/// \brief VespucciDataset::partial_least_squares_data
/// Accessor for partial_least_squares_data_;
/// \return
///
PLSData* VespucciDataset::partial_least_squares_data()
{
    return partial_least_squares_data_;
}

///
/// \brief VespucciDataset::k_means_data
/// \return Pointer to the matrix containing the k-means assignments
///
mat *VespucciDataset::k_means_data()
{
    return &k_means_data_;
}

///
/// \brief VespucciDataset::univariate_datas
/// \return The container holding smart pointers to the univariate data objects
///
QList<QSharedPointer<UnivariateData> > VespucciDataset::univariate_datas()
{
    return univariate_datas_;
}

///
/// \brief VespucciDataset::spectra_ptr
/// \return
///
mat* VespucciDataset::spectra_ptr()
{
    return &spectra_;
}



///
/// \brief VespucciDataset::Undoable
/// \return Whether or not the last operation can be undone
///
bool VespucciDataset::Undoable()
{
    return (spectra_old_.n_elem > 0 ? true : false);
}

///
/// \brief VespucciDataset::UnivariateCount
/// \return
/// Number of univariate/band ratio data objects have been created
int VespucciDataset::UnivariateCount()
{
    return univariate_datas_.size();
}

///
/// \brief VespucciDataset::wavelength_ptr
/// \return
///
mat* VespucciDataset::wavelength_ptr()
{
    return &wavelength_;
}

///
/// \brief VespucciDataset::x_ptr
/// \return
///
mat* VespucciDataset::x_ptr()
{
    return &x_;
}

///
/// \brief VespucciDataset::y_ptr
/// \return
///
mat* VespucciDataset::y_ptr()
{
    return &y_;
}

///
/// \brief VespucciDataset::non_spatial
/// \return True if map has empty x_ and y_
///
bool VespucciDataset::non_spatial()
{
    return non_spatial_;
}

///
/// \brief VespucciDataset::meta
/// \return
/// Whether or not this is an instance of MetaDataset
bool VespucciDataset::meta()
{
    return meta_;
}

///
/// \brief VespucciDataset::SetParentDatasetIndices
/// \param parent_dataset_indices
/// Set the parent_dataset_indices_ variable. Used by the MetaDataset constructor.
void VespucciDataset::SetParentDatasetIndices(mat parent_dataset_indices)
{
    parent_dataset_indices_ = parent_dataset_indices;
}

mat *VespucciDataset::parent_dataset_indices()
{
    return &parent_dataset_indices_;
}

///
/// \brief VespucciDataset::last_operation
/// \return Description of last pre-processing operation performed
///
QString VespucciDataset::last_operation()
{
    return last_operation_;
}

MapListModel* VespucciDataset::map_list_model()
{
    return map_list_model_;
}
