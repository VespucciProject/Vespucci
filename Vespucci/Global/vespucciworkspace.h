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

#ifndef VESPUCCIWORKSPACE_H

#define VESPUCCIWORKSPACE_H

#include "Data/Dataset/vespuccidataset.h"

#include "Math/VespucciMath.h"
#include "Global/vespucci.h"
#include <qcustomplot.h>
#include "GUI/mainwindow.h"
#include "GUI/QAbstractItemModel/datasettreemodel.h"
#include "Global/enums.h"
#include "Global/datamodel.h"
class VespucciDataset;
class MainWindow;
class DatasetTreeModel;
///
/// \brief The VespucciWorkspace class
/// A class which contains all "global variables" (that aren't held in MainWindow)
class VespucciWorkspace
{
public:
    VespucciWorkspace();
    ~VespucciWorkspace();
    QStringList dataset_names() const;
    //QStringList map_names();

    //adds or removes dataset or map to relevant lists
    void AddDataset(QSharedPointer<VespucciDataset> dataset);
    void RemoveDataset(QString name);
    void UpdateModel();

    void RemoveDatasetAt(const QModelIndex &parent);
    //void RemoveMapAt(int i);
    QSharedPointer<VespucciDataset> DatasetAt(const QModelIndex &parent);

    int dataset_loading_count() const;
    //int map_loading_count();

    void set_directory(QString directory);
    QString directory() const;
    QString *directory_ptr();

    void SetPointers(MainWindow *main_window);

    MainWindow* main_window();
    double GetWavelengthMin(const QString &key) const;
    double GetWavelengthMax(const QString &key) const;

    QSharedPointer<VespucciDataset> GetDataset(const QString &key) const;
    QSharedPointer<AnalysisResults> GetAnalysisResults(const QString &dataset_key, const QString &results_key) const;
    QSharedPointer<MapData> GetMap(const QString &dataset_key, const QString &map_key) const;
    const mat& GetAuxiliaryMatrix(const QString &dataset_key, const QString &matrix_key) const;
    const mat& GetResultsMatrix(const QString &dataset_key, const QString &results_key, const QString &matrix_key) const;
    const mat& GetCoreMatrix(const QString &dataset_key, const QString &matrix_key) const;



    QCPRange *global_data_range();
    QCPColorGradient *global_gradient();

    QFile *CreateLogFile(QString dataset_name);
    void DestroyLogFile(QFile *log_file);


    bool RecalculateGlobalDataRange(QCPRange* new_data_range);
    void RefreshGlobalColorGradient(QCPColorGradient new_gradient);
    void SetGlobalDataRange(QCPRange* new_data_range);

    void ClearDatasets();

    QList<QSharedPointer<VespucciDataset> > *datasets();
    void SetListWidgetModel(DatasetTreeModel *model);
    void SetDatasets(QList<QSharedPointer<VespucciDataset> > *datasets);
    unsigned int UpdateCount();
    DatasetTreeModel *dataset_tree_model() const;
    void CleanLogFiles();

    void ResetSettings();
    void CheckSettings();
    QSettings *settings();

    //bool SavePlot(QCustomPlot *plot, const QString filename) const;


private:

    void UpdateTreeModel();

    //pointers to main window and necessary widgets

    ///
    /// \brief main_window_
    /// The main window of the program
    MainWindow *main_window_;

    ///
    /// \brief dataset_tree_model_
    /// The model for the dataset list widget.
    DatasetTreeModel *dataset_tree_model_;

    ///
    /// \brief directory_
    /// The directory used by all the file dialogs in the program
    QString directory_; //= QDir::homePath();


    ///
    /// \brief dataset_loading_count_
    /// A count of all datasets created. Used to set name of dataset.
    unsigned int dataset_loading_count_;

    ///
    /// \brief global_data_range_
    /// The global datarange
    QCPRange global_data_range_;

    ///
    /// \brief global_color_gradient_
    /// The global color gradient
    QCPColorGradient global_color_gradient_;

    ///
    /// \brief settings_
    /// The software's settings
    QSettings settings_;

    ///
    /// \brief data_model_
    /// A model that keeps track of the datasets in the workspace and their members
    DataModel *data_model_;

};

#endif // VESPUCCIWORKSPACE_H
