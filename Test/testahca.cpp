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
#include <QObject>
#include <QtTest>
#include "Math/Clustering/agglomerativeclustering.h"
#include "testahca.h"
TestAHCA::TestAHCA(QObject *parent)
    : QObject(parent)
{
    std::vector<std::string> metrics= {"euclidean",
                                       "squaredeuclidean",
                                       "manhattan",
                                       "chebyshev",
                                       "cosine",
                                       "correlation"};
    std::vector<std::string> linkages = {"average",
                                         "centroid",
                                         "ward",
                                         "complete",
                                         "single"};
    for (auto linkage: linkages)
        for (auto metric: metrics)
            ahca_obj.push_back(Vespucci::Math::Clustering::AHCA(linkage, metric));
    arma::mat data = arma::randu<arma::mat>(1024, 50);
    for (auto ahca: ahca_obj){
        ahca.Link(data);
    }
}

///
/// \brief TestAHCA::Cluster
/// Guarantee that all clustering assignments are valid (every
void TestAHCA::testCompleteClusters()
{
    for (auto ahca: ahca_obj){
        arma::mat assignments = ahca.Cluster(15);
        for (arma::uword i = 0; i < assignments.n_cols; ++i){
            arma::uvec unique_values = arma::find_unique(assignments.col(i));
            QCOMPARE((i + 1), unique_values.n_elem);
        }
    }
}

///
/// \brief TestAHCA::ExclusiveClusters
/// Verify that all clusters within a clustering arrangement have unique indices
void TestAHCA::testExclusiveClusters()
{
    for (auto ahca: ahca_obj){
        std::map<size_t, nodevec> clusters = ahca.clusters();
        arma::uvec indices;
        for (size_t i = 1; i < 15; ++i){
            nodevec current_clusters = clusters[i];
            for (auto current_cluster: current_clusters){
                if (!indices.n_rows)
                    indices = current_cluster->GetChildIndices();
                else
                    indices = arma::join_vert(indices, current_cluster->GetChildIndices());
            }
            arma::uvec unique_values = arma::find_unique(indices);
            arma::uword unique_count = unique_values.n_rows;
            QCOMPARE(unique_count, indices.n_rows);
        }
    }
}
