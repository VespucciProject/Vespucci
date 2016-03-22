#include "Data/Analysis/analysisresults.h"


AnalysisResults::AnalysisResults(QString name, QString type) : empty_matrix_()
{
    name_ = name;
    type_ = type;
}

AnalysisResults::AnalysisResults(const AnalysisResults &other)
{
    name_ = other.name();
    data_ = other.data();
}

///
/// \brief AnalysisResults::AppendObject
/// \param key key of object to append or overwrite
/// \param object object to append or overwrite
/// Append or overwrite the object with the name key
void AnalysisResults::AppendObject(const QString &key, const mat &object, QStringList column_headings)
{
    data_[key] = object;
    column_headings_[key] = column_headings;
}

QStringList AnalysisResults::GetColumnHeadings(const QString &key)
{
    return column_headings_[key];
}


///Get a copy of the internal data storage
QMap<QString, mat> AnalysisResults::data() const
{
    return data_;
}

///
/// \brief AnalysisResults::value
/// \param key
/// \return A copy of the value at key
///
const mat &AnalysisResults::value(const QString &key) const
{
    if (!data_.contains(key)){return empty_matrix_;}
    return data_[key];
}


QStringList AnalysisResults::KeyList()
{
    return data_.keys();
}

QString AnalysisResults::name() const
{
    return name_;
}

QString AnalysisResults::type() const
{
    return type_;
}

