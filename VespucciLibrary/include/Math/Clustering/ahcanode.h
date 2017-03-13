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
#ifndef AHCANODE_H
#define AHCANODE_H
#include "libvespucci.h"
#include <mlpack/core.hpp>
namespace Vespucci{
    namespace Math{
        namespace Clustering{
        ///
        /// \brief The AHCANode class
        /// This class stores a node in the data structure representing the AHCA dendrogram
        /// The AHCA dendrogram is a binary tree, each node stores two children,
        /// and its parent, if it exists.
        /// The root node has a NULL parent.
        /// The "leaves", which store single indices have null children
            class VESPUCCI_EXPORT AHCANode
            {
                public:
                    AHCANode(Vespucci::Math::Clustering::AHCANode *child1, Vespucci::Math::Clustering::AHCANode *child2, double distance);
                    virtual ~AHCANode();
                    double distance();
                    Vespucci::Math::Clustering::AHCANode *child1();
                    Vespucci::Math::Clustering::AHCANode *child2();
                    virtual arma::uvec GetChildIndices();
                    void SetParent(Vespucci::Math::Clustering::AHCANode *parent);
                    Vespucci::Math::Clustering::AHCANode *parent();
                    bool HasChildren();
                private:
                    Vespucci::Math::Clustering::AHCANode *child1_;
                    Vespucci::Math::Clustering::AHCANode *child2_;
                    Vespucci::Math::Clustering::AHCANode *parent_;
                    double distance_;
            };

            class VESPUCCI_EXPORT AHCALeaf : public Vespucci::Math::Clustering::AHCANode
            {
                public:
                    AHCALeaf(arma::uword index);
                    arma::uword index();
                    arma::uvec GetChildIndices();
                private:
                    arma::uword index_;
            };
        }
    }
}



#endif // AHCANODE_H
