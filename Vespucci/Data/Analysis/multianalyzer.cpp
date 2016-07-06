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
#include "multianalyzer.h"
#include "Global/vespucciworkspace.h"
#include "Data/Analysis/vcadata.h"
#include "Data/Analysis/principalcomponentsdata.h"
#include "Data/Analysis/mlpackpcadata.h"
#include "Data/Analysis/plsdata.h"
#include <mlpack/methods/kmeans/kmeans.hpp>
MultiAnalyzer::MultiAnalyzer(QSharedPointer<VespucciWorkspace> workspace)
{
    workspace_ = workspace;
}

///
/// \brief MultiAnalyzer::VertexComponents
/// \param keys
/// \param name
/// \param endmembers
///
void MultiAnalyzer::VertexComponents(const QStringList &keys, const QString &name, uword endmembers)
{
    QList<QSharedPointer<VespucciDataset> > datasets = GetDatasets(keys);
    mat data;
    uvec start_indices, end_indices;
    ConcatenateSpectra(datasets, data, start_indices, end_indices);

    QSharedPointer<VCAData> vca_data(new VCAData(name));
    vca_data->Apply(data, endmembers);

    for (int i = 0; i < datasets.size(); ++i){
        datasets[i]->AddAnalysisResult(vca_data);
        mat scores = vca_data->GetMatrix("Fractional Abundances").rows(start_indices(i), end_indices(i));
        datasets[i]->AddAuxiliaryMatrix(name + ": Fractional Abundances", scores);
    }
}

///
/// \brief MultiAnalyzer::KMeans
/// \param keys
/// \param clusters
/// \param metric_text
/// \param name
///
void MultiAnalyzer::KMeans(const QStringList &keys, size_t clusters, const QString &metric_text, const QString &name)
{
    QList<QSharedPointer<VespucciDataset> > datasets = GetDatasets(keys);
    mat data;
    uvec start_indices, end_indices;
    ConcatenateSpectra(datasets, data, start_indices, end_indices);

    for (uword i = 1; i < start_indices.n_rows; ++ i){
        start_indices(i) = data.n_cols;
        data = join_horiz(data, datasets[i]->spectra_ref());
        end_indices(i) = data.n_cols - 1;
    }

    mat centroids;
    Row<size_t> assignments;
    QSharedPointer<AnalysisResults> km_results(new AnalysisResults(name, "k-Means Analysis"));

    if (metric_text == "Euclidean"){
        mlpack::kmeans::KMeans<mlpack::metric::EuclideanDistance> k;
        k.Cluster(data, clusters, assignments, centroids);
    }
    else if (metric_text == "Manhattan"){
        mlpack::kmeans::KMeans<mlpack::metric::EuclideanDistance> k;
        k.Cluster(data, clusters, assignments, centroids);
    }
    else if (metric_text == "Chebyshev"){
        mlpack::kmeans::KMeans<mlpack::metric::ChebyshevDistance> k;
        k.Cluster(data, clusters, assignments, centroids);
    }
    else{ //Default to squared euclidean distance as in chemometrics literature
        mlpack::kmeans::KMeans<mlpack::metric::SquaredEuclideanDistance> k;
        k.Cluster(data, clusters, assignments, centroids);
    }

    mat assignments_mat(assignments.n_cols, 1);
    for (uword i = 0; i < assignments.n_rows; ++i){
        assignments_mat(i, 0) = assignments(i) + 1.0;
    }

    km_results->AddMatrix("Assignments", assignments_mat);
    km_results->AddMatrix("Centroids", centroids);

    for (int i = 0; i < datasets.size(); ++i){
        datasets[i]->AddAnalysisResult(km_results);
        mat assignments = km_results->GetMatrix("Assignments").rows(start_indices(i), end_indices(i));
        datasets[i]->AddAuxiliaryMatrix(name + ": Assignments", assignments);
    }
}

///
/// \brief MultiAnalyzer::PrincipalComponents
/// \param keys
/// \param name
///
void MultiAnalyzer::PrincipalComponents(const QStringList &keys, const QString &name)
{
    QList<QSharedPointer<VespucciDataset> > datasets = GetDatasets(keys);
    mat data;
    uvec start_indices, end_indices;
    ConcatenateSpectra(datasets, data, start_indices, end_indices);

    for (uword i = 1; i < start_indices.n_rows; ++ i){
        start_indices(i) = data.n_cols;
        data = join_horiz(data, datasets[i]->spectra_ref());
        end_indices(i) = data.n_cols - 1;
    }

    QSharedPointer<PrincipalComponentsData> pca_data(new PrincipalComponentsData(name));
    pca_data->Apply(data);

    for (int i = 0; i < datasets.size(); ++i){
        datasets[i]->AddAnalysisResult(pca_data);
        mat scores = pca_data->GetMatrix("Scores").rows(start_indices(i), end_indices(i));
        datasets[i]->AddAuxiliaryMatrix(name + ": Scores", scores);
    }
}

///
/// \brief MultiAnalyzer::PrincipalComponents
/// \param keys
/// \param name
/// \param scale_data
///
void MultiAnalyzer::PrincipalComponents(const QStringList &keys, const QString &name, bool scale_data)
{
    QList<QSharedPointer<VespucciDataset> > datasets = GetDatasets(keys);

    mat data;
    uvec start_indices, end_indices;
    ConcatenateSpectra(datasets, data, start_indices, end_indices);

    for (uword i = 1; i < start_indices.n_rows; ++ i){
        start_indices(i) = data.n_cols;
        data = join_horiz(data, datasets[i]->spectra_ref());
        end_indices(i) = data.n_cols - 1;
    }

    QSharedPointer<MlpackPCAData> pca_data(new MlpackPCAData(name));
    pca_data->Apply(data, scale_data);

    for (int i = 0; i < datasets.size(); ++i){
        datasets[i]->AddAnalysisResult(pca_data);
    }
}

///
/// \brief MultiAnalyzer::PartialLeastSquares
/// \param keys
/// \param name
/// \param components
///
void MultiAnalyzer::PartialLeastSquares(const QStringList &keys, const QString &name, uword components)
{
    QList<QSharedPointer<VespucciDataset> > datasets = GetDatasets(keys);
    mat data;
    uvec start_indices, end_indices;
    ConcatenateSpectra(datasets, data, start_indices, end_indices);

    vec abscissa = datasets[0]->abscissa();
    QSharedPointer<PLSData> pls_data(new PLSData(name));
    pls_data->Classify(data, abscissa, components);

    for (int i = 0; i < datasets.size(); ++i){
        datasets[i]->AddAnalysisResult(pls_data);
        mat x_scores = pls_data->GetMatrix("Predictor Scores").rows(start_indices(i), end_indices(i));
        datasets[i]->AddAuxiliaryMatrix(name + ": Predictor Scores", x_scores);
        mat y_scores = pls_data->GetMatrix("Response Scores").rows(start_indices(i), end_indices(i));
        datasets[i]->AddAuxiliaryMatrix(name + ": Response Scores", y_scores);
    }
}

///
/// \brief MultiAnalyzer::CheckMergability
/// \param keys
/// \param problematic_keys
/// \return
/// Validates that all the keys exist and have the same number of rows in spectra_
bool MultiAnalyzer::CheckMergability(QStringList &keys, QStringList &problematic_keys)
{
    foreach(const QString &key, keys){
        if (!workspace_->dataset_names().contains(key))
            problematic_keys << key;
    }

    bool found = false;
    QString first_key;
    for (int i = 0; !found && i < keys.size(); ++i){
        if (workspace_->dataset_names().contains(keys[i])){
            first_key = keys[i];
            found = true;
        }
    }

    if (!found) return false;

    uword rows = workspace_->GetDataset(first_key)->spectra_ptr()->n_rows;
    foreach (const QString &key, keys){
        if (!problematic_keys.contains(key)){
            uword current_rows = workspace_->GetDataset(key)->spectra_ptr()->n_rows;
            if (current_rows != rows) problematic_keys << key;
        }
    }

    return !problematic_keys.size();
}

///
/// \brief MultiAnalyzer::GetDatasets
/// \param keys
/// \return
///
QList<QSharedPointer<VespucciDataset> > MultiAnalyzer::GetDatasets(QStringList keys)
{
    QList<QSharedPointer<VespucciDataset> > datasets;
    foreach (const QString &key, keys){
        if (workspace_->dataset_names().contains(key)){
            datasets.append(workspace_->GetDataset(key));
        }
    }
    return datasets;
}

void MultiAnalyzer::ConcatenateSpectra(QList<QSharedPointer<VespucciDataset> > datasets, mat &data, uvec &start_indices, uvec &end_indices)
{
    start_indices.set_size(datasets.size());
    end_indices.set_size(datasets.size());
    data = datasets[0]->spectra_ref();
    start_indices(0) = 0;
    end_indices(0) = datasets[0]->spectra_ref().n_cols - 1;

    for (uword i = 1; i < start_indices.n_rows; ++ i){
        start_indices(i) = data.n_cols;
        data = join_horiz(data, datasets[i]->spectra_ref());
        end_indices(i) = data.n_cols - 1;
    }
}
