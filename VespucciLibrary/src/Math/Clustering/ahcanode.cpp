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
#include "Math/Clustering/ahcanode.h"
Vespucci::Math::Clustering::AHCANode::AHCANode(Vespucci::Math::Clustering::AHCANode *child1, Vespucci::Math::Clustering::AHCANode *child2, double distance)
{
    parent_ = NULL;
    child1_ = child1;
    child2_ = child2;
    distance_ = distance;
    if (child1_) child1_->SetParent(this);
    if (child2_) child2_->SetParent(this);
}

Vespucci::Math::Clustering::AHCANode::~AHCANode()
{
    delete child1_;
    delete child2_;
}

double Vespucci::Math::Clustering::AHCANode::distance()
{
    return distance_;
}

Vespucci::Math::Clustering::AHCANode *Vespucci::Math::Clustering::AHCANode::child1()
{
    return child1_;
}

Vespucci::Math::Clustering::AHCANode *Vespucci::Math::Clustering::AHCANode::child2()
{
    return child2_;
}

arma::uvec Vespucci::Math::Clustering::AHCANode::GetChildIndices()
{
    return arma::join_vert(child1_->GetChildIndices(), child2_->GetChildIndices());
}

void Vespucci::Math::Clustering::AHCANode::SetParent(Vespucci::Math::Clustering::AHCANode *parent)
{
    parent_ = parent;
}

Vespucci::Math::Clustering::AHCANode *Vespucci::Math::Clustering::AHCANode::parent()
{
    return parent_;
}

///
/// \brief Vespucci::Math::Clustering::AHCANode::HasChildren
/// \return
/// A way of determining if this node has children or not, without having
/// to check the cast to AHCALeaf
bool Vespucci::Math::Clustering::AHCANode::HasChildren()
{
    return (child1_ != NULL) && (child2_ != NULL);
}

Vespucci::Math::Clustering::AHCALeaf::AHCALeaf(arma::uword index)
    :Vespucci::Math::Clustering::AHCANode(NULL, NULL, 0)
{
    index_ = index;
}

arma::uword Vespucci::Math::Clustering::AHCALeaf::index()
{
    return index_;
}

arma::uvec Vespucci::Math::Clustering::AHCALeaf::GetChildIndices()
{
    return arma::uvec({index_});
}
