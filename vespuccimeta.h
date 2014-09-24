#ifndef VESPUCCIMETA_H
#define VESPUCCIMETA_H
#include <mlpack/core.hpp>
using namespace arma;
class VespucciMeta
{
public:
    VespucciMeta();

    mat results();
    mat results(uvec indices);
    double results(const uword row);
    double results(const uword i, const uword j);
private:
    mat results_;
};

class PCAMeta: public VespucciMeta
{

};

class VCAMeta: public VespucciMeta
{

};

class PLSMeta: public VespucciMeta
{

};

class KMeansMeta: public VespucciMeta
{

};

#endif // VESPUCCIMETA_H
