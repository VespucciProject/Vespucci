/*******************************************************************************
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
*******************************************************************************/

#include "metadataset.h"

MetaDataset::MetaDataset()
{
}

///
/// \brief MetaDataset::MetaDataset
/// \param name Name of this dataset as displayed to user
/// \param main_window Main window of the program
/// \param log_file The log file
/// \param directory The current global working directory
/// \param method_description The description of the method used
/// \param endmember_selection The string used by the user to select endmembers
/// \param method An enum specifying the method
/// \param parent_datasets The datasets from which this dataset is extracted
///
MetaDataset::MetaDataset(QString name,
                         MainWindow *main_window,
                         QFile *log_file,
                         QString *directory,
                         QString method_description,
                         QString endmember_selection,
                         VespucciMetaMethod method,
                         QList<QSharedPointer<VespucciDataset> > parent_datasets)
    : VespucciDataset(name, main_window, directory, log_file)
{
    parent_datasets_ = parent_datasets;

    if(!ParentsValid())
        throw std::runtime_error("Improper input to MetaDataset constructor");


    QDateTime datetime = QDateTime::currentDateTimeUtc();
    log_stream_ << "Dataset " << name << "created "
                << datetime.date().toString("yyyy-MM-dd") << "T"
                << datetime.time().toString("hh:mm:ss") << "Z" << endl;
    log_stream_ << "Created from previous datasets:" << endl;
    for(int i = 0; i < parent_datasets_.size(); ++i){
        log_stream_ << "  " << parent_datasets_[i]->name() << endl;
    }

    log_stream_ << endl;

    method_ = method;
    method_description_ = method_description;
    mat spectra;
    rowvec wavelength = parent_datasets_[0]->wavelength();

    switch(method_) {
    case VCAEndmembers:
        try{
            spectra = ProcessVCA(endmember_selection);
        }
        catch(std::exception e){
            throw std::runtime_error("MetaDataset::ProcessVCA");
        }
        break;
    case AverageSpectra:
        try{
            spectra = ProcessAverage();
        }
        catch(std::exception e){
            throw std::runtime_error("MetaDataset::ProcessAverage");
        }

        break;
    case ConcatenateDatasets:
        try{
            spectra = Concatenate();
        }
        catch(std::exception e){
            throw std::runtime_error("MetaDataset::Concatenate");
        }
        break;
    default:
        throw std::runtime_error("Improper input to MetaDataset");
    }
    //we don't preserve spatal data
    colvec x = ones(spectra.n_rows);
    colvec y = ones(spectra.n_rows);
    try{
        SetData(spectra, wavelength, x, y);
    }
    catch(std::exception e){
        throw std::runtime_error("Failure to set data in MetaDataset constructor");
    }
}




///
/// \brief MetaDataset::ProcessVCA
/// \param endmember_selection The string entered by the user to select endmember numbers
/// \return Matrix with chosen endmembers as rows
///
mat MetaDataset::ProcessVCA(QString endmember_selection)
{
    field<uvec> indices_list(parent_datasets_.size());
    QStringList dataset_lists = endmember_selection.split(";");
    QStringList buffer;
    int buffer_int;
    bool ok;
    for (int i = 0; i < parent_datasets_.size(); ++i){
        buffer = dataset_lists[i].split(",");
        for (int j = 0; j < buffer.size(); ++j){
            buffer_int = buffer[j].toUInt(&ok);
            if(ok){
                indices_list(i) << buffer_int;
                endmember_numbers_ << buffer_int;
                parent_indices_ << i;
            }
        }
    }
    //size the matrix ahead of time.
    uword num_rows=0;
    uword num_cols = parent_datasets_[0]->wavelength().n_cols;
    //an unsophistated way to count the total number of endmembers
    for (uword i = 0; i < indices_list.n_elem; ++i)
        for (uword j = 0; j < indices_list.n_elem; ++j)
            ++num_rows;
    mat spectra(num_rows, num_cols);
    uword start_index = 0;
    uword end_index = indices_list(0).n_elem - 1;
    spectra.rows(start_index, end_index) = trans(parent_datasets_[0]->vertex_components_data()->endmember_spectra()->rows(indices_list(0)));
    for (uword i = 0; i < indices_list.n_elem; ++i){
        spectra.rows(start_index, end_index) = parent_datasets_[i]->vertex_components_data()->EndmembersAsRows(indices_list(i));
        start_index += indices_list(i).n_elem;
        end_index += indices_list(i).n_elem;
    }

    return spectra;

}

///
/// \brief MetaDataset::ProcessAverage
/// \return
/// Create a matrix with the average spectrum of each parent dataset on each row
mat MetaDataset::ProcessAverage()
{
    mat spectra(parent_datasets_.size(), parent_datasets_[0]->wavelength().n_elem);

    for (int i = 0; i < parent_datasets_.size(); ++i)
        spectra.row(i) = parent_datasets_[0]->AverageSpectrum(false);

    return spectra;
}

///
/// \brief MetaDataset::Concatenate
/// \return
/// Create a matrix with one dataset after another
mat MetaDataset::Concatenate()
{
    uword num_rows = 0;
    for (int i = 0; i < parent_datasets_.size(); ++i)
        num_rows += parent_datasets_[i]->spectra_ptr()->n_rows;
    mat spectra(num_rows, parent_datasets_[0]->wavelength_ptr()->n_elem);\
    uword start_index = 0;
    uword diff;
    uword end_index = parent_datasets_[0]->spectra_ptr()->n_rows - 1;
    for (int i = 0; i < parent_datasets_.size(); ++i){
        spectra.rows(start_index, end_index) = parent_datasets_[i]->spectra();
        diff = end_index - start_index;
        start_index += diff;
        end_index += diff;
    }

    return spectra;

}

///
/// \brief MetaDataset::ParentsValid
/// \return
/// Checks the validity of the parent datasets to make sure that they have the
/// same (or at least compatible) spectral resolution.
bool MetaDataset::ParentsValid()
{
    uword size = parent_datasets_[0]->wavelength_ptr()->n_elem;
    for (int i = 0; i<parent_datasets_.size(); ++i)
        if (parent_datasets_[i]->wavelength_ptr()->n_elem != size)
            return false;

    return true;
}
