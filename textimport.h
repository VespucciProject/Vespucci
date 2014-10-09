#ifndef TEXTIMPORT_H
#define TEXTIMPORT_H

#include "vespucciworkspace.h"

using namespace arma;


namespace TextImport
{
    bool ImportWideText(QString filename,
                        mat &spectra,
                        rowvec &wavelength,
                        colvec &x, colvec &y,
                        bool swap_spatial,
                        QProgressDialog *progress,
                        const QString sep);

    bool ImportLongText(QString filename,
                        mat &spectra,
                        rowvec &wavelength,
                        colvec &x, colvec &y,
                        bool swap_spatial,
                        QProgressDialog *progress);

    bool ImportWitecText(QString x_filename,
                         QString y_filename,
                         QString spectra_filename,
                         mat &spectra,
                         rowvec &wavelength,
                         colvec &x, colvec &y,
                         bool swap_spatial,
                         QProgressDialog *progress);
}

#endif // TEXTIMPORT_H
