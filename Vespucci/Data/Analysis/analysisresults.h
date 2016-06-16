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
    const mat & GetMatrix(const QString &key);
    void AddMatrix(const QString &key, const mat &value, QStringList column_headings = QStringList());
    void AddMetadata(QString key, QString value);
    void AddField(const QString &key, const field<mat> &value);
    const field<mat> & GetField(const QString &key);
    const mat & GetFieldElement(const QString &key, const uword &index);
    bool HasField(const QString &key) const;
    bool HasMatrix(const QString &key) const;
    const QStringList KeyList() const;
    const QString name() const;
    const QString type() const;
    const mat & EmptyMatrix();
    const QMap<QString, QString> GetMetadata() const;
    const QString GetColumnHeading(const QString &key, int column);

private:
    ///
    /// \brief name_
    /// The name used by VespucciDataset and the TreeModel to reference this result
    QString name_;

    ///
    /// \brief type_
    /// Type desciption displayed to user.
    QString type_;

    ///
    /// \brief matrices_
    /// Structure containing all the matrices in this result indexed by name
    QMap<QString, QSharedPointer<mat> > matrices_;

    QMap<QString, QString> metadata_;
    QMap<QString, QStringList> column_headings_;
    QMap<QString, QSharedPointer<field<mat> > > fields_;

    mat empty_matrix_;
};

#endif // ANALYSISRESULTS_H
