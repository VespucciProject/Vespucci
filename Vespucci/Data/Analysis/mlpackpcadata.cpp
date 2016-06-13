#include "Data/Analysis/mlpackpcadata.h"

MlpackPCAData::MlpackPCAData(QString name):
    AnalysisResults(name, "mlpack PCA Results")
{

}

void MlpackPCAData::Apply(const mat &data, bool scale_data)
{
    mlpack::pca::PCA pca_data(scale_data);
    mat transformed_data, eigvec;
    vec eigval, percent_variance;
    try{
        pca_data.Apply(data, transformed_data, eigval, eigvec);
    }
    catch(exception e){
        throw std::runtime_error("MLPACKPCAData::Apply()");
    }
    double eigval_sum = sum(eigval);
    percent_variance = eigval / eigval_sum;
    percent_variance /= 0.01;

    AddMatrix("Projection", transformed_data, QStringList());
    AddMatrix("Loadings", eigvec, QStringList());
    AddMatrix("Eigenvalues", eigval, QStringList());
    AddMatrix("Percent Variance", percent_variance, QStringList());
}

