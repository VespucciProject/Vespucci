#ifndef BINARYIMPORT_H
#define BINARYIMPORT_H
#include "vespucciworkspace.h"

using namespace arma;


namespace BinaryImport
{
    bool ImportVespucciBinary(QString filename,
                              mat &spectra,
                              rowvec &wavelength,
                              colvec &x, colvec &y);
}

#endif // BINARYIMPORT_H
