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
#include "Data/Analysis/univariatedata.h"
#include <Math/Clustering/agglomerativeclustering.h>
MultiAnalyzer::MultiAnalyzer(QSharedPointer<VespucciWorkspace> workspace, QStringList dataset_keys)
{
    workspace_ = workspace;
    GetDatasets(dataset_keys);
    bool ok = ConcatenateSpectra(dataset_keys);
    if (!ok) throw runtime_error("Could not concatenate datasets");
}

void MultiAnalyzer::Univariate(const QString &name, double &left_bound, double &right_bound, uword bound_window)
{
    QString new_name = FindUniqueName(name);
    QSharedPointer<UnivariateData> univariate_data(new UnivariateData(new_name));
    univariate_data->Apply(left_bound, right_bound, bound_window, data_, abscissa_);
    QStringList matrix_keys = univariate_data->KeyList();
    AddAnalysisResults(univariate_data, matrix_keys);
}

void MultiAnalyzer::BandRatio(const QString &name, double &first_left_bound, double &first_right_bound, double &second_left_bound, double &second_right_bound, uword bound_window)
{
    QString new_name = FindUniqueName(name);
    QSharedPointer<UnivariateData> univariate_data(new UnivariateData(new_name));
    univariate_data->Apply(first_left_bound, first_right_bound,
                           second_left_bound, second_right_bound,
                           bound_window, data_, abscissa_);
    QStringList matrix_keys = univariate_data->KeyList();
    AddAnalysisResults(univariate_data, matrix_keys);
}

void MultiAnalyzer::ClassicalLeastSquares(const QString &name, const QStringList &reference_keys)
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
    coefs = coefs.t();
    cls_results->AddMatrix("Coefficients", coefs);
    AddAnalysisResults(cls_results, QStringList({"Coefficients"}));
}


///
/// \brief MultiAnalyzer::VertexComponents
/// \param keys
/// \param name
/// \param endmembers
///
void MultiAnalyzer::VertexComponents(const QString &name, uword endmembers)
{
    QString new_name = FindUniqueName(name);
    QSharedPointer<VCAData> vca_data(new VCAData(new_name));
    vca_data->Apply(data_, endmembers);
    QStringList matrix_keys({"Fractional Abundances"});
    AddAnalysisResults(vca_data, matrix_keys);
}

///
/// \brief MultiAnalyzer::KMeans
/// \param keys
/// \param clusters
/// \param metric_text
/// \param name
///
void MultiAnalyzer::KMeans(size_t clusters, const QString &metric_text, const QString &name)
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
    for (uword i = 0; i < assignments_mat.n_rows; ++i){
        assignments_mat(i, 0) = assignments(i) + 1.0;
    }

    km_results->AddMatrix("Assignments", assignments_mat);
    km_results->AddMatrix("Centroids", centroids);
    QStringList matrix_keys({"Assignments"});

    AddAnalysisResults(km_results, matrix_keys);
}

///
/// \brief MultiAnalyzer::PrincipalComponents
/// \param keys
/// \param name
///
void MultiAnalyzer::PrincipalComponents(const QString &name)
{
    QString new_name = FindUniqueName(name);
    QSharedPointer<PrincipalComponentsData> pca_data(new PrincipalComponentsData(new_name));
    pca_data->Apply(data_);
    QStringList matrix_keys({"Scores"});
    AddAnalysisResults(pca_data, matrix_keys);
}

///
/// \brief MultiAnalyzer::PrincipalComponents
/// \param keys
/// \param name
/// \param scale_data
///
void MultiAnalyzer::PrincipalComponents(const QString &name, bool scale_data)
{
    QString new_name = FindUniqueName(name);
    QSharedPointer<MlpackPCAData> pca_data(new MlpackPCAData(new_name));
    pca_data->Apply(data_, scale_data);
    AddAnalysisResults(pca_data, QStringList());
}

///
/// \brief MultiAnalyzer::PartialLeastSquares
/// \param keys
/// \param name
/// \param components
///
void MultiAnalyzer::PartialLeastSquares(const QString &name, uword components)
{
    QString new_name = FindUniqueName(name);
    QSharedPointer<PLSData> pls_data(new PLSData(new_name));
    pls_data->Classify(data_, abscissa_, components);
    QStringList matrices({"Predictor Scores", "Response Scores"});
    AddAnalysisResults(pls_data, matrices);
}

void MultiAnalyzer::PLSCalibration(const QString &name, const QStringList &control_keys)
{
    QString new_name = FindUniqueName(name);
    QSharedPointer<PLSData> pls_data(new PLSData(new_name));
    mat controls = workspace_->GetMatrix(control_keys);
    pls_data->Calibrate(data_, controls);
    QStringList matrices({"Predictor Scores", "Response Scores"});
    AddAnalysisResults(pls_data, matrices);
}

void MultiAnalyzer::TrainPLSDA(const QString &name, const QStringList &label_keys)
{
    QString new_name = FindUniqueName(name);
    QSharedPointer<PLSData> pls_data(new PLSData(new_name));
    mat labels = workspace_->GetMatrix(label_keys);
    pls_data->Discriminate(data_, labels);
    QStringList matrices({"Predictor Scores", "Response Scores"});
    AddAnalysisResults(pls_data, matrices);
}

void MultiAnalyzer::AgglomerativeClustering(const QString &name, const QString &metric, const QString &linkage)
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
    AddAnalysisResults(ahca_results, matrix_keys);
}

bool MultiAnalyzer::CheckMergability(const QStringList dataset_keys)
{
    for (auto key: dataset_keys)
        if (workspace_->GetDataset(key)->spectra_ptr()->n_rows
                != workspace_->GetDataset(dataset_keys.first())->spectra_ptr()->n_rows)
            return false;
    return true;
}

bool MultiAnalyzer::ConcatenateSpectra(QStringList dataset_keys)
{
    if (!CheckMergability(dataset_keys)) return false;

    try{
        start_indices_.set_size(datasets_.size());
        end_indices_.set_size(datasets_.size());
        data_ = datasets_[0]->spectra_ref();
        abscissa_ = datasets_[0]->abscissa_ref();
        start_indices_(0) = 0;
        end_indices_(0) = datasets_[0]->spectra_ref().n_cols - 1;

        for (uword i = 1; i < start_indices_.n_rows; ++ i){
            start_indices_(i) = data_.n_cols;
            data_ = join_horiz(data_, datasets_[i]->spectra_ref());
            end_indices_(i) = data_.n_cols - 1;
        }
    }catch(...){
        return false;
    }

    return true;
}

void MultiAnalyzer::SNVNormalize(double offset)
{
   data_ = Vespucci::Math::Normalization::SNVNorm(data_, offset, true);
   for (uword i = 0; i < start_indices_.n_rows; ++i){
       mat spectra = data_.cols(start_indices_(i), end_indices_(i));
       vec abscissa = datasets_[i]->abscissa();
       vec x = datasets_[i]->x();
       vec y = datasets_[i]->y();
       datasets_[i]->SetData(spectra, abscissa, x, y);
   }
}

///
///
/// \brief MultiAnalyzer::GetDatasets
/// \param keys
/// \return
///
void MultiAnalyzer::GetDatasets(QStringList keys)
{
    for (auto key: keys){
        if (workspace_->dataset_names().contains(key)){
            datasets_.append(workspace_->GetDataset(key));
        }
    }
}

void MultiAnalyzer::AddAnalysisResults(QSharedPointer<AnalysisResults> results, QStringList matrices)
{
    for (uword i = 0; i < datasets_.size(); ++i){
        datasets_[i]->AddAnalysisResult(results, start_indices_(i), end_indices_(i));
        if (!matrices.isEmpty())
            datasets_[i]->AddAnalysisResult(results->Subset(matrices, start_indices_(i), end_indices_(i)));
    }
}

QString MultiAnalyzer::FindUniqueName(QString name)
{
    QString new_name = name;
    QStringList results_names;
    for (auto dataset: datasets_){
        results_names = results_names + dataset->AnalysisResultsKeys();
    }
    int i = 0;
    QString basename = name;
    while (results_names.contains(new_name)){
        new_name = basename + " (" + QString::number(i++) + ")";
    }
    return new_name;
}
