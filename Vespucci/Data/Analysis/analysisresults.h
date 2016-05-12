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
#ifndef ANALYSISRESULTS_H
#define ANALYSISRESULTS_H
#include "Global/libvespucci.h"
#include "Math/VespucciMath.h"
#include <QStringList>
#include <QMap>
#include <QSharedPointer>
using namespace std;
using namespace arma;
///
/// \brief The AnalysisResults class
/// A container for a mat object that allows a mat to be copied to a heap-allocated
/// object (this) so pointers to that mat cannot go out of scope. These objects
/// should be heap-allocated inside smart pointers.
class AnalysisResults
{
public:    
    AnalysisResults(QString name, QString type);
    virtual ~AnalysisResults();
    const mat &GetMatrix(const QString &key);
    QStringList KeyList() const;
    QString name() const;
    QString type() const;
    const mat &EmptyMatrix();
    QMap<QString, QString> GetMetadata();
    QString GetColumnHeading(const QString &key, int column);
private:
    ///
    /// \brief data_
    /// key-value store for the data associated with this analysis
    ///
    /// \brief empty_matrix_
    /// A matrix that is initialized with the default empty constructor
    const mat empty_matrix_;

    ///
    /// \brief name_
    /// The name used by VespucciDataset and the TreeModel to reference this result
    QString name_;

    ///
    /// \brief type_
    /// Type desciption displayed to user.
    QString type_;
};

#endif // ANALYSISRESULTS_H
