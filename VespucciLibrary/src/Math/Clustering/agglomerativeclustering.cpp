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
#include "Math/Clustering/agglomerativeclustering.h"
#include <mlpack/core/metrics/mahalanobis_distance.hpp>
#include "Math/Accessory/distancemetricwrapper.h"

///
/// \brief Vespucci::Math::Clustering::AHCA::AHCATree
/// \param linkage_method
/// \param metric_type
///
/// Valid parameters:
/// linkage_method:
///     "average" - average distance
///     "centroid" - centroid distance should only be used with 'squaredeuclidean' or 'euclidean' distance metric
///     "ward" - inner squared distance, should only be used with euclidean distance metrics
///     "complete" - greatest distance between points
///     "single" - least distance between points
/// metric_type:
///     "euclidean"
///     "squaredeuclidean"
///     "manhattan"
///     "chebyshev"
///     "cosine"
///     "correlation"
///
/// This will throw std::invalid_argument if invalid linkage or metric provided
/// If "ward" is selected as linkage method, the metric_type argument is ignored.
/// Squared euclidean distance will be used.
///
/// If "ward" is seleced, the distance between steps is stored as the variance of the cluster.
Vespucci::Math::Clustering::AHCA::AHCA(std::string linkage_method, std::string metric_type)
    : metric_type_(metric_type), linkage_method_(linkage_method)
{
    std::set<std::string> valid_metrics = {"euclidean",
                                           "squaredeuclidean",
                                           "manhattan",
                                           "chebyshev",
                                           "cosine",
                                           "correlation"};
    std::set<std::string> valid_linkages = {"average",
                                            "centroid",
                                            "ward",
                                            "complete",
                                            "single"};
    if (!valid_metrics.count(metric_type))
        throw std::invalid_argument("Unsupported metric type " + metric_type);
    if (!valid_linkages.count(linkage_method))
        throw std::invalid_argument("Unsupported linkage method " + linkage_method);
    if (linkage_method_ == "ward" && metric_type_ != "squaredeuclidean")
        std::cerr << "Using Ward's method requires squared Euclidean distance."
                     "Squared Euclidean distance will be used in lieu of " + metric_type_ + "\n";
}

Vespucci::Math::Clustering::AHCA::AHCA()
{

}

///
/// \brief Vespucci::Math::Clustering::AHCA::Link
/// \param data
/// Perform the linkage to build the AHCA tree
void Vespucci::Math::Clustering::AHCA::Link(const arma::mat &data)
{
    std::cout << "Begin AHCA::Link()" << std::endl;
    arma::wall_clock timer;
    timer.tic();
    observations_ = data.n_cols;
    merge_data_ = arma::mat(observations_, 2);
    merge_data_.col(0) = arma::linspace(1, observations_, observations_);

    // The distance between each pair of points
    if (linkage_method_ == "ward")
        dist_ = Vespucci::Math::Clustering::pdist(data, "squaredeuclidean", 1, "square");
    else
        dist_ = Vespucci::Math::Clustering::pdist(data, metric_type_, 1, "square");

    //clusters holds the clustering nodes at this level
    //we start with every observation in its own node
    nodevec nodes; //clusters at this level
    for (arma::uword i = 0; i < data.n_cols; ++i)
        nodes.push_back(new AHCALeaf(i));
    while (nodes.size() > 1){
        clusters_[nodes.size()] = nodes;
        //find the pairs with the closest distance in clusters
        double distance;
        arma::uvec to_merge = FindClosest(data, dist_, nodes, distance);
        merge_data_(nodes.size() - 1, 1) = distance;
        //create a new node in the list with two child nodes
        node_t *node = new node_t(nodes[to_merge(0)], nodes[to_merge(1)], distance);
        merge_data_(nodes.size() - 1, 1) = distance;
        if (to_merge(0) > to_merge(1)){
            nodes.erase(nodes.begin() + to_merge(0));
            nodes.erase(nodes.begin() + to_merge(1));
        }
        else{
            nodes.erase(nodes.begin() + to_merge(1));
            nodes.erase(nodes.begin() + to_merge(0));
        }

        nodes.push_back(node); //add merged cluster to vector and proceed
    }
    root_node_ = nodes[0]; //there is only one cluster left at this point
    clusters_[1] = nodes;
    merge_data_.shed_row(0);
    std::cout << "End of AHCA::Link()" << std::endl;
    std::cout << "took " << timer.toc() << " s" << std::endl;
}

void Vespucci::Math::Clustering::AHCA::SetLinkage(std::string linkage_method)
{
    std::set<std::string> valid_linkages = {"average",
                                            "centroid",
                                            "ward",
                                            "complete",
                                            "single"};
    if (!valid_linkages.count(linkage_method))
        throw std::invalid_argument("Unsupported linkage method " + linkage_method);
    linkage_method_ = linkage_method_;
}

void Vespucci::Math::Clustering::AHCA::SetMetric(std::string metric_type)
{
    std::set<std::string> valid_metrics = {"euclidean",
                                           "squaredeuclidean",
                                           "manhattan",
                                           "chebyshev",
                                           "cosine",
                                           "correlation"};
    if (!valid_metrics.count(metric_type))
        throw std::invalid_argument("Unsupported metric type " + metric_type);
    if (linkage_method_ == "ward" && metric_type_ != "squaredeuclidean")
        std::cerr << "Using Ward's method requires squared Euclidean distance."
                "Squared Euclidean distance will be used in lieu of " + metric_type_ + "\n";
    metric_type_ = metric_type;
}

///
/// \brief Vespucci::Math::Clustering::AHCA::Cluster Generate cluster assignments from tree
/// \param k Number of clusters to find
/// \return matrix with assignments in columns
/// The linkage process will be repeated, but at a higher tree level with precalculated distances.
arma::mat Vespucci::Math::Clustering::AHCA::Cluster(arma::uword k)
{
    std::cout << "Begin AHCA::Cluster()" << std::endl;
    arma::wall_clock timer;
    timer.tic();
    k = (k > clusters_.size() ? clusters_.size() : k);
    arma::mat assignments = arma::zeros(dist_.n_rows, k);
    for (arma::uword i = 1; i <= k; ++i){
        nodevec cluster = clusters_.at(i);
        for (arma::uword j = 0; j < cluster.size(); ++j){
            arma::uvec column = {i - 1};
            arma::uvec rows = cluster[j]->GetChildIndices();
            assignments(rows, column).fill(j+1);
        }
    }
    std::cout << "End AHCA::Cluster()" << std::endl;
    std::cout << "Took " << timer.toc() << " s" << std::endl;
    return assignments;
}

///
/// \brief Vespucci::Math::Clustering::AHCA::metric_type
/// \return
/// Access to name of metric
std::string Vespucci::Math::Clustering::AHCA::metric_type()
{
    return metric_type_;
}

///
/// \brief Vespucci::Math::Clustering::AHCA::linkage_method
/// \return
/// Access to name of linkage method
std::string Vespucci::Math::Clustering::AHCA::linkage_method()
{
    return linkage_method_;
}

arma::mat Vespucci::Math::Clustering::AHCA::merge_data()
{
    return merge_data_;
}

arma::mat Vespucci::Math::Clustering::AHCA::dist()
{
    return dist_;
}

std::map<size_t, nodevec> Vespucci::Math::Clustering::AHCA::clusters()
{
    return clusters_;
}

///
/// \brief Vespucci::Math::Clustering::AHCA::FindClosest
/// \param data
/// \param dist
/// \param clusters
/// \return
/// Search for distance between pairs is parallelized
arma::uvec Vespucci::Math::Clustering::AHCA::FindClosest(const arma::mat &data,
                                                         const arma::mat &dist,
                                                         const nodevec &clusters,
                                                         double &distance)
{
    if (clusters.size() < 2) throw std::invalid_argument("Only one cluster to merge");
    arma::mat cluster_dist(clusters.size(), clusters.size());
    for (size_t i = 0; i < clusters.size(); ++i){
        #ifdef _WIN32
        #pragma omp parallel for default(none) \
        shared(cluster_dist, clusters, i, data, dist)
        for (intmax_t j = 0; j < (intmax_t) clusters.size(); ++j)
        #else
        #pragma omp parallel for default(none) \
        shared(cluster_dist, clusters, i, data, dist)
        for (size_t j = 0; j < clusters.size(); ++ j)
        #endif
        {
            cluster_dist(i, j) = ClusterDistance(i, j, data, dist, clusters);
            cluster_dist(j, i) = ClusterDistance(i, j, data, dist, clusters);
        }
    }
    //to prevent the trivial case of a cluster merging with itself, we fill
    //the (i,i) pairs with the maxium value;
    cluster_dist.diag(0).fill(cluster_dist.max());

    arma::uword min_index = cluster_dist.index_min();
    distance = cluster_dist.min();
    //if the total number of clusters is 2, then the matrix is filled with one value
    if (clusters.size() == 2) return arma::uvec({0, 1});
    return arma::ind2sub(arma::size(cluster_dist), min_index);
}

///
/// \brief Vespucci::Math::Clustering::AHCA::ClusterDistance
/// \param i
/// \param j
/// \param data
/// \param dist
/// \param clusters
/// \return Linkage criteria (a distance for all metrics but Ward).
double Vespucci::Math::Clustering::AHCA::ClusterDistance(int i, int j,
                                                         const arma::mat &data,
                                                         const arma::mat &dist,
                                                         const nodevec &clusters)
{
    arma::uvec I = clusters[i]->GetChildIndices();
    arma::uvec J = clusters[j]->GetChildIndices();
    arma::mat distances = dist.submat(I, J);
    if (linkage_method_ == "single") return distances.min();
    if (linkage_method_ == "complete") return distances.max();
    if (linkage_method_ == "average") return arma::mean(mean(distances));
    if (linkage_method_ == "ward"){
        arma::vec i_centroid = arma::mean(data.cols(I), 1);
        arma::vec j_centroid = arma::mean(data.cols(J), 1);
        Vespucci::Math::DistanceMetricWrapper metric(metric_type_);
        return (metric.Evaluate(i_centroid, j_centroid) * I.n_rows * J.n_rows)/(I.n_rows + J.n_rows);
    }
    if (linkage_method_ == "centroid"){
        arma::vec i_centroid = arma::mean(data.cols(I), 1);
        arma::vec j_centroid = arma::mean(data.cols(J), 1);
        Vespucci::Math::DistanceMetricWrapper metric(metric_type_);
        return metric.Evaluate(i_centroid, j_centroid);
    }
    else
        return distances.min(); //defaulting to single. Unlikely to occur if this is used properly.
}

arma::mat Vespucci::Math::Clustering::pdist(const arma::mat &x,
                                            std::string metric_type,
                                            arma::uword dim,
                                            std::string shape)
{
    if (dim > 1) throw std::invalid_argument("dim must be 0 or 1");
    std::set<std::string> valid_types = {"euclidean",
                                         "squaredeuclidean",
                                         "manhattan",
                                         "chebyshev",
                                         "cosine",
                                         "correlation"};
    if (!valid_types.count(metric_type))
        throw std::invalid_argument("Unsupported metric type " + metric_type);

    Vespucci::Math::DistanceMetricWrapper metric(metric_type);
    arma::uword size = (dim == 1 ? x.n_cols : x.n_rows);

    arma::mat pdist_mat(size, size);
    pdist_mat.diag(0) = arma::zeros(size);

    for (arma::uword i = 0; i < size; ++i){
        #ifdef _WIN32
        #pragma omp parallel for default(none) \
        shared(pdist_mat, x, size, i, metric, dim)
        for (intmax_t j = 0; j < (intmax_t) size; ++j)
        #else
        #pragma omp parallel for default(none) \
        shared(pdist_mat, x, size, i, metric, dim)
        for (size_t j = 0; j < size; ++ j)
        #endif
        {
            arma::vec first = (dim == 1 ? x.col(i) : arma::vec(x.row(i).t()));
            arma::vec second = (dim == 1 ? x.col(j) : arma::vec(x.row(j).t()));
            double distance = metric.Evaluate(first, second);
            pdist_mat(i, j) = distance;
            pdist_mat(j, i) = distance;
        }
    }
    if (shape == "row"){
        pdist_mat.reshape(1, pdist_mat.n_elem);
        return pdist_mat;
    }
    if (shape == "column"){
        pdist_mat.reshape(pdist_mat.n_elem, 1);
        return pdist_mat;
    }
    return pdist_mat;
}
