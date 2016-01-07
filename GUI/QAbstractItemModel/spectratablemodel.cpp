#include "GUI/QAbstractItemModel/spectratablemodel.h"

SpectraTableModel::SpectraTableModel(QObject *parent, QSharedPointer<VespucciDataset> dataset) :
    QAbstractTableModel(parent)
{
    dataset_ = dataset;
}

int SpectraTableModel::rowCount(const QModelIndex &parent) const
{
    int rows = dataset_->spectra_ptr()->n_cols;
    return rows;
}

int SpectraTableModel::columnCount(const QModelIndex &parent) const
{
    return 3;
}

QVariant SpectraTableModel::data(const QModelIndex &index, int role) const
{
    if (role != Qt::DisplayRole){
        return QVariant();
    }
    switch (index.column()){
      case 0:
            return QString::number(index.row());
      case 1:

            return QString::number(dataset_->x_ptr()->at(index.row()));
      case 2:
            return QString::number(dataset_->y_ptr()->at(index.row()));
      default:
            return QVariant();
    }
}

void SpectraTableModel::ShedRow(int row)
{
    dataset_->ShedSpectrum(row);
}

