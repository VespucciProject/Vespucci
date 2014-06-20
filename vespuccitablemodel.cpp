#include "vespuccitablemodel.h"
VespucciTableModel::VespucciTableModel(QObject *parent, mat *input_data) :
    QAbstractTableModel(parent)
{
    data_ = input_data;
    rows_ = data_->n_rows;
    cols_ = data_->n_cols;
    composite_ = false;
}

VespucciTableModel::VespucciTableModel(QObject *parent, QSharedPointer<SpecMap> dataset, QString type) :
    QAbstractTableModel(parent)
{
    dataset_ = dataset;
    composite_ = true;
    if (type == "spatial"){
        rows_ = dataset->x_ptr()->n_rows;
        cols_ = 2;
        spatial_only_ = true;
    }
    else{
        rows_ = dataset->x_ptr()->n_rows + 1;
        cols_ = dataset->wavelength_ptr()->n_cols + 2;
        spatial_only_ = false;
    }
}

QVariant VespucciTableModel::data(const QModelIndex &index, int role) const
{
    if (role != Qt::DisplayRole){
        return QVariant();
    }

    int row = index.row();
    int column = index.column();
    double value;
    if (!composite_){
        value = data_->at(row, column);
        return QString::number(value);
    }

    else
    {
        if (spatial_only_){
            switch (column)
            {
              case 0:
                return QString::number(dataset_->x_ptr()->at(row));
              default:
                return QString::number(dataset_->y_ptr()->at(row));
            }

        }
        switch (row)
        {
          case 0:
            switch (column)
            {
              case 0:
              case 1:
                return "";
              default:
                return QString::number(dataset_->wavelength_ptr()->at(column));
            }
          default:
            switch (column)
            {
              case 0:
                return QString::number(dataset_->x_ptr()->at(row));
              case 1:
                return QString::number(dataset_->y_ptr()->at(row));
              default:
                return QString::number(dataset_->spectra_ptr()->at(row - 1, column - 2));
            }

        }
    }

}

int VespucciTableModel::columnCount(const QModelIndex &parent) const
{
    return cols_;
}

int VespucciTableModel::rowCount(const QModelIndex &parent) const
{
    return rows_;
}
