#include <textimport.h>
bool TextImport::ImportWideText(QString filename,
                                mat &spectra,
                                rowvec &wavelength,
                                colvec &x, colvec &y,
                                bool swap_spatial,
                                QProgressDialog *progress,
                                const QString sep)
{
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

    int columns = wavelength_string_list.size();
    wavelength.set_size(columns);

    for(i=0; i<columns; ++i){
        wavelength(i) = wavelength_string_list.at(i).toDouble();
    }
    i=0;
    j=0;

    QString spectra_string;

    QStringList spectra_string_list;
    progress->setWindowTitle("Loading Dataset");
    progress->setWindowModality(Qt::WindowModal);

    int rows = 0;
    while(inputstream.readLine()!=NULL){
        ++rows;
    }
    progress->setValue(1);
    progress->setRange(0,rows+1);

    spectra.set_size(rows, columns);
    x.set_size(rows);
    y.set_size(rows);

    progress->setLabelText("Parsing spectra...");

    inputstream.seek(0);
    inputstream.readLine(); //discard it to advance to next line

    for(i=0; i<rows; ++i){
        spectra_string=inputstream.readLine();
        spectra_string_list =
                spectra_string.split(sep, QString::SkipEmptyParts);

        if (swap_spatial){
            y(i) = spectra_string_list.at(0).toDouble();
            spectra_string_list.removeAt(0);

            x(i) = spectra_string_list.at(0).toDouble();
            spectra_string_list.removeAt(0);
        }
        else{
            x(i) = spectra_string_list.at(0).toDouble();
            spectra_string_list.removeAt(0);

            y(i) = spectra_string_list.at(0).toDouble();
            spectra_string_list.removeAt(0);
        }

        for (j=0; j<columns; ++j){
            spectra(i,j) = spectra_string_list.at(j).toDouble();
        }
        if (progress->wasCanceled()){
            return false;
        }
        else{
            progress->setValue(i);
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
bool TextImport::ImportLongText(QString filename,
                                mat &spectra,
                                rowvec &wavelength,
                                colvec &x, colvec &y,
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
    all_data.load(filename.toStdString());
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
    uvec max_indices = find(all_wavelength == wavelength_max);
    wavelength.set_size(max_indices.n_elem);
    wavelength = all_wavelength.cols(0, max_indices.n_elem - 1);
    uword num_rows = all_wavelength.n_elem / wavelength.n_elem;
    uword num_cols = wavelength.n_elem;
    spectra.set_size(num_rows, num_cols);
    x = all_x.elem(max_indices);
    y = all_y.elem(max_indices);
    uword range_start = 0;
    uword range_end = num_cols - 1;

    //Reading by columns because armadillo matrices are stored in column-major
    //format
    progress->setMaximum(num_cols);
    for (uword i=0; i<num_cols; ++i){
        spectra.row(i) = trans(all_spectra.rows(range_start, range_end));
        range_start += num_cols;
        range_end += num_cols;
    }
    if (progress->wasCanceled())
        return false;
    //must reverse spectra and abcissa if they were read in backward
    if (max_to_min){
        wavelength = fliplr(wavelength);
        spectra = fliplr(spectra);
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
