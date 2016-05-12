#include "Data/Analysis/mlpackpcadata.h"

MlpackPCAData::MlpackPCAData(QString name, bool scaleData):
    AnalysisResults(name, "mlpack PCA Results"),
    pca_data_(scaleData),
    name_(name){}

void MlpackPCAData::Apply(mat data)
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

mat *MlpackPCAData::transformed_data()
{
    return &transformed_data_;
}

vec *MlpackPCAData::eigval()
{
    return &eigval_;
}

mat *MlpackPCAData::eigvec()
{
    return &eigvec_;
}

vec *MlpackPCAData::percent_variance()
{
    return &percent_variance_;
}

mat *MlpackPCAData::value(QString key)
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

const mat &MlpackPCAData::GetMatrix(const QString &key)
{
    if (key == "Projection")
        return transformed_data_;
    else if (key == "Loadings")
        return eigvec_;
    else if (key == "Eigenvalues")
        return eigval_;
    else if (key == "Percent Variance")
        return percent_variance_;
    else
        return EmptyMatrix();
}

QStringList MlpackPCAData::KeyList()
{
    return QStringList({"Projection",
                        "Loadings",
                        "Eigenvalues",
                        "Percent Variance"});
}

QMap<QString, QString> MlpackPCAData::GetMetadata()
{
    return QMap<QString, QString>();
}

QString MlpackPCAData::GetColumnHeading(const QString &key, int column)
{
    if (key == "Projection") return "Score " + QString::number(column+1);
    else if (key == "Loadings") return "Loading " + QString::number(column+1);
    else if (key == "Eigenvalues") return "Eigenvalues";
    else if (key == "Percent Variance") return "Percent Variance";
    else return QString();
}


