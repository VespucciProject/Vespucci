#include "datasetlistmodel.h"

DatasetListModel::DatasetListModel(QObject *parent) :
    QAbstractListModel(parent)
{
}

int DatasetListModel::rowCount(const QModelIndex &parent) const
{
    return datasets_.size();
}

bool DatasetListModel::insertRow(int row, const QModelIndex &parent)
{
    datasets_.insert(row, parent.data().value);
}

bool DatasetListModel::removeRow(int row, const QModelIndex &parent)
{
    datasets_.removeAt(row);
}

QVariant DatasetListModel::data(const QModelIndex &index, int role) const
{
    return index.data(role);
}
