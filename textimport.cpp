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
#include <textimport.h>
bool TextImport::ImportWideText(QString filename,
                                mat &spectra,
                                vec &wavelength,
                                vec &x, vec &y,
                                bool swap_spatial,
                                QProgressDialog *progress,
                                const QString sep)
{
    cout << "ImportWideText" << endl;
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
    QString wavelength_string = inputstream.readLine();

    QStringList wavelength_string_list =
            wavelength_string.split(sep,  QString::SkipEmptyParts);

    int rows = wavelength_string_list.size();
    wavelength.set_size(rows);
    for(i=0; i<rows; ++i){
        wavelength(i) = wavelength_string_list.at(i).toDouble();
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

    return true;
}

///
/// \brief TextImport::ImportLongText
/// \param filename
/// \param spectra
/// \param wavelength
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
                                vec &wavelength,
                                vec &x, vec &y,
                                bool swap_spatial,
                                QProgressDialog *progress)
{
    cout << "ImportLongText" << endl;
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
    vec all_wavelength = all_data.col(2);
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
    double wavelength_max = all_wavelength.max();


    bool max_to_min = (all_wavelength(1) < all_wavelength(0) ? true : false);

    //This is the location of the maximum of each copy of the wavelength vector
    //The size of this vector is the number of spectra.

    cout << "find max_indices" << endl;
    uvec max_indices = find(all_wavelength == wavelength_max);

    x.set_size(max_indices.n_elem);
    y.set_size(max_indices.n_elem);
    x = all_x.elem(max_indices);
    y = all_y.elem(max_indices);
    uword abscissa_size = all_data.n_rows / max_indices.n_elem;
    wavelength.set_size(abscissa_size);
    cout << "get wavelength" << endl;
    wavelength = all_wavelength.subvec(0, abscissa_size - 1);

    spectra.set_size(wavelength.n_elem, x.n_elem);

    uword range_start = 0;
    uword range_end = x.n_elem - 1;

    //Reading by columns because armadillo matrices are stored in column-major
    //format
    progress->setMaximum(spectra.n_cols);
    cout << "loop" << endl;
    for (uword i = 0; i < spectra.n_cols; ++i){
        spectra.col(i) = all_spectra.rows(range_start, range_end);
        range_start += spectra.n_rows;
        range_end += spectra.n_rows;
    }
    if (progress->wasCanceled())
        return false;
    //must reverse spectra and abscissa if they were read in backward
    if (max_to_min){
        spectra = flipud(spectra);
        wavelength = flipud(wavelength);
    }

    return true;

}

bool TextImport::CheckFileValidity(QString filename, bool &comma_decimals)
{
    QFile inputfile(filename);
    inputfile.open(QIODevice::ReadOnly);
    QTextStream inputstream(&inputfile);
    QString line = inputstream.readLine();
    QStringList tab_list = line.split("\t");
    QStringList comma_list = line.split(",");
    bool valid;

    if(tab_list.size() <= 0 && comma_list.size() <= 0){
        return false;
    }
    else if(tab_list.size() > 0 && comma_list.size() > 0){
        //we probably have a tab delimited file with commas for decimal points
        //import function will probabably throw exception if comma is separator
        //and type is wide text, so comma_decimals must be checked
        comma_decimals = true;
        return true;
    }
    else if(tab_list.size() > 0 && comma_list.size() == 0){
        comma_decimals = false;
        for (int i = 0; i < tab_list.size(); ++i){
            tab_list[i].toDouble(&valid);
            if(!valid)
                return false;
        }
        return true;
    }
    else if(tab_list.size() == 0 && comma_list.size() > 0){
        comma_decimals = false; //we don't know if this is true or not.
        //users using instruments that save csv files with local number formatting
        //in regions where commas are used for decimal places should be super
        //careful.
        for (int i = 0; i < comma_list.size(); ++i){
            comma_list[i].toDouble(&valid);
            if(!valid)
                return false;
        }
        return true;
    }
    else{
        return false;
    }


}
