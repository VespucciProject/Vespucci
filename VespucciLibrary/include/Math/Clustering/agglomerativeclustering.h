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
#ifndef AGGLOMERATIVECLUSTERING_H
#define AGGLOMERATIVECLUSTERING_H
#include "libvespucci.h"
#include <mlpack/core.hpp>
#include "Math/Accessory/distancemetricwrapper.h"
#include "Math/Clustering/ahcanode.h"
typedef Vespucci::Math::Clustering::AHCANode node_t;
typedef std::vector<node_t *> nodevec;


namespace Vespucci{
    namespace Math{
        namespace Clustering{
            ///
            /// \brief The AHCA class
            /// Handles agglomerative hierarchical clustering of data
            /// This class holds a tree which represents the clustering dendrogram
            ///
            class VESPUCCI_EXPORT AHCA
            {
                public:
                    AHCA(std::string linkage_method,
                         std::string metric_type);
                    AHCA();

                    void Link(const arma::mat &data);

                    void SetLinkage(std::string linkage_method);
                    void SetMetric(std::string metric_type);

                    arma::mat Cluster(arma::uword k);
                    std::string metric_type();
                    std::string linkage_method();
                    arma::mat merge_data();
                    arma::mat dist();
                    std::map<size_t, nodevec> clusters();
                private:
                    node_t *root_node_;
                    std::string metric_type_;
                    std::string linkage_method_;
                    arma::uword observations_;
                    arma::uvec FindClosest(const arma::mat &data, const arma::mat &dist,
                                           const nodevec &clusters, double &distance);
                    double ClusterDistance(int i, int j, const arma::mat &data, const arma::mat &dist,
                                           const nodevec &clusters);
                    arma::mat dist_;
                    arma::mat merge_data_;
                    arma::mat mean_distance_;
                    std::map<size_t, nodevec> clusters_;

            };

            VESPUCCI_EXPORT arma::mat pdist(const arma::mat &x,
                                            std::string metric_type,
                                            arma::uword dim = 1,
                                            std::string shape = "row");

        }
    }
}

#endif // AGGLOMERATIVECLUSTERING_H
