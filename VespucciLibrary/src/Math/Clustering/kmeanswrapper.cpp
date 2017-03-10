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
#include "include/Math/Clustering/kmeanswrapper.h"
#include <mlpack/methods/kmeans/kmeans.hpp>
#include <mlpack/methods/kmeans/random_partition.hpp>
#include <mlpack/methods/kmeans/refined_start.hpp>
#include <mlpack/methods/kmeans/allow_empty_clusters.hpp>
Vespucci::Math::KMeansWrapper::KMeansWrapper(const std::string &init, const std::string &metric, bool allow_empty)
    :init_(init), metric_(metric)
{
    allow_empty_ = allow_empty;
    if ((init_ != "sampleinitialization")
    && (init_ != "randompartition")
     && (init_ != "refinedstart")){
       throw std::invalid_argument("invalid initialization policy");}

    if ((metric_ != "squaredeuclidean")
     && (metric_ != "euclidean")
     && (metric_ != "manhattan")
     && (metric_ != "chebyshev")){
        throw std::invalid_argument("invalid distance metric");
    }
}

Vespucci::Math::KMeansWrapper::~KMeansWrapper()
{

}

arma::vec Vespucci::Math::KMeansWrapper::Cluster(const arma::mat &data, const size_t clusters, arma::mat &centroids)
{
    using namespace mlpack::metric;
    using namespace mlpack::kmeans;
    arma::Row<size_t> assignments;
    arma::vec assignments_vec;
    if (allow_empty_){
        if (metric_ == "squaredeuclidean"){
            if (init_ == "sampleinitialization"){
                KMeans<SquaredEuclideanDistance, SampleInitialization, AllowEmptyClusters> k;
                k.Cluster(data, clusters, assignments, centroids);
            }
            else if (init_ == "randompartition"){
                KMeans<SquaredEuclideanDistance, RandomPartition, AllowEmptyClusters> k;
                k.Cluster(data, clusters, assignments, centroids);
            }
            else if (init_ == "refinedstart"){
                KMeans<SquaredEuclideanDistance, RefinedStart, AllowEmptyClusters> k;
                k.Cluster(data, clusters, assignments, centroids);
            }
        }
        else if (metric_ == "euclidean"){
            if (init_ == "sampleinitialization"){
                KMeans<EuclideanDistance, SampleInitialization, AllowEmptyClusters> k;
                k.Cluster(data, clusters, assignments, centroids);
            }
            else if (init_ == "randompartition"){
                KMeans<EuclideanDistance, RandomPartition, AllowEmptyClusters> k;
                k.Cluster(data, clusters, assignments, centroids);
            }
            else if (init_ == "refinedstart"){
                KMeans<EuclideanDistance, RefinedStart, AllowEmptyClusters> k;
                k.Cluster(data, clusters, assignments, centroids);
            }
        }
        else if (metric_ == "manhattan"){
            if (init_ == "sampleinitialization"){
                KMeans<ManhattanDistance, SampleInitialization, AllowEmptyClusters> k;
                k.Cluster(data, clusters, assignments, centroids);
            }
            else if (init_ == "randompartition"){
                KMeans<ManhattanDistance, RandomPartition, AllowEmptyClusters> k;
                k.Cluster(data, clusters, assignments, centroids);
            }
            else if (init_ == "refinedstart"){
                KMeans<ManhattanDistance, RefinedStart, AllowEmptyClusters> k;
                k.Cluster(data, clusters, assignments, centroids);
            }
        }
        else if (metric_ == "chebyshev"){
            if (init_ == "sampleinitialization"){
                KMeans<ChebyshevDistance, SampleInitialization, AllowEmptyClusters> k;
                k.Cluster(data, clusters, assignments, centroids);
            }
            else if (init_ == "randompartition"){
                KMeans<ChebyshevDistance, RandomPartition, AllowEmptyClusters> k;
                k.Cluster(data, clusters, assignments, centroids);
            }
            else if (init_ == "refinedstart"){
                KMeans<ChebyshevDistance, RefinedStart, AllowEmptyClusters> k;
                k.Cluster(data, clusters, assignments, centroids);
            }
        }
    }
    else{
        if (metric_ == "squaredeuclidean"){
            if (init_ == "sampleinitialization"){
                KMeans<SquaredEuclideanDistance, SampleInitialization> k;
                k.Cluster(data, clusters, assignments, centroids);
            }
            else if (init_ == "randompartition"){
                KMeans<SquaredEuclideanDistance, RandomPartition> k;
                k.Cluster(data, clusters, assignments, centroids);
            }
            else if (init_ == "refinedstart"){
                KMeans<SquaredEuclideanDistance, RefinedStart> k;
                k.Cluster(data, clusters, assignments, centroids);
            }
        }
        else if (metric_ == "euclidean"){
            if (init_ == "sampleinitialization"){
                KMeans<EuclideanDistance, SampleInitialization> k;
                k.Cluster(data, clusters, assignments, centroids);
            }
            else if (init_ == "randompartition"){
                KMeans<EuclideanDistance, RandomPartition> k;
                k.Cluster(data, clusters, assignments, centroids);
            }
            else if (init_ == "refinedstart"){
                KMeans<EuclideanDistance, RefinedStart> k;
                k.Cluster(data, clusters, assignments, centroids);
            }
        }
        else if (metric_ == "manhattan"){
            if (init_ == "sampleinitialization"){
                KMeans<ManhattanDistance, SampleInitialization> k;
                k.Cluster(data, clusters, assignments, centroids);
            }
            else if (init_ == "randompartition"){
                KMeans<ManhattanDistance, RandomPartition> k;
                k.Cluster(data, clusters, assignments, centroids);
            }
            else if (init_ == "refinedstart"){
                KMeans<ManhattanDistance, RefinedStart> k;
                k.Cluster(data, clusters, assignments, centroids);
            }
        }
        else if (metric_ == "chebyshev"){
            if (init_ == "sampleinitialization"){
                KMeans<ChebyshevDistance, SampleInitialization> k;
                k.Cluster(data, clusters, assignments, centroids);
            }
            else if (init_ == "randompartition"){
                KMeans<ChebyshevDistance, RandomPartition> k;
                k.Cluster(data, clusters, assignments, centroids);
            }
            else if (init_ == "refinedstart"){
                KMeans<ChebyshevDistance, RefinedStart> k;
                k.Cluster(data, clusters, assignments, centroids);
            }
        }
    }

    assignments_vec.set_size(assignments.n_elem);
    for (arma::uword i = 0; i < assignments.n_elem; ++i)
        assignments_vec(i) = double(assignments(i) + 1);
    return assignments_vec;
}
