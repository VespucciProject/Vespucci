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

bool AnalysisResults::HasMatrix(const QString &key) const
{
    return matrices_.contains(key);
}

void AnalysisResults::AddMatrix(const QString &key, const mat &value, QStringList column_headings)
{
    matrices_[key] = QSharedPointer<mat>(new mat(value));
    column_headings_[key] = column_headings;
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
