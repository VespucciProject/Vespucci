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
#include "Data/Analysis/matrixanalyzer.h"

MatrixAnalyzer::MatrixAnalyzer(QSharedPointer<VespucciWorkspace> ws, const QStringList &data_keys, bool transpose)
    :MetaAnalyzer(ws), data_keys_(data_keys)
{
    transpose_ = transpose;
    GetData();
}

MatrixAnalyzer::~MatrixAnalyzer(){}

void MatrixAnalyzer::GetData()
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

QString MatrixAnalyzer::FindUniqueName(QString name)
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

void MatrixAnalyzer::AddAnalysisResults(QSharedPointer<AnalysisResults> results, QStringList matrices)
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
