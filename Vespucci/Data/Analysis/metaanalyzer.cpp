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
#include "Data/Analysis/metaanalyzer.h"
#include "Data/Analysis/vcadata.h"
#include "Data/Analysis/principalcomponentsdata.h"
#include "Data/Analysis/mlpackpcadata.h"
#include "Data/Analysis/plsdata.h"
#include <mlpack/methods/kmeans/kmeans.hpp>
#include "Data/Analysis/univariatedata.h"
#include <Math/Clustering/agglomerativeclustering.h>
#include <Math/Clustering/kmeanswrapper.h>
MetaAnalyzer::MetaAnalyzer(QSharedPointer<VespucciWorkspace> ws)
{
    workspace_ = ws;
}

MetaAnalyzer::~MetaAnalyzer()
{

}

void MetaAnalyzer::Univariate(const QString &name, double &left_bound, double &right_bound, arma::uword bound_window)
{
    QString new_name = FindUniqueName(name);
    QSharedPointer<UnivariateData> univariate_data(new UnivariateData(new_name));
    univariate_data->Apply(left_bound, right_bound, bound_window, data_, abscissa_);
    QStringList matrix_keys({"Peak Centers",
                             "Peak Intensities",
                             "Adjusted Peak Intensities",
                             "Total Area",
                             "Adjusted Area",
                             "Area Between Inflection Points",
                             "Adjusted Area Between Inflection Points",
                             "Empirical Full-Width at Half-Maximum"});
    AddAnalysisResults(univariate_data, matrix_keys);
}

void MetaAnalyzer::FitPeak(const QString &name, const QString &peak_shape, double &left_bound, double &right_bound)
{
    QSharedPointer<UnivariateData> univariate_data(new UnivariateData(name));
    univariate_data->Apply(peak_shape, left_bound, right_bound, data_, abscissa_);
    QStringList matrix_keys;
    if (peak_shape == "Voigt"){
        matrix_keys = QStringList({"Intensity",
                                   "Area",
                                   "Gaussian Full-Width at Half Maximum",
                                   "Lorentzian Full-Width at Half Maximum",
                                   "Full-Width at Half Maximum",
                                   "Peak Centers",
                                   "R²"});
    }
    else{
        matrix_keys = QStringList({"Intensity",
                                   "Area",
                                   "Full-Width at Half Maximum",
                                   "Peak Centers",
                                   "R²"});
    }
    AddAnalysisResults(univariate_data, matrix_keys);
}

void MetaAnalyzer::BandRatio(const QString &name,
                             double &first_left_bound,
                             double &first_right_bound,
                             double &second_left_bound,
                             double &second_right_bound,
                             arma::uword bound_window)
{
    QString new_name = FindUniqueName(name);
    QSharedPointer<UnivariateData> univariate_data(new UnivariateData(new_name));
    univariate_data->Apply(first_left_bound, first_right_bound,
                           second_left_bound, second_right_bound,
                           bound_window, data_, abscissa_);
    QStringList matrix_keys({"Band Ratios"});
    AddAnalysisResults(univariate_data, matrix_keys);
}

void MetaAnalyzer::ClassicalLeastSquares(const QString &name, const QStringList &reference_keys)
{
    QString new_name = FindUniqueName(name);
    QSharedPointer<AnalysisResults> cls_results(new AnalysisResults(new_name, "CLS Analysis"));
    mat reference = workspace_->GetMatrix(reference_keys);
    mat coefs = Vespucci::Math::LinLeastSq::OrdinaryLeastSquares(reference, data_).t();
    cls_results->AddMatrix("Coefficients", coefs.t());
    AddAnalysisResults(cls_results, QStringList({"Coefficients"}));
}

void MetaAnalyzer::VertexComponents(const QString &name, uword endmembers)
{
    QString new_name = FindUniqueName(name);
    QSharedPointer<VCAData> vca_data(new VCAData(new_name));
    vca_data->Apply(data_, endmembers);
    QStringList matrix_keys({"Fractional Abundances"});
    AddAnalysisResults(vca_data, matrix_keys);
}

void MetaAnalyzer::KMeans(const QString &name,
                          const QString &metric_text, const QString &partition_policy, bool allow_empty,
                          size_t clusters)
{
    QString new_name = FindUniqueName(name);
    mat centroids;
    Vespucci::Math::KMeansWrapper k(partition_policy.toStdString(), metric_text.toStdString(), allow_empty);
    vec assignments = k.Cluster(data_, clusters, centroids);

    QSharedPointer<AnalysisResults> km_results(new AnalysisResults(new_name, "k-Means Analysis"));
    km_results->AddMatrix("Assignments", assignments);
    km_results->AddMatrix("Centroids", centroids);
    QStringList matrix_keys({"Assignments"});

    AddAnalysisResults(km_results, matrix_keys);
}

void MetaAnalyzer::PrincipalComponents(const QString &name)
{
    QString new_name = FindUniqueName(name);
    QSharedPointer<PrincipalComponentsData> pca_data(new PrincipalComponentsData(new_name));
    pca_data->Apply(data_);
    QStringList matrix_keys({"Scores", "Hotelling t²"});
    AddAnalysisResults(pca_data, matrix_keys);
}

void MetaAnalyzer::PrincipalComponents(const QString &name, bool scale_data)
{
    QString new_name = FindUniqueName(name);
    QSharedPointer<MlpackPCAData> pca_data(new MlpackPCAData(new_name));
    pca_data->Apply(data_, scale_data);
    AddAnalysisResults(pca_data, QStringList());
}

void MetaAnalyzer::PartialLeastSquares(const QString &name, uword components)
{
    QString new_name = FindUniqueName(name);
    QSharedPointer<PLSData> pls_data(new PLSData(new_name));
    pls_data->Classify(data_, abscissa_, components);
    QStringList matrices({"Predictor Scores", "Response Scores"});
    AddAnalysisResults(pls_data, matrices);
}

void MetaAnalyzer::PLSCalibration(const QString &name, const QStringList &control_keys)
{
    QString new_name = FindUniqueName(name);
    QSharedPointer<PLSData> pls_data(new PLSData(new_name));
    mat controls = workspace_->GetMatrix(control_keys);
    pls_data->Calibrate(data_, controls);
    QStringList matrices({"Predictor Scores", "Response Scores"});
    AddAnalysisResults(pls_data, matrices);
}

void MetaAnalyzer::TrainPLSDA(const QString &name, const QStringList &label_keys)
{
    QString new_name = FindUniqueName(name);
    QSharedPointer<PLSData> pls_data(new PLSData(new_name));
    mat labels = workspace_->GetMatrix(label_keys);
    pls_data->Discriminate(data_, labels);
    QStringList matrices({"Predictor Scores", "Response Scores"});
    AddAnalysisResults(pls_data, matrices);
}

void MetaAnalyzer::AgglomerativeClustering(const QString &name, const QString &metric, const QString &linkage)
{
    QSharedPointer<AnalysisResults> ahca_results(new AnalysisResults(name, "AHCA"));
    mat assignments;
    Vespucci::Math::Clustering::AHCA ahca;
    ahca.SetLinkage(linkage.toStdString());
    ahca.SetMetric(metric.toStdString());
    ahca.Link(data_);
    assignments = ahca.Cluster(data_.n_cols);

    ahca_results->AddMatrix("Assignments", assignments);
    ahca_results->AddMatrix("Spectrum Distances", ahca.dist());
    ahca_results->AddMatrix("Cluster Distances", ahca.merge_data());
    QStringList matrix_keys = QStringList({"Assignments"});
    AddAnalysisResults(ahca_results, matrix_keys);
}

size_t MetaAnalyzer::columns() const
{
    return data_.n_cols;
}

double MetaAnalyzer::AbscissaMin() const
{
    return abscissa_.min();
}

double MetaAnalyzer::AbscissaMax() const
{
    return abscissa_.max();
}

arma::vec MetaAnalyzer::abscissa() const
{
    return abscissa_;
}

arma::vec MetaAnalyzer::PointSpectrum(arma::uword index) const
{
    return data_.col((index < data_.n_cols ? index : data_.n_cols - 1));
}
