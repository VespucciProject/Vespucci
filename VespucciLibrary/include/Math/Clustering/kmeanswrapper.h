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
#ifndef KMEANSWRAPPER_H
#define KMEANSWRAPPER_H
#include "libvespucci.h"
#include <mlpack/core.hpp>

namespace Vespucci{
   namespace Math{
       ///
       /// \brief The KMeansWrapper class
       /// mlpack relies heavily on template metaprogramming. This means that you only need to compile
       /// what you actually use. But Vespucci must be precompiled with all possible
       /// options availible at runtime. Here, we instantiate supported combinations of
       /// template parameters for the kmeans class in mlpack
       class VESPUCCI_EXPORT KMeansWrapper
       {
       public:
           KMeansWrapper(const std::string &init,
                         const std::string &metric,
                         bool allow_empty);
           ~KMeansWrapper();
           arma::vec Cluster(const arma::mat &data,
                        const size_t clusters,
                        arma::mat &centroids);
       private:
           std::string init_;
           std::string metric_;
           bool allow_empty_;
       };
   }
}


#endif // KMEANSWRAPPER_H
