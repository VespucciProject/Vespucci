/*******************************************************************************
    Copyright (C) 2015 Wright State University - All Rights Reserved
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
#ifndef TREEITEM_H
#define TREEITEM_H
#include <QList>
#include <QVariant>
#include "Data/Dataset/vespuccidataset.h"
class MapData;
class VespucciDataset;
class TreeItem
{
public:
    TreeItem(const QList<QVariant> &data, TreeItem *parent = 0);
    TreeItem(const QString &name, TreeItem *parent, bool is_dataset, bool is_analysis_result, bool is_matrix, bool is_plot);
    virtual ~TreeItem();
    void appendChild(TreeItem *child);
    TreeItem *child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    int row() const;
    TreeItem *parentItem();
    QList<TreeItem*> child_items();
    bool is_matrix();
    bool is_plot();
    bool is_dataset();
    bool is_analysis_results();
    void UpdateType(QString new_type);
    void ClearChildren();

private:
    QList<TreeItem*> child_items_;
    QList<QVariant> item_data_;
    TreeItem *parent_item_;
    QString type_;
    bool is_matrix_;
    bool is_plot_;
    bool is_dataset_;
    bool is_analysis_results_;

};

///
/// \brief The DatasetTreeItem class
/// Parent is root item of model
class MatrixTreeItem;
class DatasetTreeItem : public TreeItem
{
public:
    DatasetTreeItem(QSharedPointer<VespucciDataset> dataset, TreeItem *parent);
    //appendChild should handle both cases
    //void AddAnalysisResult(AnalysisResultTreeItem *result);
    //void AddImage(ImageTreeItem *item);
    QSharedPointer<VespucciDataset> dataset();
    void UpdateDescriptions();
    void AddChildren();
private:
    QSharedPointer<VespucciDataset> dataset_;

    ///
    /// \brief spectra_item_
    /// Spectra are editable, so we need to know to update the dimensions
    MatrixTreeItem *spectra_item_;

    ///
    /// \brief abscissa_item_
    /// Abscissa is editable, so we need to be able to update dimensions
    MatrixTreeItem *abscissa_item_;

    ///
    /// \brief spatial_item_
    /// Spatial data is editable, so we need to be able to update dimensions
    MatrixTreeItem *x_item_;

    MatrixTreeItem *y_item_;



};

///
/// \brief The AnalysisResultTreeItem class
/// Parent should always be a DatasetTreeItem
class AnalysisResultTreeItem : public TreeItem
{
public:
    AnalysisResultTreeItem(const QString &name, DatasetTreeItem *parent);
};

///
/// \brief The MatrixTreeItem class
/// Is child of either AnalysisResultTreeItem or DatasetTreeItem
class MatrixTreeItem : public TreeItem
{
public:
    MatrixTreeItem(const QString &key, QSharedPointer<VespucciDataset> dataset, TreeItem *parent);
    MatrixTreeItem(const QString &key, const QString &parent_key, QSharedPointer<VespucciDataset> dataset, TreeItem *parent);
    QString key() const;
    const arma::mat &value();
    int MatrixRows();
    int MatrixColumns();
    QString parent_key();
private:
    QString parent_key_;
    QString key_;
    ///
    /// \brief dataset_
    /// The dataset that value_ was calculated from
    QSharedPointer<VespucciDataset> dataset_;
};

class ImageTreeItem : public TreeItem
{
public:
    ImageTreeItem(QSharedPointer<MapData> map_data, DatasetTreeItem *parent);
    QSharedPointer<MapData> map_data();
    void ToggleMapViewerVisible();
private:
    QSharedPointer<MapData> map_data_;
};

#endif // TREEITEM_H
