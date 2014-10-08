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

#include "metadataset.h"

MetaDataset::MetaDataset()
{
}

///
/// \brief MetaDataset::MetaDataset
/// \param name Name of this dataset as displayed to user
/// \param main_window Main window of the program
/// \param log_file The log file
/// \param directory The current global working directory
/// \param method_description The description of the method used
/// \param endmember_selection The string used by the user to select endmembers
/// \param method An enum specifying the method
/// \param parent_datasets The datasets from which this dataset is extracted
///
MetaDataset::MetaDataset(QString name,
                         MainWindow *main_window,
                         QFile *log_file,
                         QString *directory,
                         QString method_description,
                         QString endmember_selection,
                         VespucciMetaMethod method,
                         QList<QSharedPointer<VespucciDataset> > parent_datasets)
    : VespucciDataset(name, main_window, directory, log_file)
{
    parent_datasets_ = parent_datasets;

    QDateTime datetime = QDateTime::currentDateTimeUtc();
    log_stream_ << "Dataset " << name << "created "
                << datetime.date().toString("yyyy-MM-dd") << "T"
                << datetime.time().toString("hh:mm:ss") << "Z" << endl;
    log_stream_ << "Created from previous datasets:" << endl;
    for(int i = 0; i < parent_datasets_.size(); ++i){
        log_stream_ << "  " << parent_datasets_[i]->name() << endl;
    }

    log_stream_ << endl;

    method_ = method;
    method_description_ = method_description;

    ///
    /// \brief endmember_numbers
    /// Holds endmember number vectors in columns.
    field<uvec> endmember_numbers;

    switch(method_) {
    case VCAEndmembers:
        endmember_numbers.set_size(1, parent_datasets_.size(), 1);
        break;
    case AverageSpectra:
        break;
    case ConcatenateDatasets:
        break;
    }


}
