/*******************************************************************************
    Copyright (C) 2014-2016 Wright State University - All Rights Reserved
    Daniel P. Foose - Maintainer/Lead Developer

    This file is part of Vespucci.

    Vespucci is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Vespucci is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Vespucci.  If not, see <http://www.gnu.org/licenses/>.
*******************************************************************************/
#ifndef DATASETTREEMODEL_H
#define DATASETTREEMODEL_H

#include <QAbstractItemModel>
#include <GUI/QAbstractItemModel/treeitem.h>
#include <Global/datamodel.h>
class TreeItem;
class DatasetTreeItem;
class MatrixTreeItem;
class ImageTreeItem;
class AnalysisResultTreeItem;
class DataModel;
class DatasetTreeModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    DatasetTreeModel(QObject *parent);
    void SetupModelData(const DataModel *data_model);

    ~DatasetTreeModel();
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QModelIndex parent(const QModelIndex &index) const Q_DECL_OVERRIDE;
    int rowCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    bool removeRows(int row, int count,
                    const QModelIndex &parent = QModelIndex()) Q_DECL_OVERRIDE;
    void UpdateData(const DataModel *data_model);
    bool IsMatrix(const QModelIndex &index);
    bool IsDataset(const QModelIndex &index);
    bool IsMap(const QModelIndex &index);
    void ClearDatasets();
    void removeRow(const QModelIndex &index);
    TreeItem *getItem(const QModelIndex &index) const;
    TreeItem *root_item();


private:
    TreeItem *SetupDatasetTreeItem(QSharedPointer<VespucciDataset> dataset) const;
    TreeItem *SetupAnalysisResultTreeItem(QSharedPointer<VespucciDataset> dataset, QSharedPointer<AnalysisResults> results, TreeItem *parent) const;
    TreeItem *SetupMatrixTreeItem(const QString &dataset_key, const QString &results_key, const QString &matrix_key, const mat &matrix, TreeItem *parent) const;
    TreeItem *SetupMatrixTreeItem(const QString &dataset_key, const QString &matrix_key, const mat &matrix, TreeItem *parent) const;
    TreeItem *SetupMapTreeItem(const QString &dataset_key,
                               const QString &map_key,
                               QSharedPointer<MapData> data,
                               TreeItem *parent) const;
    QString DescribeMatrix(const mat &matrix) const;
    QString DescribeSpectra(const mat &spectra_matrix) const;
    QString DescribeAbscissa(const vec &abscissa) const;
    bool DatasetAdded() const;
    bool DatasetRemoved() const;

    TreeItem *root_item_;

};

#endif // DATASETTREEMODEL_H
