/*******************************************************************************
    Copyright (C) 2014-2017 Wright State University - All Rights Reserved
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
#include "Data/Analysis/multianalyzer.h"
#include <Math/Smoothing/denoise.h>
MultiAnalyzer::MultiAnalyzer(QSharedPointer<VespucciWorkspace> workspace, QStringList dataset_keys)
    : MetaAnalyzer(workspace), dataset_keys_(dataset_keys)
{
     GetData();
}

MultiAnalyzer::~MultiAnalyzer(){}

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
void MultiAnalyzer::QUIC_SVD(double epsilon)
{
    uword rank;
    mat U, V;
    vec s;
    data_ = Vespucci::Math::Smoothing::QUICSVDDenoise(data_, epsilon, U, s, V, rank);
    vec SVD_vec({double(rank)});

    QSharedPointer<AnalysisResults> results(new AnalysisResults("QUIC-SVD", "QUIC-SVD"));
    results->AddMatrix("Left Singualr Vectors", U);
    results->AddMatrix("Right Singualr Vectors", V);
    results->AddMatrix("Singualr Values", s);
    results->AddMatrix("Rank", SVD_vec);

    for (uword i = 0; i < start_indices_.n_rows; ++i){
       mat spectra = data_.cols(start_indices_(i), end_indices_(i));
       vec abscissa = datasets_[i]->abscissa();
       vec x = datasets_[i]->x();
       vec y = datasets_[i]->y();
       datasets_[i]->SetData(spectra, abscissa, x, y);
    }
    AddAnalysisResults(results, QStringList());
}
void MultiAnalyzer::SVDDenoise(uword k)
{
    mat U, V;
    vec s;
    data_ = Vespucci::Math::Smoothing::SVDDenoise(data_, k, U, s, V);
    vec SVD_vec({double(k)});
    QSharedPointer<AnalysisResults> results(new AnalysisResults("QUIC-SVD", "QUIC-SVD"));
    results->AddMatrix("Left Singualr Vectors", U);
    results->AddMatrix("Right Singualr Vectors", V);
    results->AddMatrix("Singualr Values", s);
    results->AddMatrix("Rank", SVD_vec);
    for (uword i = 0; i < start_indices_.n_rows; ++i){
       mat spectra = data_.cols(start_indices_(i), end_indices_(i));
       vec abscissa = datasets_[i]->abscissa();
       vec x = datasets_[i]->x();
       vec y = datasets_[i]->y();
       datasets_[i]->SetData(spectra, abscissa, x, y);
    }

    AddAnalysisResults(results, QStringList());
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

///
/// \brief MultiAnalyzer::GetData
/// Throws exception if datasets cannot be concatenated
void MultiAnalyzer::GetData()
{
    GetDatasets(dataset_keys_);
    bool ok = ConcatenateSpectra(dataset_keys_);
    if (!ok) throw runtime_error("Could not concatenate datasets");
}

void MultiAnalyzer::AddAnalysisResults(QSharedPointer<AnalysisResults> results, QStringList matrices)
{
    for (uword i = 0; i < uword(datasets_.size()); ++i){
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
