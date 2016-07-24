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
#include "datasettreemodel.h"

DatasetTreeModel::DatasetTreeModel(QObject *parent)
    : QAbstractItemModel(parent)
{
    QStringList root_keys;
    QList<QVariant> root_data = {QVariant("Title"), QVariant("Description")};
    root_item_ = new TreeItem(TreeItem::ItemType::Base, root_keys, root_data, 0);

}

DatasetTreeModel::DatasetTreeModel(QObject *parent, const DataModel *data_model)
    : QAbstractItemModel(parent)
{
    QStringList root_keys;
    QList<QVariant> root_data = {QVariant("Title"), QVariant("Description")};
    root_item_ = new TreeItem(TreeItem::ItemType::Base, root_keys, root_data, 0);
    QStringList dataset_keys = data_model->DatasetNames();

    for (auto dataset_key: dataset_keys){
        QStringList dataset_item_keys = {dataset_key};
        QList<QVariant> dataset_item_data = {QVariant(dataset_key),
                                        QVariant("Dataset")};
        TreeItem *dataset_tree_item = new TreeItem(TreeItem::ItemType::Dataset,
                                                   dataset_item_keys, dataset_item_data,
                                                   root_item_);
        QSharedPointer<VespucciDataset> dataset = data_model->GetDataset(dataset_key);
        QList<QStringList> corekeys = {
            {dataset_key, "Spectra"},
            {dataset_key, "Spectral Abscissa"},
            {dataset_key, "x"},
            {dataset_key, "y"}
        };

        QList<QList<QVariant> > coredata = {
            {"Spectra", DescribeSpectra(dataset->spectra_ref())},
            {"Spectral Abscissa", DescribeAbscissa(dataset->abscissa_ref())},
            {"x", DescribeMatrix(dataset->x_ref())},
            {"y", DescribeMatrix(dataset->y_ref())}
        };

        for (int i = 0; i < coredata.size(); ++i){
            TreeItem *coreitem = new TreeItem(TreeItem::Matrix,
                                             corekeys[i],
                                             coredata[i],
                                             dataset_tree_item);
            dataset_tree_item->appendChild(coreitem);
        }

        for (auto matrix_key: dataset->AuxiliaryMatrixKeys()){
            QStringList item_keys = {dataset_key, matrix_key};
            QList<QVariant> item_data =
            {QVariant(matrix_key),
            QVariant(DescribeMatrix(dataset->GetAuxiliaryMatrix(matrix_key)))
            };
            TreeItem *matrix_item = new TreeItem(TreeItem::ItemType::Matrix,
                                                 item_keys, item_data,
                                                 dataset_tree_item);
            dataset_tree_item->appendChild(matrix_item);
        }

        for (auto results_key: dataset->AnalysisResultsKeys()){
            QSharedPointer<AnalysisResults> results =
                    dataset->GetAnalysisResult(results_key);
            QStringList results_item_keys = {dataset_key, results_key};
            QList<QVariant> results_item_data = {QVariant(results_key),
                                                 QVariant(results->type())};
            TreeItem *results_tree_item =
                    new TreeItem(TreeItem::ItemType::AnalysisResult,
                                 results_item_keys,
                                 results_item_data,
                                 dataset_tree_item);

            for (auto matrix_key: results->KeyList()){
                QStringList matrix_item_keys = {dataset_key, results_key, matrix_key};
                QList<QVariant> matrix_item_data = {QVariant(matrix_key),
                                                    QVariant(DescribeMatrix(results->GetMatrix(matrix_key)))
                                                   };
                TreeItem *matrix_tree_item =
                        new TreeItem(TreeItem::Matrix,
                                     matrix_item_keys,
                                     matrix_item_data,
                                     results_tree_item);
                results_tree_item->appendChild(matrix_tree_item);
            }
            dataset_tree_item->appendChild(results_tree_item);
        }

        for (auto map_key: dataset->MapKeys()){
            QSharedPointer<MapData> mapdata = dataset->GetMapData(map_key);
            QStringList map_item_keys = {dataset_key, map_key};
            QList<QVariant> map_item_data = {QVariant(map_key),
                                             QVariant(mapdata->type())};
            TreeItem *map_tree_item =
                    new TreeItem(TreeItem::Map,
                                 map_item_keys,
                                 map_item_data,
                                 dataset_tree_item);
            dataset_tree_item->appendChild(map_tree_item);
        }
        root_item_->appendChild(dataset_tree_item);
    }
}

///
/// \brief DatasetTreeModel::SetupModelData
/// \param data_model
/// Can throw an exception if one is thrown by data_model->GetDataset()
void DatasetTreeModel::SetupModelData(const DataModel *data_model)
{
    QStringList dataset_keys = data_model->DatasetNames();
    for (auto dataset_key: dataset_keys){
        QSharedPointer<VespucciDataset> dataset = data_model->GetDataset(dataset_key);
        TreeItem *dataset_tree_item = SetupDatasetTreeItem(dataset);
        root_item_->appendChild(dataset_tree_item);
    }
}


DatasetTreeModel::~DatasetTreeModel()
{
    delete root_item_;
}

QVariant DatasetTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    if(role != Qt::DisplayRole)
        return QVariant();
    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
    return item->data(index.column());
}

Qt::ItemFlags DatasetTreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;
    return QAbstractItemModel::flags(index);
}

QVariant DatasetTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return root_item_->data(section);
    return QVariant();
}

QModelIndex DatasetTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if(!hasIndex(row, column, parent))
        return QModelIndex();
    TreeItem *parent_item;
    if (!parent.isValid())
        parent_item = root_item_;
    else
        parent_item = static_cast<TreeItem*>(parent.internalPointer());

    TreeItem *child_item = parent_item->child(row);
    if (child_item)
        return createIndex(row, column, child_item);
    else
        return QModelIndex();
}

QModelIndex DatasetTreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    TreeItem *child_item = static_cast<TreeItem*>(index.internalPointer());
    TreeItem *parent_item = child_item->parentItem();

    if (parent_item == root_item_)
        return QModelIndex();
    return createIndex(parent_item->row(), 0, parent_item);
}

int DatasetTreeModel::rowCount(const QModelIndex &parent) const
{
    TreeItem *parent_item = getItem(parent);
    return parent_item->childCount();
}

int DatasetTreeModel::columnCount(const QModelIndex &parent) const
{
    return root_item_->columnCount();
}

bool DatasetTreeModel::removeRows(int row, int count, const QModelIndex &parent)
{
    //TreeItem *parent_item = getItem(parent);
    bool success = true;
    beginRemoveRows(parent, row, count + row - 1);
    endRemoveRows();
    return success;
}

void DatasetTreeModel::UpdateData(const DataModel *data_model)
{
    QList<TreeItem*> items = root_item_->child_items();
    //From top level (dataset level), make sure that any items in items
    //that are not in the data model are removed
    for (auto item: items)
        if (!data_model->DatasetNames().contains(item->data(0).toString()))
            root_item_->removeChild(item);

    //Check if a dataset has been added and set it up if it has been
    QStringList dataset_names = root_item_->ChildNames();
    for (auto dataset_key: data_model->DatasetNames()){
        if (!dataset_names.contains(dataset_key)){
            TreeItem *dataset_tree_item = SetupDatasetTreeItem(data_model->GetDataset(dataset_key));
            root_item_->appendChild(dataset_tree_item);
        }
    }

    //check all the datasets to see if anything changed in them:
    for (auto item: root_item_->child_items()){
        QString dataset_name = item->data(0).toString();
        QStringList analysis_results_names = data_model->AnalysisResultsNames(dataset_name);
        QStringList core_matrix_names = {"Spectra", "Spectral Abscissa", "x", "y"};

        //check for changed analysis results
        QStringList child_names = item->ChildNames();
        //add new analysis results
        for (auto results_name: analysis_results_names){
            if (!child_names.contains(results_name)
                    && data_model->GetResults(dataset_name, results_name)->KeyList().size()){
                TreeItem *analysis_results_item =
                        SetupAnalysisResultTreeItem(data_model->GetDataset(dataset_name),
                                                    data_model->GetResults(dataset_name, results_name),
                                                    item);
                item->appendChild(analysis_results_item);
            }//if
        }//for (AnalysisResultsNames)

        //Remove old subobjects
        for (auto child_item: item->child_items()){
            QString child_name = child_item->data(0).toString();
            if (!analysis_results_names.contains(child_name)
                    && !core_matrix_names.contains(child_name)){
                item->removeChild(child_item);
            }
        }//for (item->child_items)

    }//for (root_item->child_items)


}

void DatasetTreeModel::AddDataset(QSharedPointer<VespucciDataset> dataset)
{
    TreeItem *dataset_tree_item = SetupDatasetTreeItem(dataset);
    root_item_->appendChild(dataset_tree_item);
}


void DatasetTreeModel::ClearDatasets()
{
    int row_count = rowCount(index(0,0));
    beginRemoveRows(index(0,0), 0, row_count - 1);
    root_item_->ClearChildren();
    endRemoveRows();
}

void DatasetTreeModel::removeRow(const QModelIndex &index)
{
    beginRemoveRows(index, index.row(), index.row());
    removeRows(index.row(), 1, index);
    endRemoveRows();
}

TreeItem *DatasetTreeModel::getItem(const QModelIndex &index) const
{
    if (index.isValid()){
        TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
        if (item){return item;}
    }
    return root_item_;
}

TreeItem *DatasetTreeModel::root_item()
{
    return root_item_;
}

///
/// \brief DatasetTreeModel::SetUpDatasetTreeItem
/// \param key
/// \return
///
TreeItem *DatasetTreeModel::SetupDatasetTreeItem(QSharedPointer<VespucciDataset> dataset)
{
    QList<QVariant> dataset_data = {QVariant(dataset->name()), QVariant("Dataset")};
    QStringList dataset_key_list = {dataset->name()};
    TreeItem *dataset_tree_item = new TreeItem(TreeItem::Dataset,
                                               dataset_key_list,
                                               dataset_data,
                                               root_item_);
    QList<QStringList> corekeys = {
        {dataset->name(), "Spectra"},
        {dataset->name(), "Spectral Abscissa"},
        {dataset->name(), "x"},
        {dataset->name(), "y"}
    };

    QList<QList<QVariant> > coredata = {
        {"Spectra", DescribeSpectra(dataset->spectra_ref())},
        {"Spectral Abscissa", DescribeAbscissa(dataset->abscissa_ref())},
        {"x", DescribeMatrix(dataset->x_ref())},
        {"y", DescribeMatrix(dataset->y_ref())}
    };

    for (int i = 0; i < coredata.size(); ++i){
        TreeItem *coreitem = new TreeItem(TreeItem::Matrix,
                                         corekeys[i],
                                         coredata[i],
                                         dataset_tree_item);
        dataset_tree_item->appendChild(coreitem);
    }

    for (auto results_key: dataset->AnalysisResultsKeys()){
        QSharedPointer<AnalysisResults> results = dataset->GetAnalysisResult(results_key);
        TreeItem *analysis_results_item =
                SetupAnalysisResultTreeItem(dataset,
                                            results,
                                            dataset_tree_item);
        dataset_tree_item->appendChild(analysis_results_item);
    }

    for (auto matrix_key: dataset->AuxiliaryMatrixKeys()){
        const mat& matrix = dataset->GetAuxiliaryMatrix(matrix_key);
        TreeItem *matrix_item = SetupMatrixTreeItem(dataset->name(),
                                                    matrix_key,
                                                    matrix,
                                                    dataset_tree_item);
        dataset_tree_item->appendChild(matrix_item);
    }

    for (auto map_key: dataset->MapKeys()){
        QSharedPointer<MapData> map = dataset->GetMapData(map_key);
        TreeItem *map_item = SetupMapTreeItem(dataset->name(),
                                              map_key,
                                              map,
                                              dataset_tree_item);
        dataset_tree_item->appendChild(map_item);
    }

    return dataset_tree_item;
}

///
/// \brief DatasetTreeModel::SetupAnalysisResultTreeItem
/// \param dataset_key
/// \param results_key
/// \return
///
TreeItem *DatasetTreeModel::SetupAnalysisResultTreeItem(QSharedPointer<VespucciDataset> dataset,
                                                        QSharedPointer<AnalysisResults> results,
                                                        TreeItem *parent)
{
    QStringList analysis_results_key_list = {dataset->name(), results->name()};

    QList<QVariant> analysis_results_item_data =
            {results->name(), results->type()};

    TreeItem *results_tree_item =
            new TreeItem(TreeItem::AnalysisResult,
                         analysis_results_key_list,
                         analysis_results_item_data,
                         parent);

    for (auto matrix_key: results->KeyList()){
        TreeItem *matrix_tree_item =
                SetupMatrixTreeItem(dataset->name(),
                                    results->name(),
                                    results->GetMatrix(matrix_key),
                                    results_tree_item);
        results_tree_item->appendChild(matrix_tree_item);
    }

    return results_tree_item;
}

///
/// \brief DatasetTreeModel::SetupMatrixTreeItem
/// \param dataset_key
/// \param results_key
/// \param matrix_key
/// \param matrix
/// \param parent
/// \return
///
TreeItem *DatasetTreeModel::SetupMatrixTreeItem(const QString &dataset_key,
                                                const QString &results_key,
                                                const QString &matrix_key,
                                                const mat & matrix,
                                                TreeItem *parent)
{
    QStringList matrix_key_list = {dataset_key, results_key, matrix_key};
    QList<QVariant> matrix_data = {matrix_key, DescribeMatrix(matrix)};
    return new TreeItem(TreeItem::Matrix, matrix_key_list, matrix_data, parent);
}

///
/// \brief DatasetTreeModel::SetupMatrixTreeItem
/// \param dataset_key
/// \param matrix_key
/// \param matrix
/// \param parent
/// \return
///
TreeItem *DatasetTreeModel::SetupMatrixTreeItem(const QString &dataset_key,
                                                const QString &matrix_key,
                                                const mat & matrix,
                                                TreeItem *parent)
{
    QStringList matrix_key_list = {dataset_key, matrix_key};
    QList<QVariant> matrix_data = {matrix_key, DescribeMatrix(matrix)};
    return new TreeItem(TreeItem::Matrix, matrix_key_list, matrix_data, parent);
}

///
/// \brief DatasetTreeModel::SetupMapTreeItem
/// \param dataset_key
/// \param map_key
/// \param data
/// \param parent
/// \return
///
TreeItem *DatasetTreeModel::SetupMapTreeItem(const QString &dataset_key,
                                             const QString &map_key,
                                             QSharedPointer<MapData> data,
                                             TreeItem *parent)
{
    QStringList map_key_list = {dataset_key, map_key};
    QList<QVariant> map_data = {map_key, data->type()};
    return new TreeItem(TreeItem::Map, map_key_list, map_data, parent);
}


QString DatasetTreeModel::DescribeMatrix(const mat & matrix) const
{
    return QString::number(matrix.n_rows)
            + "×"
            + QString::number(matrix.n_cols)
            + " matrix";
}

QString DatasetTreeModel::DescribeSpectra(const mat & spectra_matrix) const
{
    return QString::number(spectra_matrix.n_cols) + " spectra";
}

QString DatasetTreeModel::DescribeAbscissa(const vec &abscissa) const
{
    return QString::number(abscissa.n_rows)
            + " points (" + QString::number(abscissa.min())
            + "–" + QString::number(abscissa.max()) + ")";
}
