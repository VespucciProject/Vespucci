#include "Data/Analysis/mlpackpcadata.h"

MLPACKPCAData::MLPACKPCAData(bool scaleData): pca_data_(scaleData)
{

}

void MLPACKPCAData::Apply(mat data)
{
   try{
        pca_data_.Apply(data, transformed_data_, eigval_, eigvec_);
    }
    catch(exception e){
        throw std::runtime_error("MLPACKPCAData::Apply()");
    }
    double eigval_sum = sum(eigval_);
    percent_variance_ = eigval_ / eigval_sum;
    percent_variance_ /= 0.01;
}

mat *MLPACKPCAData::transformed_data()
{
    return &transformed_data_;
}

vec *MLPACKPCAData::eigval()
{
    return &eigval_;
}

mat *MLPACKPCAData::eigvec()
{
    return &eigvec_;
}

vec *MLPACKPCAData::percent_variance()
{
    return &percent_variance_;
}

mat *MLPACKPCAData::value(QString key)
{
    if (key == "MLPACK PCA Projection")
        return &transformed_data_;
    if (key == "MLPACK PCA Loadings")
        return &eigvec_;
    if (key == "MLPACK PCA Eigenvalues")
        return (mat*) &eigval_;
    if (key == "MLPACK PCA Variance")
        return (mat*) &percent_variance_;
    else
        return NULL;
}
