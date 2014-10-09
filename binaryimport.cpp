#include "binaryimport.h"
bool BinaryImport::ImportVespucciBinary(QString filename,
                                        mat &spectra,
                                        rowvec &wavelength,
                                        colvec &x, colvec &y)
{
    mat input_data;
    bool success = input_data.load(filename.toStdString());
    spectra = input_data(0);
    wavelength = input_data(1);
    x = input_data(2);
    y = input_data(3);
    return success;
}
