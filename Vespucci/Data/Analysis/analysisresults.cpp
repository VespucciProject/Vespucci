#include "Data/Analysis/analysisresults.h"

///
/// \brief AnalysisResults::AnalysisResults
/// \param name
/// \param type
/// Base constructor
AnalysisResults::AnalysisResults(QString name, QString type) :
    empty_matrix_(),
    name_(name),
    type_(type)
{

}

///
/// \brief AnalysisResults::~AnalysisResults
/// Virtual destructor
AnalysisResults::~AnalysisResults()
{

}

///
/// \brief AnalysisResults::GetMatrix
/// \param name
/// \return
/// Returns empty matrix in the base class, implemented separately for subclasses
const mat &AnalysisResults::GetMatrix(const QString &key)
{
    return empty_matrix_;
}

///
/// \brief AnalysisResults::KeyList
/// \return
/// Returns empty list in the base class, implemented separately for subclasses
QStringList AnalysisResults::KeyList() const
{
    return QStringList();
}

///
/// \brief AnalysisResults::name
/// \return
///
QString AnalysisResults::name() const
{
    return name_;
}

///
/// \brief AnalysisResults::type
/// \return
///
QString AnalysisResults::type() const
{
    return type_;
}

const mat &AnalysisResults::EmptyMatrix()
{
    return empty_matrix_;
}

QMap<QString, QString> AnalysisResults::GetMetadata()
{
    return QMap<QString, QString>();
}

QString AnalysisResults::GetColumnHeading(const QString &key, int column)
{
    return QString();
}
