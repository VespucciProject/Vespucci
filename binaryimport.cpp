#include "binaryimport.h"
bool BinaryImport::ImportVespucciBinary(QString filename,
                                        mat &spectra,
                                        rowvec &wavelength,
                                        colvec &x, colvec &y)
{
    field<mat> input_data;
    bool success = input_data.load(filename.toStdString());
    cout << (success ? "success" : "failure") << endl;
    if(success){
        spectra = input_data(0);
        mat wl= input_data(1);
        mat xm = input_data(2);
        mat ym = input_data(3);

        //field subview doesn't properly convert to vec forms
        wavelength = wl.row(0);
        x = xm.col(0);
        y = ym.col(0);
    }
    return success;
}
