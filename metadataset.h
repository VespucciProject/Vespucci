/*******************************************************************************
    Copyright (C) 2014 Daniel P. Foose - All Rights Reserved

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

#ifndef METADATASET_H
#define METADATASET_H
#include "vespuccidataset.h"

///
/// \brief The VespucciMetaMethod enum
/// Different methods for extracting data for a MetaDataset
enum VespucciMetaMethod{
    VCAEndmembers,
    AverageSpectra,
    ConcatenateDatasets
};

///
/// \brief The MetaDataset class
/// A subclass of VespucciDataset for datasets created from multiple other
/// datasets. this stores information about the parents.
class MetaDataset : public VespucciDataset
{
public:
    MetaDataset();
    MetaDataset(QString name, 
                MainWindow *main_window,
                QFile *log_file,
                QString *directory, 
                QString method_description,
                QString endmember_selection,
                VespucciMetaMethod method,
                QList<QSharedPointer<VespucciDataset> > parent_datasets);
private:
    ///
    /// \brief parent_datasets_
    /// Pointers to the dataset that this dataset is constructed from
    QList<QSharedPointer<VespucciDataset> > parent_datasets_;

    ///
    /// \brief method_description
    /// A text description, displayable to the user, of how this dataset was
    /// created from previous
    QString method_description_;

    ///
    /// \brief parent_indices_
    /// A list, of size x_, that indicates the index of parent_datasets_ where
    /// each row originated
    uvec parent_indices_;

    ///
    /// \brief endmember_numbers_
    /// A list, of size x_, that indicates the VCA endmember number of each row.
    /// When method_ != VCAEndmembers, this is undefined.
    uvec endmember_numbers_;

    ///
    /// \brief method_
    /// Enum of the method used
    VespucciMetaMethod method_;
};

#endif // METADATASET_H
