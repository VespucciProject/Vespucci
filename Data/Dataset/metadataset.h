/*******************************************************************************
    Copyright (C) 2015 Wright State University - All Rights Reserved
    Daniel P. Foose - Author

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
#include "Data/Dataset/vespuccidataset.h"



///
/// \brief The MetaDataset class
/// A subclass of VespucciDataset for datasets created from multiple other
/// datasets. this stores information about the parents.
class MetaDataset : public VespucciDataset
{
public:
    MetaDataset(QString name, 
                MainWindow *main_window,
                QFile *log_file,
                QString *directory, 
                QString method_description,
                MetaMethod::Method method,
                QList<QSharedPointer<VespucciDataset> > parent_datasets);
    vec *parents();

private:
    ///
    /// \brief parent_datasets_
    /// Pointers to the datasets that this dataset is constructed from
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
    /// \brief parents_
    /// A list of the origin of each dataset
    vec parents_;
    mat parent_coordinates_;


    ///
    /// \brief method_
    /// Enum of the method used
    MetaMethod::Method method_;

    mat ProcessAverage(vec &x, vec &y);
    mat Concatenate(vec &x, vec &y);

    mat Stitch(vec &x, vec &y, int v_pos, int h_pos);

    bool ParentsValid();
};

#endif // METADATASET_H
