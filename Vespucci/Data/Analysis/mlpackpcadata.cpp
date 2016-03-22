#include "Data/Analysis/mlpackpcadata.h"

MLPACKPCAData::MLPACKPCAData(QString name, bool scaleData): pca_data_(scaleData)
{
    name_ = name_;
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

///
/// \brief MLPACKPCAData::GetResults
/// \return The AnalysisResults object that contains calculated results
///
QSharedPointer<AnalysisResults> MLPACKPCAData::GetResults()
{
    QSharedPointer<AnalysisResults> results(new AnalysisResults(name_, "mlpack PCA Data"));
    QStringList column_headings;
    results->AppendObject("Projection", transformed_data_, column_headings);
    for (int i = 1; i < 16; ++i)
        column_headings << "PCA Loading " + QString::number(i);
    results->AppendObject("Loadings", eigvec_, column_headings);
    column_headings.clear();
    column_headings << "PCA Eigenvalues";
    results->AppendObject("Eigenvalues", eigval_, column_headings);
    column_headings.clear();
    column_headings << "PCA Percent Variance";
    results->AppendObject("Percent Variance", percent_variance_, column_headings);
    return results;
}
