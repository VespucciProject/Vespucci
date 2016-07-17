#include "Data/Analysis/analysisresults.h"

///
/// \brief AnalysisResults::AnalysisResults
/// \param name
/// \param type
/// Base constructor
AnalysisResults::AnalysisResults(QString name, QString type) :
    name_(name),
    type_(type)
{

}

AnalysisResults::~AnalysisResults()
{

}

const mat & AnalysisResults::GetMatrix(const QString &key)
{
    if (matrices_.contains(key)) return *matrices_[key];
    else return EmptyMatrix();
}

void AnalysisResults::AddMetadata(QString key, QString value)
{
    metadata_[key] = value;
}

void AnalysisResults::AddField(const QString &key, const field<mat> &value)
{
    fields_[key] = QSharedPointer<field<mat> >(new field<mat>(value));
}

const field<mat> &AnalysisResults::GetField(const QString &key)
{
    return *fields_[key];
}

const mat &AnalysisResults::GetFieldElement(const QString &key, const uword &index)
{
    if (!HasField(key) || (HasField(key) && index >= fields_[key]->n_elem))
        return empty_matrix_;
    else return fields_[key]->at(index);
}

bool AnalysisResults::HasField(const QString &key) const
{
    return fields_.contains(key);
}

bool AnalysisResults::HasMatrix(const QString &key) const
{
    return matrices_.contains(key);
}

void AnalysisResults::AddMatrix(const QString &key, const mat &value, QStringList column_headings)
{
    matrices_[key] = QSharedPointer<mat>(new mat(value));
    column_headings_[key] = column_headings;
}

void AnalysisResults::AddColumns(const QStringList &keys, const mat &value)
{
    //if more columns than provided column names, stop when we run out of
    //names. If more names than columns, stop when we run out of columns
    int count = (keys.size() < value.n_cols ? keys.size() : value.n_cols);
    for (uword i = 0; i < count; ++i)
        matrices_[keys[i]] = QSharedPointer<mat>(new mat(value.col(i)));
}

const QStringList AnalysisResults::KeyList() const
{
    return matrices_.keys();
}

const QString AnalysisResults::name() const
{
    return name_;
}

const QString AnalysisResults::type() const
{
    return type_;
}

const mat & AnalysisResults::EmptyMatrix()
{
    return empty_matrix_;
}

const QMap<QString, QString> AnalysisResults::GetMetadata() const
{
    return metadata_;
}

const QString AnalysisResults::GetColumnHeading(const QString &key, int column)
{
    if (column_headings_.contains(key) && column_headings_[key].size() < column)
        return column_headings_[key][column];
    else return QString();
}

const QString AnalysisResults::GetRowHeading(const QString &key, int row)
{
    if (row_headings_.contains(key) && row_headings_[key].size() < row)
        return row_headings_[key][row];
    else return QString();
}
