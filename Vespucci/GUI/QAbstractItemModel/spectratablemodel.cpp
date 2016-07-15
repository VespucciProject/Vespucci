#include "GUI/QAbstractItemModel/spectratablemodel.h"

SpectraTableModel::SpectraTableModel(QObject *parent, QSharedPointer<VespucciDataset> dataset) :
    QAbstractTableModel(parent)
{
    dataset_ = dataset;
}

SpectraTableModel::SpectraTableModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    dataset_ = QSharedPointer<VespucciDataset>(0);
}

int SpectraTableModel::rowCount(const QModelIndex &parent) const
{
    if (dataset_.data()) return dataset_->spectra_ref().n_cols;
    return 0;
}

int SpectraTableModel::columnCount(const QModelIndex &parent) const
{
    return 3;
}

QVariant SpectraTableModel::data(const QModelIndex &index, int role) const
{
    if (role != Qt::DisplayRole) return QVariant();

    if (!dataset_.data()) return QVariant();

    switch (index.column()){
      case 0: return QString::number(index.row());
      case 1: return QString::number(dataset_->x_ptr()->at(index.row()));
      case 2: return QString::number(dataset_->y_ptr()->at(index.row()));
      default: return QVariant();
    }
}

void SpectraTableModel::ShedRow(int row)
{
    if (dataset_.data()) dataset_->ShedSpectrum(row);
}

