#ifndef ARRAYINTERFACE_H
#define ARRAYINTERFACE_H
#include <octave/oct.h>
#include <octave/octave.h>
#include <octave/parse.h>
#include <octave/toplev.h>
#include <octave/idx-vector.h>

class ArrayInterface : public Array<double>
{
public:
    ArrayInterface(Array<double>::ArrayRep *new_rep);
};

#endif // ARRAYINTERFACE_H
