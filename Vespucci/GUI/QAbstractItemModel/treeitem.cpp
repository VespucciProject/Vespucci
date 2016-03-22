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
#include "treeitem.h"

TreeItem::TreeItem(const QList<QVariant> &data, TreeItem *parent)
{
    parent_item_ = parent;
    item_data_ = data;
    is_matrix_ = false;
    is_plot_ = false;
}

TreeItem::TreeItem(const QString &name, TreeItem *parent, bool dataset, bool analysis_result, bool matrix, bool plot)
{
    QString type;
    if (dataset){type = "Dataset";}
    else if (matrix){type="Matrix";}
    else if (plot){type="Plot";}
    else if (analysis_result){type="Analysis Result";}
    else{type="";}
    parent_item_ = parent;
    item_data_ << QVariant(name) << QVariant(type);
    is_matrix_ = matrix;
    is_plot_ = plot;
    is_analysis_results_ = analysis_result;
    is_dataset_ = dataset;
}

TreeItem::~TreeItem()
{
    qDeleteAll(child_items_);
}

void TreeItem::appendChild(TreeItem *child)
{
    child_items_.append(child);
}

TreeItem *TreeItem::child(int row)
{
    return child_items_.value(row);
}

int TreeItem::childCount() const
{
    return child_items_.count();
}

int TreeItem::columnCount() const
{
    return item_data_.count();
}

QVariant TreeItem::data(int column) const
{
    return item_data_.value(column);
}

int TreeItem::row() const
{
    if (parent_item_)
        return parent_item_->child_items().indexOf(const_cast<TreeItem*>(this));
    return 0;
}

TreeItem *TreeItem::parentItem()
{
    return parent_item_;
}

QList<TreeItem *> TreeItem::child_items()
{
    return child_items_;
}

bool TreeItem::is_matrix()
{
    return is_matrix_;
}

bool TreeItem::is_plot()
{
    return is_plot_;
}

bool TreeItem::is_dataset()
{
    return is_dataset_;
}

bool TreeItem::is_analysis_results()
{
    return is_analysis_results_;
}

void TreeItem::UpdateType(QString new_type)
{
    if (item_data_.size() > 1)
        item_data_[1] = QVariant(new_type);
}

void TreeItem::ClearChildren()
{
    child_items_.clear();
}

DatasetTreeItem::DatasetTreeItem(QSharedPointer<VespucciDataset> dataset, TreeItem *parent)
    : TreeItem(dataset->name(), parent, true, false, false, false)
{
    dataset_ = dataset;
    spectra_item_ = new MatrixTreeItem("Spectra", dataset, parent);
    abscissa_item_ = new MatrixTreeItem("Spectral Abscissa", dataset, parent);
    x_item_ = new MatrixTreeItem("x", dataset, parent);
    y_item_ = new MatrixTreeItem("y", dataset, parent);

    UpdateDescriptions();
    appendChild(spectra_item_);
    appendChild(abscissa_item_);
    appendChild(x_item_);
    appendChild(y_item_);

}

QSharedPointer<VespucciDataset> DatasetTreeItem::dataset()
{
    return dataset_;
}

void DatasetTreeItem::UpdateDescriptions()
{
    const arma::mat &spectra(dataset_->spectra_ref());
    const arma::vec &abscissa(dataset_->abscissa_ref());
    const arma::vec &x(dataset_->x_ref());
    const arma::vec &y(dataset_->y_ref());
    int spectrum_rows = spectra.n_rows;
    int spectrum_cols = spectra.n_cols;
    int abscissa_rows = abscissa.n_rows;
    double abscissa_min = abscissa.min();
    double abscissa_max = abscissa.max();
    int x_rows = x.n_rows;
    int y_rows = y.n_rows;
    int unique_x = dataset_->UniqueX();
    int unique_y = dataset_->UniqueY();
    double x_min = x.min();
    double y_min = y.min();
    double x_max = x.max();
    double y_max = y.max();

    QString spectra_description = QString::number(spectrum_cols)
            + " spectra with " + QString::number(spectrum_rows) + " elements";

    QString abscissa_description = QString::number(abscissa_rows) + " elements, "
            + QString::number(abscissa_min) + "–" + QString::number(abscissa_max)
            + " " + dataset_->x_axis_description();

    QString x_description = QString::number(unique_x) + " unique elements ("
            + QString::number(x_rows) + " total), " + QString::number(x_min)
            + "–" + QString::number(x_max);

    QString y_description = QString::number(unique_y) + " unique elements ("
            + QString::number(y_rows) + " total), " + QString::number(y_min)
            + "–" + QString::number(y_max);

    spectra_item_->UpdateType(spectra_description);
    abscissa_item_->UpdateType(abscissa_description);
    x_item_->UpdateType(x_description);
    y_item_->UpdateType(y_description);
}

///
/// \brief DatasetTreeItem::AddChildren
/// Initializes and adds the children to this model.
/// Is also used to update the model from VespucciDataset
/// For matrices to appear in the model, this must be called
void DatasetTreeItem::AddChildren()
{
    QStringList aux_mat_list = dataset_->AuxiliaryMatrixKeys();
    QStringList analysis_results_list = dataset_->AnalysisResultsKeys();
    QStringList maps_list = dataset_->MapKeys();

    QStringList mat_child_names;
    QStringList result_child_names;
    QStringList map_child_names;

    foreach (TreeItem *item, child_items()){
        if (item->is_analysis_results())
            result_child_names << item->data(0).toString();
        if (item->is_matrix())
            mat_child_names << item->data(0).toString();
        if (item->is_plot())
            map_child_names << item->data(0).toString();
    }
    foreach (QString name, aux_mat_list){
        if (!mat_child_names.contains(name))
            appendChild(new MatrixTreeItem(name, dataset_, this));
    }
    foreach(QString name, analysis_results_list){
        if (!result_child_names.contains(name))
            appendChild(new AnalysisResultTreeItem(name, this));
    }
    foreach(QString name, maps_list){
        if (!map_child_names.contains(name))
            appendChild(new ImageTreeItem(dataset_->GetMapData(name), this));
    }

}

AnalysisResultTreeItem::AnalysisResultTreeItem(const QString &name, DatasetTreeItem *parent)
    : TreeItem(name, parent, false, true, false, false)
{
    //create children from this analysis result
    QSharedPointer<AnalysisResults> result = parent->dataset()->GetAnalysisResult(name);
    QStringList matrix_list = result->KeyList();
    foreach (const QString &matrix_name, matrix_list)
        appendChild(new MatrixTreeItem(matrix_name, parent->dataset(), this));
}


MatrixTreeItem::MatrixTreeItem(const QString &key, QSharedPointer<VespucciDataset> dataset, TreeItem *parent)
    : TreeItem(key, parent, false, false, true, false)
{
    key_ = key;
    parent_key_ = "";
    dataset_ = dataset;
    const mat &matrix(dataset_->GetAuxiliaryMatrix(key));
    int rows = matrix.n_rows;
    int cols = matrix.n_cols;
    QString new_description = "[" + QString::number(rows)
            + " x " + QString::number(cols) + " ] matrix";
    UpdateType(new_description);

}

MatrixTreeItem::MatrixTreeItem(const QString &key, const QString &parent_key, QSharedPointer<VespucciDataset> dataset, TreeItem *parent)
    : TreeItem(key, parent, false, false, true, false)
{
    key_ = key;
    parent_key_ = parent_key;
    dataset_ = dataset;
    const mat &matrix(dataset_->GetAnalysisResultMatrix(parent_key, key));
    int rows = matrix.n_rows;
    int cols = matrix.n_cols;
    QString new_description = "[" + QString::number(rows)
            + " x " + QString::number(cols) + " ] matrix";
    UpdateType(new_description);
}

QString MatrixTreeItem::key() const
{
    return key_;
}

const arma::mat &MatrixTreeItem::value()
{
    if (parentItem()->is_analysis_results()){
        return dataset_->GetAnalysisResultMatrix(key_, parent_key_);
    }
    if (dataset_->IsCoreMatrix(key_)){
        return dataset_->GetCoreMatrix(key_);
    }
    return dataset_->GetAuxiliaryMatrix(key_);
}

int MatrixTreeItem::MatrixRows()
{
    return value().n_rows;
}

int MatrixTreeItem::MatrixColumns()
{
    return value().n_cols;
}

QString MatrixTreeItem::parent_key()
{
    return parent_key_;
}

ImageTreeItem::ImageTreeItem(QSharedPointer<MapData> map_data, DatasetTreeItem *parent)
    : TreeItem(map_data->name(), parent, false, false, false, true)
{
    map_data_ = map_data;
}

QSharedPointer<MapData> ImageTreeItem::map_data()
{
    return map_data_;
}

void ImageTreeItem::ToggleMapViewerVisible()
{
    map_data_->ShowMapWindow(map_data_->MapWindowVisible());
}
