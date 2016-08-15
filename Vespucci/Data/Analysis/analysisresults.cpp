#include "Data/Analysis/analysisresults.h"
#include <QSet>
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
    uword count = (uword(keys.size()) < value.n_cols ? keys.size() : value.n_cols);
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

void AnalysisResults::SetName(const QString &new_name)
{
    name_ = new_name;
}

void AnalysisResults::SetType(const QString &new_type)
{
    type_ = new_type;
}

QSharedPointer<AnalysisResults> AnalysisResults::Subset(QStringList matrices, uword start_row, uword end_row)
{
    QSharedPointer<AnalysisResults> results(new AnalysisResults(name_ + " (subset)", type_ + " (subset)"));
    for (auto matrix_key: matrices){
        mat submatrix = matrices_.value(matrix_key)->rows(start_row, end_row);
        results->AddMatrix(matrix_key, submatrix);
    }
    return results;
}

void AnalysisResults::AddParent(QString key, uword start_row, uword end_row)
{
    parent_rows_[key] = uvec({start_row, end_row});
}

QMap<QString, uvec> AnalysisResults::parent_rows()
{
    return parent_rows_;
}

///
/// \brief AnalysisResults::Concatenate
/// \param other
/// Merge the matrices of other into the same-named matrices of this one
/// Will perform a check of mergability for every matrix before performing merge
/// Non mergable matrices are removed from this result.
bool AnalysisResults::Concatenate(QSharedPointer<AnalysisResults> other)
{
    QSet<QString> intersection = KeyList().toSet().intersect(other->KeyList().toSet());
    //Check mergability (this should not be a problem if both come from same dataset)
    //User may be allowed to merge from two different datasets, if for some reason they wanted to do that
    QSet<QString> mergeable_matrices = intersection;
    for (auto key: intersection)
        if (GetMatrix(key).n_rows != other->GetMatrix(key).n_rows){
            mergeable_matrices.remove(key);
            RemoveMatrix(key);
        }
    if (mergeable_matrices.isEmpty()) return false;
    for (auto key: mergeable_matrices){
        mat matrix = GetMatrix(key);
        mat other_matrix = other->GetMatrix(key);
        matrix = join_horiz(matrix, other_matrix);
        AddMatrix(key, matrix);
    }
    return true;
}

///
/// \brief AnalysisResults::Replicate
/// \return
/// Used in lieu of a copy constructor. Does not produce a perfect copy
/// but does copy name, type, matrices, etc. does not replicate most metadata
QSharedPointer<AnalysisResults> AnalysisResults::Replicate()
{
    QSharedPointer<AnalysisResults> new_result(new AnalysisResults(name_, type_));
    for (auto key: KeyList()){
        new_result->AddMatrix(key, GetMatrix(key));
    }
    return new_result;
}

void AnalysisResults::RemoveMatrix(const QString &key)
{
    matrices_.remove(key);
}
