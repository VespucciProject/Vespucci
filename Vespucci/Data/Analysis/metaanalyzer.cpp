/*******************************************************************************
    Copyright (C) 2014-2016 Wright State University - All Rights Reserved
    Daniel P. Foose - Maintainer/Lead Developer

    This file is part of Vespucci.

    Vespucci is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Vespucci is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Vespucci.  If not, see <http://www.gnu.org/licenses/>.
*******************************************************************************/
#include "metaanalyzer.h"
#include "Data/Analysis/vcadata.h"
#include "Data/Analysis/plsdata.h"
#include "Data/Analysis/principalcomponentsdata.h"
#include "Data/Analysis/mlpackpcadata.h"
#include "mlpack/methods/kmeans/kmeans.hpp"
#include "Math/Clustering/agglomerativeclustering.h"

MetaAnalyzer::MetaAnalyzer(QSharedPointer<VespucciWorkspace> ws, const QStringList &data_keys, bool transpose)
    :workspace_(ws), data_keys_(data_keys)
{
    transpose_ = transpose;
    GetData();
}

void MetaAnalyzer::ClassicalLeastSquares(const QString &name, const QStringList &reference_keys)
{
    QString new_name = FindUniqueName(name);
    QSharedPointer<AnalysisResults> cls_results(new AnalysisResults(new_name, "CLS Analysis"));
    mat reference = workspace_->GetMatrix(reference_keys);
    mat coefs;
    try{
        coefs = Vespucci::Math::LinLeastSq::OrdinaryLeastSquares(reference, data_);
    }catch(exception e){
        workspace_->main_window()->DisplayExceptionWarning(e);
    }
    cls_results->AddMatrix("Coefficients", coefs.t());
    AddResults(cls_results, QStringList({"Coefficients"}));
}

void MetaAnalyzer::VertexComponents(const QString &name, uword endmembers)
{
    QString new_name = FindUniqueName(name);
    QSharedPointer<VCAData> vca_data(new VCAData(new_name));
    vca_data->Apply(data_, endmembers);
    QStringList matrix_keys({"Fractional Abundances"});
    AddResults(vca_data, matrix_keys);
}

void MetaAnalyzer::KMeans(size_t clusters, const QString &metric_text, const QString &name)
{
    QString new_name = FindUniqueName(name);

    mat centroids;
    Row<size_t> assignments;
    QSharedPointer<AnalysisResults> km_results(new AnalysisResults(new_name, "k-Means Analysis"));

    if (metric_text == "Euclidean"){
        mlpack::kmeans::KMeans<mlpack::metric::EuclideanDistance> k;
        k.Cluster(data_, clusters, assignments, centroids);
    }
    else if (metric_text == "Manhattan"){
        mlpack::kmeans::KMeans<mlpack::metric::EuclideanDistance> k;
        k.Cluster(data_, clusters, assignments, centroids);
    }
    else if (metric_text == "Chebyshev"){
        mlpack::kmeans::KMeans<mlpack::metric::ChebyshevDistance> k;
        k.Cluster(data_, clusters, assignments, centroids);
    }
    else{ //Default to squared euclidean distance as in chemometrics literature
        mlpack::kmeans::KMeans<mlpack::metric::SquaredEuclideanDistance> k;
        k.Cluster(data_, clusters, assignments, centroids);
    }

    mat assignments_mat(assignments.n_cols, 1);
    for (uword i = 0; i < assignments.n_rows; ++i){
        assignments_mat(i, 0) = assignments(i) + 1.0;
    }

    km_results->AddMatrix("Assignments", assignments_mat);
    km_results->AddMatrix("Centroids", centroids);
    QStringList matrix_keys({"Assignments"});

    AddResults(km_results, matrix_keys);
}

void MetaAnalyzer::PrincipalComponents(const QString &name)
{
    QString new_name = FindUniqueName(name);
    QSharedPointer<PrincipalComponentsData> pca_data(new PrincipalComponentsData(new_name));
    pca_data->Apply(data_);
    QStringList matrix_keys({"Scores"});
    AddResults(pca_data, matrix_keys);
}

void MetaAnalyzer::PrincipalComponents(const QString &name, bool scale_data)
{
    QString new_name = FindUniqueName(name);
    QSharedPointer<MlpackPCAData> pca_data(new MlpackPCAData(new_name));
    pca_data->Apply(data_, scale_data);
    AddResults(pca_data, QStringList());
}

void MetaAnalyzer::PartialLeastSquares(const QString &name, uword components)
{
    QString new_name = FindUniqueName(name);
    QSharedPointer<PLSData> pls_data(new PLSData(new_name));
    pls_data->Classify(data_, abscissa_, components);
    QStringList matrices({"Predictor Scores", "Response Scores"});
    AddResults(pls_data, matrices);
}

void MetaAnalyzer::PLSCalibration(const QString &name, const QStringList &control_keys)
{
    QString new_name = FindUniqueName(name);
    QSharedPointer<PLSData> pls_data(new PLSData(new_name));
    mat controls = workspace_->GetMatrix(control_keys);
    pls_data->Calibrate(data_, controls);
    QStringList matrices({"Predictor Scores", "Response Scores"});
    AddResults(pls_data, matrices);
}

void MetaAnalyzer::TrainPLSDA(const QString &name, const QStringList &label_keys)
{
    QString new_name = FindUniqueName(name);
    QSharedPointer<PLSData> pls_data(new PLSData(new_name));
    mat labels = workspace_->GetMatrix(label_keys);
    pls_data->Discriminate(data_, labels);
    QStringList matrices({"Predictor Scores", "Response Scores"});
    AddResults(pls_data, matrices);
}

void MetaAnalyzer::AgglomerativeClustering(const QString &name, const QString &metric, const QString &linkage)
{
    QSharedPointer<AnalysisResults> ahca_results(new AnalysisResults(name, "AHCA"));
    mat assignments;
    Vespucci::Math::Clustering::AHCA ahca;
    try{
        ahca.SetLinkage(linkage.toStdString());
        ahca.SetMetric(metric.toStdString());
        ahca.Link(data_);
        assignments = ahca.Cluster(data_.n_cols);
    }catch (exception e){
        workspace_->main_window()->DisplayExceptionWarning(e);
    }

    ahca_results->AddMatrix("Assignments", assignments);
    ahca_results->AddMatrix("Spectrum Distances", ahca.dist());
    ahca_results->AddMatrix("Cluster Distances", ahca.merge_data());
    QStringList matrix_keys = QStringList({"Assignments"});
    AddResults(ahca_results, matrix_keys);
}

void MetaAnalyzer::GetData()
{
    if (transpose_){
        data_ = workspace_->GetMatrix(data_keys_).t();
        abscissa_ = linspace(0, data_.n_rows - 1, data_.n_rows);
    }
    else{
        data_ = workspace_->GetMatrix(data_keys_);
        abscissa_ = linspace(0, data_.n_cols - 1, data_.n_cols);
    }
}

QString MetaAnalyzer::FindUniqueName(QString name)
{
    if (data_keys_.size() > 2){
        QSharedPointer<AnalysisResults> results = workspace_->GetAnalysisResults(data_keys_[0], data_keys_[1]);
        QMap<QString, uvec> parent_rows = results->parent_rows();
        QString new_name = name;
        QStringList results_names;
        int i = 0;
        for (auto dataset_key: parent_rows.keys())
            results_names = results_names + workspace_->GetDataset(dataset_key)->AnalysisResultsKeys();
        while (results_names.contains(new_name))
            new_name = name + " (" + QString::number(++i) + ")";
        return new_name;
    }
    else return name; //unique name resolved by VespucciDataset::AddAnalysisResult
}

void MetaAnalyzer::AddResults(QSharedPointer<AnalysisResults> results, QStringList matrices)
{
    QString first_dataset_key = data_keys_.first();
    QSharedPointer<VespucciDataset> first_dataset = workspace_->GetDataset(first_dataset_key);
    if (data_keys_.size() < 3) first_dataset->AddAnalysisResult(results);
    else{
        QSharedPointer<AnalysisResults> parent_results = workspace_->GetAnalysisResults(data_keys_[0], data_keys_[1]);
        QMap<QString, uvec> parent_rows = parent_results->parent_rows();
        if (parent_rows.size() < 2){
            first_dataset->AddAnalysisResult(results);
            return;
        }
        else{
            for (auto key: parent_rows.keys()){
                QSharedPointer<VespucciDataset> current_dataset = workspace_->GetDataset(key);
                current_dataset->AddAnalysisResult(results);
                if (!matrices.isEmpty())
                    current_dataset->AddAnalysisResult(results->Subset(matrices, parent_rows[key](0), parent_rows[key](1)));
            }
        }
    }
}
