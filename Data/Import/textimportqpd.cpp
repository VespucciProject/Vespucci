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

#include <Data/Import/textimport.h>
#include <Data/Import/textimportqpd.h>

bool TextImport::ImportWideText(QString filename,
                                mat &spectra,
                                vec &abscissa,
                                vec &x, vec &y,
                                bool swap_spatial,
                                QProgressDialog *progress,
                                const QString sep)
{
    if (sep == ""){return false;}
    bool comma_decimals;
    bool valid = CheckFileValidity(filename, comma_decimals);
    if(!valid){
        throw std::runtime_error("Non-text or improperly formatted file");
        return false;
    }

    if(comma_decimals && sep == ","){
        throw std::runtime_error("Comma-separated file with commas for decimals");
        return false;
    }

    QFile inputfile(filename);
    inputfile.open(QIODevice::ReadOnly);
    QTextStream inputstream(&inputfile);

    int i, j;

    inputstream.seek(0);
    QString abscissa_string = inputstream.readLine();

    QStringList abscissa_string_list =
            abscissa_string.split(sep,  QString::SkipEmptyParts);

    int rows = abscissa_string_list.size();
    abscissa.set_size(rows);
    for(i=0; i<rows; ++i){
        abscissa(i) = abscissa_string_list.at(i).toDouble();
    }
    i=0;
    j=0;

    QString spectra_string;

    QStringList spectra_string_list;
    progress->setWindowTitle("Loading Dataset");
    progress->setWindowModality(Qt::WindowModal);

    int spectra_count = 0;
    while(inputstream.readLine()!=NULL){
        ++spectra_count;
    }
    progress->setValue(1);
    progress->setRange(0,spectra_count+1);

    spectra.set_size(rows, spectra_count);
    x.set_size(spectra_count);
    y.set_size(spectra_count);

    progress->setLabelText("Parsing spectra...");

    inputstream.seek(0);
    inputstream.readLine(); //discard it to advance to next line
    bool ok = true;
    //iterate through each row of the input file, loading spatial and spectral values
    for(j=0; j<spectra_count; ++j){
        spectra_string = inputstream.readLine();
        spectra_string_list =
                spectra_string.split(sep, QString::SkipEmptyParts);
        //ignore whitespace or lines with less than 3 entries
        if (spectra_string_list.size() >= 3){
            if (swap_spatial){
                y(j) = spectra_string_list.at(0).toDouble(&ok);
                spectra_string_list.removeAt(0);

                x(j) = spectra_string_list.at(0).toDouble(&ok);
                spectra_string_list.removeAt(0);
            }
            else{
                x(j) = spectra_string_list.at(0).toDouble(&ok);
                spectra_string_list.removeAt(0);

                y(j) = spectra_string_list.at(0).toDouble(&ok);
                spectra_string_list.removeAt(0);
            }

            // Load each spectrum from the working row of the text file
            // to the working column of spectra matrix
            for (i=0; i<rows; ++i)
                spectra(i,j) = spectra_string_list.at(i).toDouble(&ok);
            if (progress->wasCanceled() || !ok)
                return false;
            else
                progress->setValue(j);

        }
    }
    if (abscissa(1) < abscissa(0)){
        abscissa = arma::flipud(abscissa);
        spectra = arma::flipud(spectra);
    }
    return true;
}


///
/// \brief TextImport::ImportLongText
/// \param filename
/// \param spectra
/// \param abscissa
/// \param x
/// \param y
/// \param swap_spatial
/// \param progress
/// \return
/// Unlike wide text files, long text files don't have blank space before the
/// spatial data, so we can import this directly.
/// These text files are in this format:
/// x y wl intensity
///
bool TextImport::ImportLongText(QString filename,
                                mat &spectra,
                                vec &abscissa,
                                vec &x, vec &y,
                                bool swap_spatial,
                                QProgressDialog *progress)
{
    bool comma_decimals;
    bool valid = CheckFileValidity(filename, comma_decimals);
    if (!valid)
        return false;

    progress->setWindowTitle("Loading File");
    progress->setWindowModality(Qt::WindowModal);
    progress->setValue(0);
    mat all_data;
    //should be able to detect whether csv or tab-delimited
    try{
        all_data.load(filename.toStdString());
    }
    catch(exception e)
    {
        cerr << "Exception thrown in TextImport::ImportLongText" << endl;
        return false;
    }

    vec all_x;
    vec all_y;
    vec all_abscissa = all_data.col(2);
    vec all_spectra = all_data.col(3);

    if (swap_spatial){
        all_x = all_data.col(0);
        all_y = all_data.col(1);
    }
    else{
        all_x = all_data.col(1);
        all_y = all_data.col(0);
    }
    progress->setValue(progress->maximum());
    progress->setWindowTitle("Parsing File");
    progress->setValue(0);
    double abscissa_max = all_abscissa.max();


    bool max_to_min = (all_abscissa(1) < all_abscissa(0) ? true : false);

    //This is the location of the maximum of each copy of the abscissa vector
    //The size of this vector is the number of spectra.

    uvec max_indices = find(all_abscissa == abscissa_max);

    x.set_size(max_indices.n_elem);
    y.set_size(max_indices.n_elem);
    x = all_x.elem(max_indices);
    y = all_y.elem(max_indices);
    uword abscissa_size = all_data.n_rows / max_indices.n_elem;
    abscissa.set_size(abscissa_size);
    abscissa = all_abscissa.subvec(0, abscissa_size - 1);

    spectra.set_size(abscissa.n_elem, x.n_elem);

    uword range_start = 0;
    uword range_end = x.n_elem - 1;

    //Reading by columns because armadillo matrices are stored in column-major
    //format
    progress->setMaximum(spectra.n_cols);
    for (uword i = 0; i < spectra.n_cols; ++i){
        spectra.col(i) = all_spectra.rows(range_start, range_end);
        range_start += spectra.n_rows;
        range_end += spectra.n_rows;
    }
    if (progress->wasCanceled())
        return false;
    //must reverse spectra and abscissa if they were read in backward

    if (abscissa(1) < abscissa(0)){
        abscissa = arma::flipud(abscissa);
        spectra = arma::flipud(spectra);
    }

    return true;

}

