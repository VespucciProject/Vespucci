#include "datasetlistmodel.h"

DatasetListModel::DatasetListModel(QObject *parent, VespucciWorkspace *ws) :
    QAbstractListModel(parent)
{
    workspace = ws;
    workspace->SetDatasets(&datasets_);
}

int DatasetListModel::rowCount(const QModelIndex &parent) const
{
    return datasets_.size();
}

///
/// \brief DatasetListModel::data
/// \param index
/// \param role The role (will always be Qt::DisplayRole (to display dataset name)
/// \return
/// A re-implementation of QAbstractItemModel::data();
QVariant DatasetListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= datasets_.size())
        return QVariant();
    else if (role == Qt::DisplayRole)
        return QVariant(datasets_.at(index.row())->name());
    else
        return QVariant();
}

bool DatasetListModel::removeRow(int row, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row);
    datasets_.removeAt(row);
    endRemoveRows();
    emit dataChanged(parent, parent);
    return true;
}

bool DatasetListModel::AddDataset(QSharedPointer<VespucciDataset> dataset)
{
    int row = datasets_.size();
    QModelIndex index = createIndex(row, 0);
    beginInsertRows(index, row, row);
    datasets_.append(dataset);
    endInsertRows();
    emit dataChanged(index, index);
    emit DatasetAdded(index);
    workspace->UpdateCount();
    return true;
}

QSharedPointer<VespucciDataset> DatasetListModel::DatasetAt(int row)
{
    return datasets_.at(row);
}

///
/// \brief DatasetListModel::ClearMapss
/// Clears the dataset container. Used when closing the program.
void DatasetListModel::ClearDatasets()
{
    datasets_.clear();
}
