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
#include "Data/Dataset/vespuccidataset.h"
#include <mlpack/methods/kmeans/kmeans.hpp>
#include <mlpack/methods/quic_svd/quic_svd.hpp>
#include "Data/Import/datasetloader.h"
#include "Data/Import/datasetsaver.h"
#include "Data/Import/textimport.h"
#include <H5Cpp.h>
using namespace arma;
using namespace std;


///
/// \brief VespucciDataset::~VespucciDataset
/// Destructor deletes everything allocated with new that isn't a smart pointer
VespucciDataset::~VespucciDataset()
{
}

bool VespucciDataset::Save(QString filename)
{
    using namespace H5;
    QStringList core_names = CoreMatrixKeys();
    QStringList results_names = AnalysisResultsKeys();
    try{
        H5File file(filename.toStdString(), H5F_ACC_TRUNC);
        DataSpace str_dataspace(H5S_SCALAR);
        StrType str_type(PredType::C_S1, H5T_VARIABLE);
        Attribute str_attr = file.createAttribute("Name", str_type, str_dataspace);
        str_attr.write(str_type, name_.toStdString());

        str_attr = file.createAttribute("x-Axis Description", str_type, str_dataspace);
        str_attr.write(str_type, x_axis_description_.toStdString());

        str_attr = file.createAttribute("y-Axis Description", str_type, str_dataspace);
        str_attr.write(str_type, y_axis_description_.toStdString());

        for (auto core_name: core_names){
            hsize_t dims[2];
            dims[0] = GetCoreMatrix(core_name).n_rows;
            dims[1] = GetCoreMatrix(core_name).n_cols;
            DataSpace dataspace(2, dims);
            DataSet ds(file.createDataSet(core_name.toStdString(),
                                          PredType::NATIVE_DOUBLE, dataspace));
            ds.write(GetCoreMatrix(core_name).memptr(),
                     PredType::NATIVE_DOUBLE, dataspace);
        }

        for (auto results_name: results_names){
            QSharedPointer<AnalysisResults> results = GetAnalysisResult(results_name);
            QStringList matrix_names = results->KeyList();
            Group results_group(file.createGroup(results_name.toStdString()));
            for (auto matrix_name: matrix_names){
                hsize_t dims[2];
                dims[0] = results->GetMatrix(matrix_name).n_rows;
                dims[1] = results->GetMatrix(matrix_name).n_cols;
                DataSpace dataspace(2, dims);
                DataSet ds(results_group.createDataSet(matrix_name.toStdString(),
                                                       PredType::NATIVE_DOUBLE,
                                                       dataspace));
                ds.write(results->GetMatrix(matrix_name).memptr(),
                         PredType::NATIVE_DOUBLE);
            }
        }
        file.close();
    }
    catch(FileIException error){
        QString msg = QString::fromStdString(error.getDetailMsg());
        main_window_->DisplayWarning("HDF5 FileIException",
                                     msg);
        return false;
    }
    catch(DataSetIException error){
        QString msg = QString::fromStdString(error.getDetailMsg());
        main_window_->DisplayWarning("HDF5 DataSetIException",
                                     msg);
        return false;
    }
    last_save_filename_ = filename;
    saved_ = true;
    state_changed_ = false;
    return true;
}

bool VespucciDataset::SaveSpectrum(QString filename, uword column, file_type type)
{
    std::string filename_stdstring = filename.toStdString();
    vec spectrum;
    bool success;
    try{
        spectrum = spectra_.col(column);
        success = spectrum.save(filename_stdstring, type);
    }
    catch(exception e){
        main_window_->DisplayExceptionWarning("VespucciDataset::SaveSpectrum", e);
        success = false;
    }
    return success;

}

void VespucciDataset::SetOldCopies()
{
    spectra_old_ = spectra_;
    x_old_ = x_;
    y_old_ = y_;
    abscissa_old_ = abscissa_;
}

///
/// \brief VespucciDataset::Contains
/// \param key
/// \return
/// Every AnalysisResult and auxiliary matrix must have a unique name. This is
/// called before an object is added to verify that it doesn't already exist
/// The caller will rename the object if it exists by appending a number that is
/// iteratively increased until the name is unique
bool VespucciDataset::Contains(const QString &key) const
{
    return CoreMatrixKeys().contains(key) || AnalysisResultsKeys().contains(key)
            || AuxiliaryMatrixKeys().contains(key) || key == "Auxiliary Matrices";
}

bool VespucciDataset::IsValid() const
{
    bool has_spatial = x_.n_rows;
    //x and y have same size
    bool valid_spatial = has_spatial && (x_.n_rows == y_.n_rows);
    //x and y have same number of rows as spectra has columns
    bool spatial_match = valid_spatial && (x_.n_rows == spectra_.n_cols);
    //spectra and abscissa have same number of rows
    bool spectral_match = spectra_.n_elem && abscissa_.n_rows == spectra_.n_rows;
    return spectral_match && (spatial_match || !has_spatial);
}

bool VespucciDataset::state_changed() const
{
    return state_changed_;
}

QString VespucciDataset::filename() const
{
    return filename_;
}

QString VespucciDataset::last_save_filename() const
{
    return last_save_filename_;
}

bool VespucciDataset::saved() const
{
    return saved_;
}


///
/// \brief VespucciDataset::VespucciDataset
/// \param name
/// \param h5_filename
/// \param main_window
/// \param directory
/// Constructor to load dataset from an HDF5 file.
/// Restrictions:
/// Top level DataSets with names "Spectra" and "Spectral Abscissa" are required.
/// Only three levels are recognized, Dataset, AnalysisResult, Matrix
/// All matrices other than "x", "y", "abscissa" or "spectra" will be loaded
/// into the auxiliary_matrices_ object.
///
/// All DataSets must have 2 dimensions only (column vectors stored as Nx1 matrices)
///
/// Attributes will be used to set metadata
///
///
/// All ananalysis results groups are added as base AnalysisResults
VespucciDataset::VespucciDataset(const QString &h5_filename,
                                 MainWindow *main_window,
                                 QSharedPointer<VespucciWorkspace> ws)
    : auxiliary_matrices_(new AnalysisResults("Auxiliary Matrices", "Auxiliary Matrices")), empty_matrix_()
{
    saved_ = true;
    filename_ = h5_filename;
    last_save_filename_ = h5_filename;
    workspace_ = ws;
    QString fallback_name = "Dataset";
    directory_ = workspace_->directory_ptr();
    int counter = 1;
    while (workspace_->dataset_names().contains(fallback_name))
        fallback_name = "Dataset " + QString::number(counter++);
    QString abs_label = workspace_->settings()->value("absLabel").toString();
    QString abs_units = workspace_->settings()->value("absUnits").toString();
    QString ord_label = workspace_->settings()->value("ordLabel").toString();
    QString ord_units = workspace_->settings()->value("ordUnits").toString();
    QString fallback_x_description = abs_label + "(" + abs_units + ")";
    QString fallback_y_description = ord_label + "(" + ord_units + ")";
    main_window_ = main_window;
    if (!QFile::exists(h5_filename)){
        constructor_canceled_ = true;
        return;
    }
    using namespace H5;

    try{
        H5File file(h5_filename.toStdString(), H5F_ACC_RDONLY);

        if (file.attrExists("Name")){
            Attribute attr = file.openAttribute("Name");
            H5std_string buf("");
            DataType type = attr.getDataType();
            attr.read(type, buf);
            name_ = QString::fromStdString(buf);
        }
        else{
            name_ = fallback_name;
        }
        if (file.attrExists("x-Axis Description")){
            Attribute attr = file.openAttribute("x-Axis Description");
            H5std_string buf("");
            DataType type = attr.getDataType();
            attr.read(type, buf);
            x_axis_description_ = QString::fromStdString(buf);
        }
        else{
            x_axis_description_ = fallback_x_description;
        }
        if (file.attrExists("y-Axis Description")){
            Attribute attr = file.openAttribute("y-Axis Description");
            H5std_string buf("");
            DataType type = attr.getDataType();
            attr.read(type, buf);
            y_axis_description_ = QString::fromStdString(buf);
        }
        else{
            y_axis_description_ = fallback_y_description;
        }

        for (hsize_t i = 0; i < file.getNumObjs(); ++i){
            H5G_obj_t obj_type = file.getObjTypeByIdx(i);
            string obj_name = file.getObjnameByIdx(i);

            if (obj_type == H5G_DATASET){
                DataSet ds = file.openDataSet(obj_name);
                DataSpace dspace = ds.getSpace();
                hsize_t dims[2];
                dspace.getSimpleExtentDims(dims);
                mat matrix(dims[0], dims[1]);
                ds.read(matrix.memptr(), PredType::NATIVE_DOUBLE);
                AddMatrix(QString::fromStdString(obj_name), matrix);
            }
            if (obj_type == H5G_GROUP){
                Group group = file.openGroup(obj_name);
                for (hsize_t j = 0; j < group.getNumObjs(); ++ j){
                    QSharedPointer<AnalysisResults> result(new AnalysisResults(QString::fromStdString(obj_name), ""));
                    H5G_obj_t subobj_type = group.getObjTypeByIdx(j);
                    string subobj_name = group.getObjnameByIdx(j);
                    if (subobj_type == H5G_DATASET){
                        DataSet ds = group.openDataSet(subobj_name);
                        DataSpace dspace = ds.getSpace();
                        hsize_t dims[2];
                        dspace.getSimpleExtentDims(dims);
                        mat matrix(dims[0], dims[1]);
                        ds.read(matrix.memptr(), PredType::NATIVE_DOUBLE);
                        result->AddMatrix(QString::fromStdString(subobj_name),
                                          matrix);
                    }//if (subobj_type == H5G_DATASET
                }//for objects in group
            }//if (obj_type == H5G_GROUP)
        }//for objects in file

        file.close();
        if (AnalysisResultsKeys().contains("Auxiliary Matrices")){
            auxiliary_matrices_ = GetAnalysisResult("Auxiliary Matrices");
        }
        else{
            auxiliary_matrices_ = QSharedPointer<AnalysisResults>(new AnalysisResults("Auxiliary Matrices", "Auxiliary Matrices"));
            AddAnalysisResult(auxiliary_matrices_);
        }
        constructor_canceled_ = false;
    }//try
    catch(H5::Exception error){
        cerr << error.getDetailMsg() << "\n";
        constructor_canceled_ = IsValid();
    }
    catch(std::exception e){
        cerr << e.what() << "\n";
        constructor_canceled_ = IsValid();
    }

    if (!constructor_canceled_){
        if (AnalysisResultsKeys().contains("Auxiliary Matrices")){
            auxiliary_matrices_ = GetAnalysisResult("Auxiliary Matrices");
        }
        else{
            AddAnalysisResult(auxiliary_matrices_);
        }
    }

}

///
/// \brief VespucciDataset::VespucciDataset
/// \param text_filename The filename of the text file from which data is imported
/// \param main_window The main window of the program
/// \param directory The current global working directory
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
                                 QString name,
                                 QString x_axis_description,
                                 QString y_axis_description,
                                 bool swap_spatial,
                                 std::string format)
    : auxiliary_matrices_(new AnalysisResults("Auxiliary Matrices", "Auxiliary Matrices"))
{
    saved_ = false;
    filename_ = text_filename;
    workspace_ = main_window->workspace_ptr();
    QDateTime datetime = QDateTime::currentDateTimeUtc();
    state_changed_ = true;
    analysis_results_.append(auxiliary_matrices_);

    non_spatial_ = false;
    meta_ = false;
    //Set up variables unrelated to hyperspectral data:
    //map_list_model_ = new MapListModel(main_window, this);
    map_loading_count_ = 0;
    z_scores_calculated_ = false;
    directory_ = directory;
    flipped_ = swap_spatial;

    QProgressDialog progress("Loading Dataset...", "Cancel", 0, 100, NULL);
    vec indices_temp;


    if (format == "WideTabDel"){
        try{
            constructor_canceled_ = TextImport::ImportWideText(text_filename.toStdString(),
                                       spectra_,
                                       abscissa_,
                                       x_, y_, swap_spatial);
            indices_ = linspace<vec>(0, x_.n_elem - 1, x_.n_elem);
        }
        catch(exception e){
            string str = "Text import constructor:" +  string(e.what());
            throw std::runtime_error(str);
        }
    }
    else if (format == "WideCSV"){
        try{
            constructor_canceled_ = TextImport::ImportWideText(text_filename.toStdString(),
                                       spectra_,
                                       abscissa_,
                                       x_, y_, swap_spatial);
            indices_ = linspace<vec>(0, x_.n_elem - 1, x_.n_elem);
        }
        catch(exception e){
            string str = "Text import constructor: " + string(e.what());
            throw std::runtime_error(str);
        }
    }
    else if (format == "LongTabDel" || format == "LongCSV"){
        try{
            constructor_canceled_ = TextImport::ImportLongText(text_filename,
                                                               spectra_,
                                                               abscissa_,
                                                               x_, y_,
                                                               swap_spatial,
                                                               &progress);
            indices_.set_size(x_.n_elem);
            for (uword i = 0; i < indices_.n_elem; ++i)
                indices_(i) = i;
        }
        catch(exception e){
            string str = "Text import constructor: " + string(e.what());
            throw std::runtime_error(str);
        }
    }
    else{
        throw std::runtime_error("Invalid file format for text import constructor");
    }

    constructor_canceled_ = false;
    name_ = name;
    x_axis_description_ = x_axis_description;
    y_axis_description_ = y_axis_description;
    main_window_ = main_window;
}

VespucciDataset::VespucciDataset(map<pair<int, int>, string> text_filenames,
                                 MainWindow *main_window,
                                 QString *directory,
                                 QString name,
                                 QString x_axis_description,
                                 QString y_axis_description,
                                 int rows, int cols)
     : auxiliary_matrices_(new AnalysisResults("Auxiliary Matrices", "Auxiliary Matrices"))
{
    saved_ = false;
    text_filenames_ = text_filenames;
    workspace_ = main_window->workspace_ptr();
    analysis_results_.append(auxiliary_matrices_);
    QDateTime datetime = QDateTime::currentDateTimeUtc();
    non_spatial_ = false;
    meta_ = false;
    //Set up variables unrelated to hyperspectral data:
    //map_list_model_ = new MapListModel(main_window, this);
    map_loading_count_ = 0;
    z_scores_calculated_ = false;
    directory_ = directory;
    flipped_ = false;
    try{
        constructor_canceled_ = TextImport::ImportMultiplePoints(text_filenames,
                                                                 rows, cols,
                                                                 spectra_,
                                                                 abscissa_,
                                                                 x_,
                                                                 y_);

    }
    catch(exception e){
        string reason = "Multi import constructor: " + string(e.what());
        throw std::runtime_error(reason.c_str());
    }
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
/// \param original The dataset from which this dataset is "extracted"
/// \param indices The indices of the spectra in the previous dataset that will form this one
/// This is a constructor to create a new dataset by "extracting" spectra from a
/// another dataset based on values on an image.
VespucciDataset::VespucciDataset(QString name,
                                 MainWindow *main_window,
                                 QString *directory,
                                 QSharedPointer<VespucciDataset> original,
                                 uvec indices)
    : auxiliary_matrices_(new AnalysisResults("Auxiliary Matrices", "Auxiliary Matrices"))

{
    saved_ = false;
    workspace_ = main_window->workspace_ptr();
    analysis_results_.append(auxiliary_matrices_);
    state_changed_ = true;
    non_spatial_ = true;
    meta_ = original->meta();
    map_loading_count_ = 0;
    //principal_components_calculated_ = false;
    //mlpack_pca_calculated_ = false;
    //partial_least_squares_calculated_ = false;
    //vertex_components_calculated_ = false;
    z_scores_calculated_ = false;
    directory_ = directory;
    vec parent_indices;

    try{
        spectra_ = original->spectra(indices);
        abscissa_ = original->wavelength();
        x_ = original->x(indices);
        y_ = original->y(indices);
        parent_indices = original->indices();
        indices_ = parent_indices(indices);
    }
    catch(exception e){
        string str = "Extraction constructor: " + string(e.what());
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
/// Constructor to create a dataset without spatial and spectral data set (i.e.
/// when using MetaDataset or a factory method).
/// Use SetData() to instantiate dataset members
///
VespucciDataset::VespucciDataset(QString name,
                                 MainWindow *main_window,
                                 QString *directory)
    : auxiliary_matrices_(new AnalysisResults("Auxiliary Matrices", "Auxiliary Matrices"))
{
    workspace_ = main_window->workspace_ptr();
    analysis_results_.append(auxiliary_matrices_);
    //map_list_model_ = new MapListModel(main_window, this);
    state_changed_ = true;
    non_spatial_ = true;
    meta_ = true;
    map_loading_count_ = 0;
    //principal_components_calculated_ = false;
    //mlpack_pca_calculated_ = false;
    //partial_least_squares_calculated_ = false;
    //vertex_components_calculated_ = false;
    z_scores_calculated_ = false;
    directory_ = directory;
    name_ = name;
    main_window_ = main_window;
    directory_ = directory;
    saved_ = false;
}

// PRE-PROCESSING FUNCTIONS //
///
/// \brief VespucciDataset::Undo Swap spectra_ and spectra_old_ to undo an action.
/// Calling this function again re-does the action that was undid.
///
void VespucciDataset::Undo()
{

    try{
        mat spectra_buffer = spectra_;
        vec x_buffer = x_;
        vec y_buffer = y_;
        vec abscissa_buffer = abscissa_;

        spectra_.swap(spectra_old_);
        x_.swap(x_old_);
        y_.swap(y_old_);
        abscissa_.swap(abscissa_old_);

        //spectra_ = spectra_old_;
        //x_ = x_old_;
        //y_ = y_old_;
        //abscissa_ = abscissa_old_;

        //spectra_old_ = spectra_buffer;
        //x_old_ = x_buffer;
        //y_old_ = y_buffer;
        //abscissa_old_ = abscissa_buffer;


    }
    catch(exception e){
        string str = "Undo: " + string(e.what());
        throw std::runtime_error(str);
    }

    state_changed_ = true;
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

    SetOldCopies();
    if (!std::isnan(x_min) && !std::isnan(x_max) && !std::isnan(y_min) && !std::isnan(y_max)){
        uvec valid_indices = find ((x_ >= x_min) && (x_ <= x_max));
        try{
            spectra_ = spectra_.cols(valid_indices);
            y_ = y_.rows(valid_indices);
            x_ = x_.rows(valid_indices);
            valid_indices = find((y_ >= y_min) && (y_ <= y_max));
            y_ = y_.rows(valid_indices);
            x_ = x_.rows(valid_indices);
            spectra_ = spectra_.cols(valid_indices);
        }catch(exception e){
            string str = "CropSpectra: " + string(e.what());
            throw std::runtime_error(str);
        }
    }
    if (!std::isnan(wl_min) && !std::isnan(wl_max)){
        uvec valid_indices = find(abscissa_ >= wl_min && abscissa_ <= wl_max);
        try{
            spectra_ = spectra_.rows(valid_indices);
            abscissa_ = abscissa_.rows(valid_indices);

        }catch(exception e){
            string str = "CropSpectra: " + string(e.what());
            throw std::runtime_error(str);
        }
    }

    last_operation_ = "crop";
    operations_ << "CropSpectra("
                   + QString::number(x_min) + ", "
                   + QString::number(x_max) + ", "
                   + QString::number(y_min) + ", "
                   + QString::number(y_max) + ", "
                   + QString::number(wl_min) + ", "
                   + QString::number(wl_max) + ")";
    state_changed_ = true;
    workspace_->UpdateModel();
}


///
/// \brief VespucciDataset::MinMaxNormalize
/// Normalizes each column of spectra_ so that the highest value is 1 and lowest
/// value is 0.
void VespucciDataset::MinMaxNormalize()
{
    try{
        SetOldCopies();
        vec spectrum_buffer;
        vec extrema_buffer;
        for (uword i = 0; i < spectra_.n_cols; ++i){
            spectrum_buffer = spectra_.col(i);
            extrema_buffer = spectrum_buffer.min()*ones<vec>(spectra_.n_rows);
            spectrum_buffer -= extrema_buffer;
            extrema_buffer = spectrum_buffer.max()*ones<vec>(spectra_.n_rows);
            spectrum_buffer /= extrema_buffer;
            spectra_.col(i) = spectrum_buffer;
        }
    }
    catch(exception e){
        string str = "MinMaxNormalize: " + string(e.what());
        throw std::runtime_error(str);
    }
    last_operation_ = "min/max normalize";
    operations_ << "MinMaxNormalize()";
    state_changed_ = true;
}

///
/// \brief VespucciDataset::VectorNormalize
/// \param norm If 1, unit area, if 2 unit length
///
void VespucciDataset::VectorNormalize(uword norm)
{
    if (!norm || norm > 2) return;
    state_changed_ = true;

    SetOldCopies();
    try{
        spectra_ = normalise(spectra_, norm);
    }
    catch(exception e){
        string str = "VectorNormalize()" + string(e.what());
        throw std::runtime_error(str);
    }
    operations_ << "VectorNormalize(" + QString::number(norm) + ")";
    last_operation_ = "vector normalize";
}

///
/// \brief VespucciDataset::MeanCenter
/// Subtract the mean of all spectra in the dataset at each wavelength from each
/// wavelength (i.e. in preparation for PCA)
void VespucciDataset::MeanCenter()
{
    state_changed_ = true;
    SetOldCopies();
    try{
        vec mean_intensity = mean(spectra_, 1);
        spectra_.each_col() -= mean_intensity;
    }
    catch(exception e){
        throw std::runtime_error("MeanCenter");
    }
    last_operation_ = "mean centering";
    operations_ << "MeanCenter()";
}

///
/// \brief VespucciDataset::PeakIntensityNormalize
/// \param left_bound
/// \param right_bound
///
void VespucciDataset::PeakIntensityNormalize(double left_bound, double right_bound)
{
    SetOldCopies();
    vec positions;
    vec peak_maxes = Vespucci::Math::Quantification::FindPeakMaxMat(spectra_, abscissa_, left_bound, right_bound, positions);
    for (uword j = 0; j < spectra_.n_cols; ++j){
        spectra_.col(j) /= peak_maxes(j);
    }
    last_operation_ = "Peak intensity normalize";
    operations_ << "PeakIntensityNormalize("
                   + QString::number(left_bound) + ", "
                   + QString::number(right_bound) + ")";
}

///
/// \brief VespucciDataset::Booleanize
/// \param min
/// \param max
/// \param keep_inside
/// \param oneify
///
void VespucciDataset::Booleanize(double min, double max, bool keep_inside, bool oneify)
{
    SetOldCopies();
    last_operation_ = "Booleanize";

    //set all values on the outside (or inside) of the range to zero
    try{
        if (keep_inside)
            spectra_.elem(find(spectra_ <= min && spectra_ >= max) ).zeros();
        else
            spectra_.elem(find(spectra_ >= min && spectra_ <= max) ).zeros();


        //set all non-zero values equal to one
        if (oneify)
            spectra_.elem( find(spectra_) ).ones();

    }
    catch (exception e){
        cerr << e.what();
        runtime_error f("Booleanize");
        main_window_->DisplayExceptionWarning(f);
    }
    operations_ << "Booleanize(" + QString::number(min) + ", "
                   + QString::number(max) + ", "
                   + (keep_inside ? "true" : "false") + ", "
                   + (oneify ? "true" : "false") + ")";
}

///
/// \brief VespucciDataset::Clamp
/// \param min
/// \param max
///
void VespucciDataset::Clamp(double min, double max)
{
    SetOldCopies();
    last_operation_ = "Clamp";
    state_changed_ = true;


    try{
        spectra_ = clamp(spectra_, min, max);
    }
    catch(exception e){
        cerr << e.what();
        runtime_error f("Clamp");
        main_window_->DisplayExceptionWarning(f);
    }
    operations_ << "Clamp("
                   + QString::number(min) + ", "
                   + QString::number(max) + ")";
}

void VespucciDataset::ShedZeroSpectra()
{
    SetOldCopies();
    vec current_col;
    uvec indices;
    uvec indices_buffer;
    for (uword i = 0; i < spectra_.n_cols; ++i){
        current_col = spectra_.col(i);
        indices_buffer = find(current_col, 1);
        if (indices_buffer.n_elem > 0)
            indices << i;
    }
    try{
        spectra_ = spectra_.cols(indices);
        x_ = x_.rows(indices);
        y_ = y_.rows(indices);
    }
    catch(exception e){
        main_window_->DisplayExceptionWarning("VespucciDataset::ShedZeroSpectra", e);
    }

    workspace_->UpdateModel();
    operations_ << "ShedZeroSpectra()";
}

void VespucciDataset::ShedZeroWavelengths()
{
    state_changed_ = true;

    SetOldCopies();
    vec current_row;
    uvec indices, indices_buffer;
    for (uword i = 0; i < spectra_.n_rows; ++i){
        current_row = spectra_.row(i);
        indices_buffer = find(current_row, 1);
        if (indices_buffer.n_elem > 0)
            indices << i;
    }
    try{
        spectra_ = spectra_.rows(indices);
        abscissa_ = abscissa_.rows(indices);
    }
    catch(exception e){
        main_window_->DisplayExceptionWarning("VespucciDataset::ShedZeroWavelengths", e);
    }
    workspace_->UpdateModel();
    operations_ << "ShedZeroWavelengths()";
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
        normalized = Vespucci::Math::Normalization::StandardScore(spectra_);
    }
    catch(exception e){
        string str = "ZScoreNormCopy: " + string(e.what());
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
    state_changed_ = true;

    SetOldCopies();

    try{
        spectra_ = Vespucci::Math::Normalization::StandardScoreMat(spectra_);
    }
    catch(exception e){
        string str = "ZScoreNormalize: " + string(e.what());
        throw std::runtime_error(str);
    }

    z_scores_calculated_ = true;
    last_operation_ = "Z-score normalize";
    operations_ << "ZScoreNormalize()";

}


void VespucciDataset::SNVNormalize(double offset, bool center)
{
    state_changed_ = true;

    SetOldCopies();
    try{
        spectra_ = Vespucci::Math::Normalization::SNVNorm(spectra_, offset, center);
    }
    catch(exception e){
        string str = "SNVNormalize: " + string(e.what());
        throw std::runtime_error(str);
    }

    last_operation_ = "SNVNormalize";
    operations_ << "SNVNormalize()";
}

void VespucciDataset::AbsoluteValue()
{
    state_changed_ = true;

    SetOldCopies();

    try{
        spectra_ = arma::abs(spectra_);
    }
    catch(exception e){
        string str =
        "AbsoluteValue: " + string(e.what());
        throw std::runtime_error(str);
    }

    last_operation_ = "Absolute value";
    operations_ << "AbsoluteValue()";

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
    state_changed_ = true;
    SetOldCopies();
    if (background.n_rows != spectra_.n_rows){
        QMessageBox::warning(0,
                             "Improper Dimensions!",
                             "The background spectrum has a different number of"
                             " points than the map data."
                             " No subtraction can be performed");
        return;
    }
    else{
        try{
            spectra_.each_col() -= background.col(0);
        }
        catch(exception e){
            string str = "SubtractBackground: " + string(e.what());
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
void VespucciDataset::MFBaseline(int window_size, int iterations)
{
    state_changed_ = true;
    SetOldCopies();
    try{
        baselines_ = spectra_;
        for (uword i = 0; i < uword(iterations); ++i){
            baselines_ = Vespucci::Math::Smoothing::MedianFilterMat(baselines_, window_size);
        }
        spectra_ -= baselines_;
    }
    catch(exception e){
        string str = "MFBaseline: " + string(e.what());
        throw std::runtime_error(str);
    }

    last_operation_ = "baseline correction (median filter)";
    operations_ << "MFBaseline("
                   + QString::number(window_size) + ", "
                   + QString::number(iterations) + ")";
}

void VespucciDataset::CWTBaseline(int lambda, int penalty_order, double SNR_threshold, double peak_shape_threshold)
{

}

void VespucciDataset::IModPolyBaseline(const uword poly_order, const uword max_it, double threshold)
{
    state_changed_ = true;
    SetOldCopies();
    mat baselines(spectra_.n_rows, spectra_.n_cols);
    vec baseline, corrected;
    QProgressDialog *progress = new QProgressDialog(main_window_);
    progress->setMinimum(0);
    progress->setMaximum(spectra_.n_cols-1);
    progress->setWindowTitle("Correcting Baseline");
    progress->setWindowModality(Qt::WindowModal);
    QString colcount = " of " + QString::number(spectra_.n_cols);
    progress->setLabelText("Spectrum 0" + colcount);
    progress->show();
    double err;
    try{
        for (uword i = 0; i < spectra_.n_cols; ++i){
            progress->setValue(i);
            progress->setLabelText("Spectrum " + QString::number(i+1) + colcount);
            Vespucci::Math::LinLeastSq::IModPoly(spectra_.col(i),
                                                 abscissa_, baseline,
                                                 corrected, err,
                                                 poly_order, max_it, threshold);
            baselines.col(i) = baseline;
            spectra_.col(i) = corrected;
            if (progress->wasCanceled()){
                progress->close();
                break;
                Undo();
            }
        }
    }catch(exception e){
          main_window_->DisplayExceptionWarning("VespucciDataset::IModPolyBaseline", e);
    }
    progress->close();
    if (!progress->wasCanceled()){
        //AddAnalysisResult("IModPoly Baselines", baselines);
    }
    operations_ << "IModPolyBaseline("
                   + QString::number(poly_order) + ", "
                   + QString::number(max_it) + ", "
                   + QString::number(threshold) + ")";
    last_operation_ = "IModPoly baseline";
}

///
/// \brief VespucciDataset::RemoveClippedSpectra
/// \param threshold
/// Removes spectra with a maximum value at or above the threshold
void VespucciDataset::RemoveClippedSpectra(double threshold)
{
    SetOldCopies();
    rowvec spectra_max = max(spectra_, 0);

    uvec valid_indices = find(spectra_max < threshold);
    if (valid_indices.n_elem == 0)
        return;

    try{
        spectra_ = spectra_.cols(valid_indices);
        x_ = x_.rows(valid_indices);
        y_ = y_.rows(valid_indices);
    }
    catch(exception e){
        string str = "RemoveClipped Spectra: " + string(e.what());
        throw std::runtime_error(str);
    }

    if (spectra_.n_rows != spectra_old_.n_rows)
        non_spatial_ = true;

    workspace_->UpdateModel();
    last_operation_ = "threshold";
    operations_ << "RemovedClippedSpectra(" + QString::number(threshold) + ")";
}

///
/// \brief VespucciDataset::RemoveFlatSpectra
/// \param threshold
/// Removes "flat" spectra whose maximum is less than the threshold
void VespucciDataset::RemoveFlatSpectra(double threshold)
{
    SetOldCopies();
    rowvec spectra_max = max(spectra_, 0);

    uvec valid_indices = find(spectra_max > threshold);
    if (valid_indices.n_elem == 0)
        return;

    try{
        spectra_ = spectra_.cols(valid_indices);
        x_ = x_.rows(valid_indices);
        y_ = y_.rows(valid_indices);
    }
    catch(exception e){
        string str = "RemoveClipped Spectra: " + string(e.what());
        throw std::runtime_error(str);
    }

    if (spectra_.n_rows != spectra_old_.n_rows)
        non_spatial_ = true;

    workspace_->UpdateModel();
    last_operation_ = "threshold";
    operations_ << "RemoveFlatSpectra(" + QString::number(threshold) + ")";
}

///
/// \brief VespucciDataset::ZeroClippedSpectra
/// \param threshold
///
void VespucciDataset::ZeroClippedSpectra(double threshold)
{
    SetOldCopies();
    rowvec spectra_max = max(spectra_, 0);
    uvec invalid_indices = find(spectra_max > threshold);
    if (invalid_indices.n_elem == 0)
        return;

    try{
        for (uword i = 0; i < invalid_indices.n_rows; ++i)
            spectra_.col(i) = zeros(spectra_.n_rows);
    }
    catch(exception e){
        string str = "RemoveClipped Spectra: " + string(e.what());
        throw std::runtime_error(str);
    }

    if (spectra_.n_rows != spectra_old_.n_rows)
        non_spatial_ = true;

    workspace_->UpdateModel();
    last_operation_ = "threshold";
    operations_ << "ZeroClippedSpectra(" + QString::number(threshold) + ")";
}

///
/// \brief VespucciDataset::ZeroFlatSpectra
/// \param threshold
///
void VespucciDataset::ZeroFlatSpectra(double threshold)
{
    SetOldCopies();
    rowvec spectra_max = max(spectra_, 0);
    uvec invalid_indices = find(spectra_max < threshold);
    if (invalid_indices.n_elem == 0)
        return;

    try{
        for (uword i = 0; i < spectra_.n_cols; ++i)
            spectra_.col(i) = zeros(spectra_.n_rows);
    }
    catch(exception e){
        string str = "RemoveClipped Spectra: " + string(e.what());
        throw std::runtime_error(str);
    }

    if (spectra_.n_rows != spectra_old_.n_rows)
        non_spatial_ = true;

    workspace_->UpdateModel();
    last_operation_ = "threshold";
    operations_ << "ZeroFlatSpectra(" + QString::number(threshold) + ")";
}

//Filtering functions
///
/// \brief VespucciDataset::MedianFilter
/// performs median filtering on the spectral data.  Entries near the boundaries
/// of spectra are not processed. See also VespucciDataset::LinearMovingAverage
/// \param window_size - an odd number representing the width of the window.

void VespucciDataset::MedianFilter(unsigned int window_size)
{
    state_changed_ = true;
    mat processed;
    SetOldCopies();
    try{
        processed = Vespucci::Math::Smoothing::MedianFilterMat(spectra_, window_size);
        spectra_ = processed;
    }
    catch(exception e){
        string str = "MedianFilter: " + string(e.what());
        throw std::runtime_error(str);
    }

    last_operation_ = "median filter";
    operations_ << "MedianFilter(" + QString::number(window_size) + ")";
}

///
/// \brief VespucciDataset::LinearMovingAverage
/// Performs moving average filtering on the spectral data.  Entries near the
/// boundaries of spectra are not processed.  See also VespucciDataset::MedianFilter.
/// \param window_size - an odd number representing the width of the window.

void VespucciDataset::LinearMovingAverage(unsigned int window_size)
{
    state_changed_ = true;
    SetOldCopies();
    try{
        vec filter = Vespucci::Math::Smoothing::CreateMovingAverageFilter(window_size);
        for (uword j = 0; j < spectra_.n_cols; ++j){
            spectra_.col(j) = Vespucci::Math::Smoothing::ApplyFilter(spectra_.col(j), filter);
        }
    }
    catch(exception e){
        string str = "LinearMovingAverage: " + string(e.what());
        throw std::runtime_error(str);
    }

    last_operation_ = "moving average filter";
    operations_ << "LinearMovingAverage(" +QString::number(window_size) + ")";
}

///
/// \brief VespucciDataset::SingularValue
/// Denoises the spectra matrix using a truncated singular value decomposition.
/// The first singular_values singular values are used to "reconstruct" the
/// spectra matrix. The function used to find the truncated SVD is
/// Vespucci::Math::DimensionReduction::svds.
/// \param singular_values Number of singular values to use.
///
void VespucciDataset::SingularValue(unsigned int singular_values)
{
    state_changed_ = true;
    SetOldCopies();
    mat U;
    vec s;
    mat V;
    try{
        svds(U, s, V, sp_mat(spectra_), singular_values);
        spectra_ = -1 * U * diagmat(s) * V.t();
    }
    catch(exception e){
        string str = "SingularValue: " + string(e.what());
        throw std::runtime_error(str);
    }

    last_operation_ = "truncated SVD de-noise";
    operations_ << "SingularValue(" + QString::number(singular_values) + ")";
}


///
/// \brief VespucciDataset::QUIC_SVD
/// \param epsilon Error tolerance fraction for calculated subspace
/// Use the QUIC-SVD algorithm to denoise the spectra by finding a lower-rank approximation
/// of the matrix.
int VespucciDataset::QUIC_SVD(double epsilon)
{
    SetOldCopies();
    int SVD_rank;
    state_changed_ = true;
    mat u, sigma, v;
    mlpack::svd::QUIC_SVD svd_obj(spectra_, u, v, sigma, epsilon, 0.1);
    SVD_rank = u.n_cols;
    state_changed_ = true;

    spectra_ = u * sigma * v.t();
    last_operation_ = "QUIC_SVD de-noise";
    operations_ << "QUIC_SVD(" + QString::number(epsilon) +  " )";
    return SVD_rank;
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
    state_changed_ = true;
    SetOldCopies();

    try{
        spectra_ = Vespucci::Math::Smoothing::sgolayfilt(spectra_,
                                        polynomial_order,
                                        window_size,
                                        derivative_order,
                                        1);
    }
    catch(exception e){
        string str = "SavitzkyGolay: " + string(e.what());
        throw std::runtime_error(str);
    }

    last_operation_ = "Savitzky-Golay filtering";
    operations_ << "SavitzkyGolay("
                   + QString::number(derivative_order) + ", "
                   + QString::number(polynomial_order) + ", "
                   + QString::number(window_size) + ")";
}

void VespucciDataset::Scale(double scaling_factor)
{
    state_changed_ = true;
    SetOldCopies();

    try{
        spectra_ = spectra_ * scaling_factor;
    }catch(exception e){
     string str = "Scale(): " + string(e.what());
     throw std::runtime_error(str);
    }

    last_operation_ = "Scaling";
    operations_ << "Scale(" + QString::number(scaling_factor) + ")";
}

///
/// \brief VespucciDataset::ShedSpectrum
/// \param index Index of spectrum to shed.
/// Removes a spectrum from the dataset
void VespucciDataset::ShedSpectrum(const uword index)
{
    cout << "VespucciDataset::ShedSpectrum\n";

    cout << "index = " << index << "x = " << x_(index) << " y = " << y_(index) << "\n";

    state_changed_ = true;

    SetOldCopies();
    cout << "spectra columns = " << spectra_.n_cols;
    try{
        spectra_.shed_col(index);
        x_.shed_row(index);
        y_.shed_row(index);
        indices_.shed_row(index);
    }
    catch(exception e){
        cout << e.what();
        std::runtime_error exc("VespucciDataset::ShedSpectrum");
        main_window_->DisplayExceptionWarning(exc);
    }
    cout << "spectra_ columns (post) = " << spectra_.n_cols;
    operations_ << "ShedSpectrum(" + QString::number(index) + ")";
    workspace_->UpdateModel();
}

///
/// \brief VespucciDataset::HySime
/// \return Dimensionality predicted by HySime algorithm
///
int VespucciDataset::HySime()
{
    cout << "VespucciDataset::HySime\n";

    wall_clock timer;
    mat noise, noise_correlation, subspace;
    cout << "call EstimateAdditiveNoise\n";

    timer.tic();
    Vespucci::Math::DimensionReduction::EstimateAdditiveNoise(noise, noise_correlation, spectra_);
    cout << "took " << timer.toc() << " seconds.\n";

    cout << "Call HySime\n";

    timer.tic();
    int k = Vespucci::Math::DimensionReduction::HySime(spectra_, noise, noise_correlation, subspace);
    cout << "Took " << timer.toc() << " seconds.\n";

    return k;
}

///
/// \brief VespucciDataset::TransformAbscissa
/// \param input_units
/// \param input_factor
/// \param output_units
/// \param output_factor
///
void VespucciDataset::TransformAbscissa(QString input_units, double input_factor, QString output_units, double output_factor, QString description)
{
    state_changed_ = true;
    if (input_units == "Wavelength"){
        if (output_units == "Energy"){
            SetOldCopies();
            abscissa_ = Vespucci::Math::WavelengthToEnergy(abscissa_, output_factor, input_factor);
            x_axis_description_ = description;
            last_operation_ = "Abscissa Transform";
        }
        else if (output_units == "Wavenumber"){
            SetOldCopies();
            abscissa_ = Vespucci::Math::WavelengthToWavenumber(abscissa_, output_factor, input_factor);
            x_axis_description_ = description;
            last_operation_ = "Abscissa Transform";
        }
        else if (output_units == "Frequency"){
            SetOldCopies();
            abscissa_ = Vespucci::Math::WavelengthToFrequency(abscissa_, output_factor, input_factor);
            x_axis_description_ = description;
            last_operation_ = "Abscissa Transform";
        }
        else if (output_units == "Wavelength"){
            SetOldCopies();
            abscissa_ = (input_factor * output_factor) * abscissa_;
            x_axis_description_ = description;
            last_operation_ = "Abscissa Transform";
        }
        else{return;}
    }
    else if (input_units == "Energy"){
        if (output_units == "Energy"){
            SetOldCopies();
            abscissa_ = (input_factor * output_factor) * abscissa_;
            x_axis_description_ = description;
            last_operation_ = "Abscissa Transform";
        }
        else if (output_units == "Wavenumber"){
            SetOldCopies();
            abscissa_ = Vespucci::Math::EnergyToWavenumber(abscissa_, output_factor, input_factor);
            x_axis_description_ = description;
            last_operation_ = "Abscissa Transform";
        }
        else if (output_units == "Frequency"){
            SetOldCopies();
            abscissa_ = Vespucci::Math::EnergyToFrequency(abscissa_, output_factor, input_factor);
            x_axis_description_ = description;
            last_operation_ = "Abscissa Transform";
        }
        else if (output_units == "Wavelength"){
            SetOldCopies();
            abscissa_ = Vespucci::Math::EnergyToWavelength(abscissa_, output_factor, input_factor);
            x_axis_description_ = description;
            last_operation_ = "Abscissa Transform";
        }
        else{return;}
    }
    else if (input_units == "Wavenumber"){
        if (output_units == "Energy"){
            SetOldCopies();
            abscissa_ = Vespucci::Math::WavenumberToEnergy(abscissa_, output_factor, input_factor);
            x_axis_description_ = description;
            last_operation_ = "Abscissa Transform";
        }
        else if (output_units == "Wavenumber"){
            SetOldCopies();
            abscissa_ = (input_factor * output_factor) * abscissa_;
            x_axis_description_ = description;
            last_operation_ = "Abscissa Transform";
        }
        else if (output_units == "Frequency"){
            SetOldCopies();
            abscissa_ = Vespucci::Math::WavenumberToFrequency(abscissa_, output_factor, input_factor);
            x_axis_description_ = description;
            last_operation_ = "Abscissa Transform";
        }
        else if (output_units == "Wavelength"){
            SetOldCopies();
            abscissa_ = Vespucci::Math::WavenumberToWavelength(abscissa_, output_factor, input_factor);
            x_axis_description_ = description;
            last_operation_ = "Abscissa Transform";
        }
        else{return;}
    }
    else if (input_units == "Frequency"){
        if (output_units == "Energy"){
            SetOldCopies();
            abscissa_ = Vespucci::Math::FrequencyToEnergy(abscissa_, output_factor, input_factor);
            x_axis_description_ = description;
            last_operation_ = "Abscissa Transform";
        }
        else if (output_units == "Wavenumber"){
            SetOldCopies();
            abscissa_ = Vespucci::Math::FrequencyToWavenumber(abscissa_, output_factor, input_factor);
            x_axis_description_ = description;
            last_operation_ = "Abscissa Transform";
        }
        else if (output_units == "Frequency"){
            SetOldCopies();
            abscissa_ = (input_factor * output_factor) * abscissa_;
            x_axis_description_ = description;
            last_operation_ = "Abscissa Transform";
        }
        else if (output_units == "Wavelength"){
            SetOldCopies();
            abscissa_ = Vespucci::Math::FrequencyToWavelength(abscissa_, output_factor, input_factor);
            x_axis_description_ = description;
            last_operation_ = "Abscissa Transform";
        }
        else{return;}//do nothing for invalid input
    }
    else{return;}
}

///
/// \brief VespucciDataset::InterpolateToNewAbscissa
/// \param new_abscissa
/// \param polynomial_order
/// \param window_size
/// Spline interpolation to new abscissa
void VespucciDataset::InterpolateToNewAbscissa(const vec &new_abscissa, unsigned int poly_order, unsigned int window_size)
{
    state_changed_ = true;
    mat new_spectra;
    try{
        new_spectra = Vespucci::Math::Smoothing::InterpolateToNewAbscissa(spectra_, abscissa_, new_abscissa, window_size, poly_order);
    }catch(exception e){
        string str = "InterpolateToNewAbscissa(vec, uint, uint): " + string(e.what());
        throw std::runtime_error(str);
    }
    SetOldCopies();
    abscissa_ = new_abscissa;
    spectra_ = new_spectra;
    last_operation_ = "Abscissa Interpolation";
    workspace_->UpdateModel();
}

///
/// \brief VespucciDataset::InterpolateToNewAbscissa
/// \param new_abscissa
/// Linear interpolation to new abscissa
void VespucciDataset::InterpolateToNewAbscissa(const vec &new_abscissa)
{
    state_changed_ = true;
    mat new_spectra;
    try{
        new_spectra = Vespucci::Math::Smoothing::InterpolateToNewAbscissa(spectra_, abscissa_, new_abscissa);
    }catch(exception e){
        string str = "InterpolateToNewAbscissa(vec): " + string(e.what());
        throw std::runtime_error(str);
    }
    SetOldCopies();
    abscissa_ = new_abscissa;
    spectra_ = new_spectra;
    last_operation_ = "Abscissa Interpolation";
    workspace_->UpdateModel();
}

///
/// \brief VespucciDataset::FourierTransform
/// \param n
///
void VespucciDataset::FourierTransform(int n)
{
    state_changed_ = true;
    cx_mat f_spectra(spectra_.n_rows, spectra_.n_cols);
    vec f_abscissa(abscissa_.n_rows);
    try{
        Vespucci::Math::Transform::fft_mat(spectra_, abscissa_,
                                           f_spectra, f_abscissa,
                                           n);
    }catch(exception e){
        string str = "FourierTransform: " + string(e.what());
        throw std::runtime_error(str);
    }
    SetOldCopies();
    spectra_ = real(f_spectra);
    spectra_imag_ = imag(f_spectra);
    abscissa_ = f_abscissa;
    x_axis_description_ = "Frequency (Hz)";
}

void VespucciDataset::InverseFourierTransform(int n)
{
    state_changed_ = true;
    cx_mat t_spectra(spectra_.n_rows, spectra_.n_cols);
    vec t_abscissa(abscissa_.n_rows);
    try{
        Vespucci::Math::Transform::ifft_mat(cx_spectra(),
                                            abscissa_,
                                            t_spectra,
                                            t_abscissa,
                                            n);
    }catch(exception e){
        string str = "InverseFourierTransform: " + string(e.what());
        throw std::runtime_error(str);
    }
    SetOldCopies();
    spectra_ = real(t_spectra);
    spectra_imag_ = imag(t_spectra);
    abscissa_ = t_abscissa;
    x_axis_description_ = "Time (s)";
}

///
/// \brief VespucciDataset::ApplyFTWeight
/// \param type
/// \param param
///
void VespucciDataset::ApplyFTWeight(QString type,
                                    double param)
{
    state_changed_ = true;
    SetOldCopies();
    string shape;
    if (type == "Exponential"){shape = "exp";}
    else{shape = "gaus";}
    try{
        spectra_ = Vespucci::Math::Transform::ApplyWeights(spectra_, abscissa_, shape, param);
    }catch(exception e){
        main_window_->DisplayExceptionWarning("VespucciDataset::ApplyFTWeight", e);
    }
}

///
/// \brief VespucciDataset::ApplyFTWeight
/// \param type
/// \param start_offset
/// \param end_offset
/// \param power
///
void VespucciDataset::ApplyFTWeight(double start_offset,
                                    double end_offset,
                                    double power)
{
    state_changed_ = true;
    SetOldCopies();
    try{
        spectra_ = Vespucci::Math::Transform::ApplySBWeights(spectra_,
                                                             abscissa_,
                                                             start_offset,
                                                             end_offset,
                                                             power);
    }catch(exception e){
        main_window_->DisplayExceptionWarning("VespucciDataset::ApplyFTWeight", e);
    }

}


///
/// \brief VespucciDataset::Univariate
/// \param left_bound
/// \param right_bound
/// \param bound_window
///
void VespucciDataset::Univariate(QString name,
                                 double &left_bound, double &right_bound,
                                 uword bound_window)
{
    state_changed_ = true;
    QSharedPointer<UnivariateData> univariate_data(new UnivariateData(name));
    try{
        univariate_data->Apply(left_bound, right_bound, bound_window, spectra_, abscissa_);
    }catch(exception e){
        main_window_->DisplayExceptionWarning(e);
        return;
    }
    analysis_results_.append(univariate_data);
    workspace_->UpdateModel();
    cout << "end of VespucciDataset::Univariate\n";

    operations_ << "Univariate("
                   + name + ", "
                   + QString::number(left_bound) + ", "
                   + QString::number(right_bound) + ", "
                   + QString::number(bound_window) + ")";

}

///
/// \brief VespucciDataset::BandRatio
/// \param first_left_bound
/// \param first_right_bound
/// \param second_left_bound
/// \param second_right_bound
/// \param bound_window
///
void VespucciDataset::BandRatio(QString name, double &first_left_bound, double &first_right_bound, double &second_left_bound, double &second_right_bound, uword bound_window)
{
    state_changed_ = true;
    QSharedPointer<UnivariateData> univariate_data(new UnivariateData(name));
    try{
        univariate_data->Apply(first_left_bound, first_right_bound,
                               second_left_bound, second_right_bound,
                               bound_window,
                               spectra_, abscissa_);
    }catch(exception e){
        main_window_->DisplayExceptionWarning(e);
        return;
    }
    analysis_results_.append(univariate_data);
    workspace_->UpdateModel();
    operations_ << "BandRatio("
                   + name + ", "
                   + QString::number(first_left_bound) + ", "
                   + QString::number(first_right_bound) + ", "
                   + QString::number(second_left_bound) + ", "
                   + QString::number(second_right_bound) + ", "
                   + QString::number(bound_window) + ")";


}


///
/// \brief VespucciDataset::PrincipalComponents
/// \param scaleData
/// \param recalculate
/// Perform PCA using MLPACK's implementation. This is designed for dimensionality
/// reduction, not classification, so there is no corresponding imaging function
void VespucciDataset::PrincipalComponents(const QString &name, bool scale_data)
{
    QSharedPointer<MlpackPCAData> mlpack_pca_data(new MlpackPCAData(name));
    state_changed_ = true;

    try{
        mlpack_pca_data->Apply(spectra_, scale_data);
    }
    catch(exception e){
        throw std::runtime_error("VespucciDataset::PrincipalComponents");
    }
    analysis_results_.append(mlpack_pca_data);
    workspace_->UpdateModel();

}

///
/// \brief VespucciDataset::PrincipalComponents
/// Perform PCA without creating an image
void VespucciDataset::PrincipalComponents(const QString &name)
{
    state_changed_ = true;

    QSharedPointer<PrincipalComponentsData> pca_data(new PrincipalComponentsData(name));
    try{
        pca_data->Apply(spectra_);
    }catch(exception e){
        string str = "PrincipalComponents: " + string(e.what());
        throw std::runtime_error(str);
    }
    analysis_results_.append(pca_data);
    workspace_->UpdateModel();
    operations_ << "PrincipalComponents(" + name + ")";


}

///
/// \brief VespucciDataset::FindPeaks
/// \param sel The amount above the surrounding data for a peak to be identified
/// \param threshold A value which peaks must be larger than to be maxima
/// \param poly_order Polynomial order for Savitzky-Golay derivatization
/// \param window_size Window size for Savitzky-Golay derivatization
///
void VespucciDataset::FindPeaks(QString name, double sel, double threshold, uword poly_order, uword window_size)
{/*

    mat peak_magnitudes;
    mat peak_positions;


    try{
        peak_positions =
                Vespucci::Math::PeakFinding::FindPeaksMat(spectra_,
                                       sel, threshold,
                                       poly_order, window_size,
                                       peak_magnitudes);
    }catch(exception e){
        cerr << e.what();
        throw std::runtime_error("FindPeaks");
    }
    QSharedPointer<AnalysisResults> peak_mag(new AnalysisResults(peak_magnitudes));
    QSharedPointer<AnalysisResults> peak_pos(new AnalysisResults(peak_positions));
    analysis_results_.insert("Peak Positions", peak_pos);
    analysis_results_.insert("Peak Magnitudes", peak_mag);

*/}


///
/// \brief VespucciDataset::VertexComponents
/// \param endmembers
/// Perform VCA without creating an image
void VespucciDataset::VertexComponents(QString name, uword endmembers)
{
    state_changed_ = true;

    if(endmembers == 0){
        QMessageBox alert;
        alert.setText("The HySime algorithm will take a long time.");
        alert.setInformativeText("OK to continue");

        alert.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        alert.setWindowTitle("Principal Components Analysis");
        alert.setIcon(QMessageBox::Question);

        int ret = alert.exec();


        if (ret == QMessageBox::Cancel){
            return;
        }
        endmembers = HySime();
    }

    QSharedPointer<VCAData> vertex_components_data(new VCAData(name));

    try{
        vertex_components_data->Apply(spectra_, endmembers);
    }catch(exception e){
        cerr << "VespucciDataset::VertexComponents()\n";

        throw std::runtime_error("VertexComponents: " + string(e.what()));
    }
    analysis_results_.append(vertex_components_data);
    workspace_->UpdateModel();
    operations_ << "VertexComponents("
                   + name + ", "
                   + QString::number(endmembers) + ")";
}




///
/// \brief VespucciDataset::PartialLeastSquares
/// \param components
/// Perform partial least squares without creating an image
void VespucciDataset::PartialLeastSquares(QString name, uword components)
{
    state_changed_ = true;
    if(components == 0){
        components = HySime();
    }

    QSharedPointer<PLSData> pls_data(new PLSData(name));


    try{
        pls_data->Classify(spectra_, abscissa_, components);
    }catch(exception e){
        string str = "PartialLeastSquares: " + string(e.what());
        throw std::runtime_error(str);
    }

    analysis_results_.append(pls_data);
    workspace_->UpdateModel();
    operations_ << "PartialLeastSquares("
                   + name + ", "
                   + QString::number(components) + ")";

}

///
/// \brief VespucciDataset::CorrelationAnalysis
/// \param control_key
/// \param name
/// Object with key control_key must be column vector or else only first column used
void VespucciDataset::CorrelationAnalysis(const QString &control_key, QString name)
{

    if (!auxiliary_matrices_->HasMatrix(control_key)){
        main_window_->DisplayWarning("Object does not exit", "The object at the specified key does not exist!");
        return;
    }
    vec control;
    try{
        control = auxiliary_matrices_->GetMatrix(control_key).col(0);
    }catch(exception e){
        main_window_->DisplayExceptionWarning("VespucciDataset::CorrelationAnalysis", e);
        return;
    }

    QSharedPointer<UnivariateData> univariate_data(new UnivariateData(name));
    try{
        univariate_data->ApplyCorrelation(spectra_, control);
    }catch(exception e){
        main_window_->DisplayExceptionWarning("UnivariateData::apply", e);
        return;
    }

    state_changed_ = true;
    analysis_results_.append(univariate_data);
    workspace_->UpdateModel();
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
    if(clusters == 0){
        state_changed_ = true;
        clusters = HySime();
    }
    Row<size_t> assignments;
    vec assignments_vec;
    mat centroids;

    if (metric_text == "Euclidean"){
        mlpack::kmeans::KMeans<mlpack::metric::EuclideanDistance> k;
        try{
            k.Cluster(spectra_, clusters, assignments, centroids);
        }
        catch(exception e){
            string str = "KMeans: " + string(e.what());
            throw std::runtime_error(str);
        }
    }

    else if (metric_text == "Manhattan"){
        mlpack::kmeans::KMeans<mlpack::metric::ManhattanDistance> k;
        try{
            k.Cluster(spectra_, clusters, assignments, centroids);
        }
        catch(exception e){
            string str = "KMeans: " + string(e.what());
            throw std::runtime_error(str);
        }
    }

    else if (metric_text == "Chebyshev"){
        mlpack::kmeans::KMeans<mlpack::metric::ChebyshevDistance> k;
        try{
            k.Cluster(spectra_, clusters, assignments, centroids);
        }
        catch(exception e){
            string str = "KMeans: " + string(e.what());
            throw std::runtime_error(str);
        }
    }

    else{
        mlpack::kmeans::KMeans<mlpack::metric::SquaredEuclideanDistance> k;
        try{
            k.Cluster(spectra_, clusters, assignments, centroids);
        }
        catch(exception e){
            string str = "KMeans: " + string(e.what());
            throw std::runtime_error(str);
        }
    }

   assignments_vec.set_size(assignments.n_elem);
   for (uword i = 0; i < assignments.n_elem; ++i)
       assignments_vec(i) = double(assignments(i) + 1);

   state_changed_ = true;
   if (clusters == 0) clusters = HySime();

   QSharedPointer<AnalysisResults> results(new AnalysisResults(name, "k-Means Analysis"));
   results->AddMatrix("Assignments", assignments_vec);
   results->AddMatrix("Centroids", centroids);
   analysis_results_.append(results);
   workspace_->UpdateModel();
   operations_ << "KMeans("
                  + QString::number(clusters) + ", "
                  + metric_text + ", "
                  + name + ")";
}

void VespucciDataset::ClassicalLeastSquares(QString name, QString reference_key)
{
    state_changed_ = true;

    if (!auxiliary_matrices_->HasMatrix(reference_key)){
        main_window_->DisplayWarning("Matrix Not Found", "The reference matrix could not be found!");
        return;
    }
    mat reference(auxiliary_matrices_->GetMatrix(reference_key));
    mat coefs;
    try{
        coefs = Vespucci::Math::LinLeastSq::OrdinaryLeastSquares(reference, spectra_);
    }catch(exception e){
        string str = "Vespucci::Math::LinLeastSq::OrdinaryLeastSquares" + string(e.what());
        throw runtime_error(str);
    }

    QSharedPointer<AnalysisResults> results(new AnalysisResults(name, "CLS Analysis"));
    results->AddMatrix("Coefficients", coefs);
    results->AddMatrix("Reference Matrix", reference);
    analysis_results_.append(results);
    workspace_->UpdateModel();
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
uvec VespucciDataset::FindRange(double start, double end) const
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
uword VespucciDataset::FindOrigin() const
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
QVector<double> VespucciDataset::PointSpectrum(const uword index) const
{
    //perform bounds check.
    std::vector<double> spectrum_stdvector;
    QVector<double> spectrum_qvector;
    cout << "index  = " << index << "\n";

    try{
        if (index > spectra_.n_cols){
            spectrum_stdvector =
                    conv_to< std::vector<double> >::from(spectra_.col(spectra_.n_cols - 1));
        }
        else{
             spectrum_stdvector =
                     conv_to< std::vector<double> >::from(spectra_.col(index));
        }

        spectrum_qvector = QVector<double>::fromStdVector(spectrum_stdvector);
    }
    catch(exception e){
        cerr << "exception thrown!\n";

        main_window_->DisplayExceptionWarning("VespucciDataset::PointSpectrum", e);
    }

    cout << "end of PointSpectrum\n";


    return spectrum_qvector;
}

QVector<double> VespucciDataset::WavelengthQVector() const
{
    std::vector<double> abscissa_stdvector =
            conv_to< std::vector<double> >::from(abscissa_);

    QVector<double> abscissa_qvector =
            QVector<double>::fromStdVector(abscissa_stdvector);

    return abscissa_qvector;
}

uword VespucciDataset::FindIndex(double abscissa_value) const
{
    double delta = std::fabs(abscissa_(1) - abscissa_(0));
    uvec indices = find((abscissa_value - delta) < abscissa_ <= (abscissa_value + delta));
    return indices(0);
}

///
/// \brief VespucciDataset::ValueRange
/// Finds the minima and maxima of y variable to properly set axes
///  of QCustomPlot objects
/// \return
///
QCPRange VespucciDataset::ValueRange() const
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
QCPRange VespucciDataset::KeyRange() const
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
int VespucciDataset::KeySize() const
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
int VespucciDataset::ValueSize() const
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
/// \return member abscissa_ (spectrum key values)
///
vec VespucciDataset::wavelength() const
{
    return abscissa_;
}

vec VespucciDataset::abscissa() const
{
    return abscissa_;
}

vec VespucciDataset::wavelength(uvec indices) const
{
    return abscissa_.rows(indices);
}

///
/// \brief VespucciDataset::x
/// \return member x_
///
vec VespucciDataset::x() const
{
    return x_;
}

///
/// \brief VespucciDataset::indices
/// \return The indices_ vector
///
vec VespucciDataset::indices() const
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
vec VespucciDataset::x(uvec indices) const
{
    return x_(indices);
}

double VespucciDataset::x(uword index) const
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
vec VespucciDataset::y() const
{
    return y_;
}

///
/// \brief VespucciDataset::y
/// \param indices Vector of indices
/// \return Subvec of y at indices
///
vec VespucciDataset::y(uvec indices) const
{
    return y_(indices);
}

double VespucciDataset::y(uword index) const
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
mat VespucciDataset::spectra() const
{
    return spectra_;
}

cx_mat VespucciDataset::cx_spectra() const
{
    if (spectra_imag_.n_rows == spectra_.n_rows && spectra_imag_.n_cols == spectra_.n_cols)
        return cx_mat(spectra_, spectra_imag_);
    else
        return cx_mat(spectra_, zeros(spectra_.n_rows, spectra_.n_cols));
}

cx_mat VespucciDataset::cx_spectra(uvec indices) const
{
    if (spectra_imag_.n_rows == spectra_.n_rows && spectra_imag_.n_cols == spectra_.n_cols)
        return cx_mat(spectra_.cols(indices), spectra_imag_.cols(indices));
    else
        return cx_mat(spectra_.cols(indices), zeros(spectra_.n_rows, indices.n_rows));
}

///
/// \brief VespucciDataset::spectra
/// \param indices Vector of indices
/// \return Submat of spectra at indices
///
mat VespucciDataset::spectra(uvec indices) const
{
    return spectra_.cols(indices);
}

///
/// \brief VespucciDataset::name
/// \return member name_, the name of the dataset as seen by the user
///
const QString VespucciDataset::name() const
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
/// \param x vec of horizontal axis spatial data
/// \param y vec of vertical axis spatial data
/// Set the data of the dataset. Used by the MetaDataset constructor
void VespucciDataset::SetData(const mat &spectra, const vec &wavelength, const vec &x, const vec &y)
{
    spectra_ = spectra;
    abscissa_ = wavelength;
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
int VespucciDataset::map_loading_count() const
{
    return map_loading_count_;
}


///
/// \brief VespucciDataset::AddMap
/// Adds a map to the list of map pointers and adds its name to relevant lists
/// \param map
///
void VespucciDataset::AddMap(QSharedPointer<MapData> map)
{
    maps_.append(map);
    workspace_->UpdateModel();
}

void VespucciDataset::RemoveMap(const QString &name)
{
    if (!MapKeys().contains(name)) return;
    int i = maps_.size();
    while (i--){
        if (maps_[i]->name() == name)
            maps_.removeAt(i); //might be safe to break the while here
    }
}

///
/// \brief VespucciDataset::WavelengthRange
/// \return the range of the wavlength vector (for plotting point spectra)
///
QCPRange VespucciDataset::WavelengthRange() const
{
    double min = abscissa_.min();
    double max = abscissa_.max();
    QCPRange range(min, max);
    return range;
}

///
/// \brief VespucciDataset::PointSpectrumRange
/// \param i col of spectra_ containing desired spectrum
/// \return the range of y values for the point spectra at i
///
QCPRange VespucciDataset::PointSpectrumRange(int i) const
{
    vec row = spectra_.col(i);
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
QCPColorGradient VespucciDataset::GetGradient(int gradient_number) const
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
QCPColorGradient VespucciDataset::GetClusterGradient(int clusters) const
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
    default: return QCPColorGradient::gpJet; //not as good, but should still be different
    }
}

///
/// \brief VespucciDataset::ConstructorCancelled
/// Specifies whether or not the constructor has been canceled. The constructor
/// asks this and cleans everything up in case it is canceled.
/// \return
///
bool VespucciDataset::ConstructorCancelled() const
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
mat VespucciDataset::AverageSpectrum(bool stats) const
{
    mat spec_mean = mean(spectra_, 1);
    vec spec_stddev;
    //insert stddevs on next line if requested
    if (stats){
        spec_stddev = stddev(spectra_, 0, 1);
        spec_mean.insert_cols(1, spec_stddev);
    }
    return spec_mean;
}



///
/// \brief VespucciDataset::x_axis_description
/// The x_axis_description is printed on the spectrum viewer.
/// \return Spectral abscissa description.
///
const QString VespucciDataset::x_axis_description() const
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
const QString VespucciDataset::y_axis_description() const
{
    return y_axis_description_;
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
/// \brief VespucciDataset::spectra_ref
/// \return spectra_ as reference
///
const mat &VespucciDataset::spectra_ref()
{
    return spectra_;
}

///
/// \brief VespucciDataset::abscissa_ref
/// \return abscissa_ as reference
///
const vec &VespucciDataset::abscissa_ref()
{
    return abscissa_;
}

///
/// \brief VespucciDataset::x_ref
/// \return x_ as reference
///
const vec &VespucciDataset::x_ref()
{
    return x_;
}

///
/// \brief VespucciDataset::y_ref
/// \return y_ as reference
///
const vec &VespucciDataset::y_ref()
{
    return y_;
}


///
/// \brief VespucciDataset::Undoable
/// \return Whether or not the last operation can be undone
///
bool VespucciDataset::Undoable() const
{
    return (spectra_old_.n_elem > 0 ? true : false);
}

uword VespucciDataset::UniqueX() const
{
    vec unique_x = unique(x_);
    return unique_x.n_rows;
}

uword VespucciDataset::UniqueY() const
{
    vec unique_y = unique(y_);
    return unique_y.n_rows;
}


///
/// \brief VespucciDataset::UnivariateCount
/// \return
/// Number of univariate/band ratio data objects have been created
int VespucciDataset::UnivariateCount() const
{
    return analysis_results_.size();
}

///
/// \brief VespucciDataset::abscissa_ptr
/// \return
///
mat* VespucciDataset::abscissa_ptr()
{
    return &abscissa_;
}

mat* VespucciDataset::wavelength_ptr()
{
    return &abscissa_;
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
bool VespucciDataset::non_spatial() const
{
    return non_spatial_;
}

///
/// \brief VespucciDataset::meta
/// \return
/// Whether or not this is an instance of MetaDataset
bool VespucciDataset::meta() const
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
const QString VespucciDataset::last_operation() const
{
    return last_operation_;
}


void VespucciDataset::AddAnalysisResult(QSharedPointer<AnalysisResults> analysis_result)
{
    analysis_results_.append(analysis_result);
    workspace_->UpdateModel();
}


///
/// \brief VespucciDataset::AnalysisResultsKeys
/// \return
/// Get a list of keys for analysis results. Used to update display model.
QStringList VespucciDataset::AnalysisResultsKeys() const
{
    QStringList keys;
    for (auto result: analysis_results_)
        keys << result->name();
    return keys;
}

///
/// \brief VespucciDataset::AnalysisResultsTreeStructure
/// \return Container parsed by TreeModel
///
QMap<QString, QStringList> VespucciDataset::AnalysisResultsTreeStructure() const
{
    QMap<QString, QStringList> tree_structure;
    for (auto result: analysis_results_)
        tree_structure[result->name()] = result->KeyList();
    return tree_structure;
}

///
/// \brief VespucciDataset::ImportAuxiliaryMatrix
/// \param name
/// \param filename
/// Load a file into the auxiliary matrix map
void VespucciDataset::ImportAuxiliaryMatrix(const QString &name, const QString &filename)
{
    mat matrix;
    bool ok = matrix.load(filename.toStdString());
    if (ok)
        auxiliary_matrices_->AddMatrix(name, matrix);
    else
        main_window_->DisplayWarning("Matrix Not Loaded", "The file " + name + " could not be loaded");
}

///
/// \brief VespucciDataset::AddAuxiliaryMatrix
/// \param name Display name and internal key of the matrix to add
/// \param matrix Auxiliary matrix to insert into this dataset
///
void VespucciDataset::AddAuxiliaryMatrix(const QString &name, mat &matrix)
{
    auxiliary_matrices_->AddMatrix(name, matrix);
}

void VespucciDataset::AddMatrix(const QString &name, mat &matrix)
{
    if (name == "Spectra")
        spectra_ = matrix;
    else if (name == "x")
        x_ = matrix.col(0);
    else if (name == "y")
        y_ = matrix.col(0);
    else if (name == "Spectral Abscissa")
        abscissa_ = matrix.col(0);
    else
        AddAuxiliaryMatrix(name, matrix);
}

///
/// \brief VespucciDataset::AuxiliaryMatrixKeys
/// \return List of names of imported auxiliary matrices
/// Used in updating display model
QStringList VespucciDataset::AuxiliaryMatrixKeys() const
{
    return auxiliary_matrices_->KeyList();
}

///
/// \brief VespucciDataset::CoreMatrixKeys
/// \return
///
QStringList VespucciDataset::CoreMatrixKeys() const
{
    return {"Spectra", "Spectral Abscissa", "x", "y"};
}

///
/// \brief VespucciDataset::GetAnalysisResultMatrix
/// \param results_key
/// \param matrix_key
/// \return
/// Currently iterates over all results (O(n)) could change structure to use binary
/// tree (O(log n)) but I don't think it should ever be necessary.
const mat & VespucciDataset::GetAnalysisResultMatrix(const QString &results_key, const QString &matrix_key) const
{
    for (auto result: analysis_results_){
        if (result->name() == results_key)
            if (result->KeyList().contains(matrix_key))
                return result->GetMatrix(matrix_key);
    }

    return empty_matrix_;
}

QSharedPointer<AnalysisResults> VespucciDataset::GetAnalysisResult(const QString &key)
{

    for (auto result: analysis_results_){
        if (result->name() == key)
            return result;
    }

    QSharedPointer<AnalysisResults> dummy(new AnalysisResults("Dummy", "Dummy"));
    return dummy;
}

const mat & VespucciDataset::GetAuxiliaryMatrix(const QString &key) const
{
    if (auxiliary_matrices_->HasMatrix(key)) return auxiliary_matrices_->GetMatrix(key);
    else
        return empty_matrix_;
}

const mat &VespucciDataset::GetCoreMatrix(const QString &key) const
{
    if (key == "Spectra"){return spectra_;}
    else if (key == "Spectral Abscissa"){return abscissa_;}
    else if (key == "x"){return x_;}
    else if (key == "y"){return y_;}
    else{return empty_matrix_;}
}

///
/// \brief VespucciDataset::IsCoreMatrix
/// \param key
/// \return Check whether the DatasetTreeItem child is core matrix or not
/// DataViewer needs to know whether to use GetCoreMatrix or GetAuxiliaryMatrix
/// after action is performed on a MatrixTreeItem that is child of DatasetTreeItem
bool VespucciDataset::IsCoreMatrix(const QString &key) const
{
    return (key == "Spectra" || key == "Spectral Abscissa" || key == "x" || key == "y");
}

QSharedPointer<MapData> VespucciDataset::GetMapData(const QString &key)
{
    for (auto map: maps_){
        if (map->name() == key) return map;
    }
    return QSharedPointer<MapData>(0); //return null mapdata
}

///
/// \brief VespucciDataset::CreateMap
/// \param map_name
/// \param results_key
/// \param matrix_key
/// \param column
/// \param gradient
/// \param tick_count
/// Caller should check the number of unique elements in the object before plotting it
/// A cluster color gradient should be used for 8 or less unique elements.
void VespucciDataset::CreateMap(const QString &map_name,
                                const QString &results_key,
                                const QString &matrix_key,
                                uword column,
                                QCPColorGradient gradient,
                                int tick_count)
{
    if (!AnalysisResultsKeys().contains(results_key)) return;
    if (!GetAnalysisResult(results_key)->HasMatrix(matrix_key)) return;
    QStringList data_keys = {name_, results_key, matrix_key};
    QString map_type = GetAnalysisResult(results_key)->type();
    QSharedPointer<MapData> new_map(new MapData(map_name,
                                                map_type,
                                                data_keys,
                                                column,
                                                workspace_));
    new_map->setGradient(gradient);
    new_map->SetColorScaleTickCount(tick_count);
    maps_.append(new_map);
    workspace_->UpdateModel();
}

void VespucciDataset::CreateMap(const QString &map_name,
                                const QString &matrix_key,
                                uword column,
                                QCPColorGradient gradient,
                                int tick_count)
{
    if (!auxiliary_matrices_->HasMatrix(matrix_key)) return;
    QStringList data_keys = {name_, matrix_key};
    QSharedPointer<MapData> new_map(new MapData(map_name,
                                                QString(),
                                                data_keys,
                                                column,
                                                workspace_));
    new_map->setGradient(gradient);
    new_map->SetColorScaleTickCount(tick_count);
    maps_.append(new_map);
    workspace_->UpdateModel();
}

bool VespucciDataset::ShowMapViewer(const QString &map_key, bool show)
{
    if (!MapKeys().contains(map_key)) return false;
    for (auto map: maps_){
        if (map->name() == map_key){
            map->ShowMapWindow(show);
            return true;
        }
    }
    return false;
}

QStringList VespucciDataset::MapKeys() const
{
    QStringList map_keys;
    for (auto map: maps_)
        map_keys << map->name();
    return map_keys;
}


