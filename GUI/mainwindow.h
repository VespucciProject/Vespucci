/*******************************************************************************
    Copyright (C) 2015 Wright State University - All Rights Reserved
    Daniel P. Foose - Author

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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Global/vespucciworkspace.h"
#include "GUI/Processing/rangedialog.h"
class VespucciWorkspace;
class VespucciDataset;
class DatasetListModel;
class RangeDialog;
namespace Ui {
class MainWindow;
}

///
/// \brief The MainWindow class
/// The main window of the program, this is where the user performs most
/// operations
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent, VespucciWorkspace *ws);
    ~MainWindow();
    QCPRange *global_data_range();
    QCPColorGradient *global_gradient();
    void RecalculateGlobalDataRange(QCPRange* new_data_range);
    void RefreshGlobalColorGradient(QCPColorGradient new_gradient);
    void SetGlobalDataRange(QCPRange* new_data_range);
    VespucciWorkspace *workspace_ptr(); //return the workspace
    void DisplayExceptionWarning(std::exception e);
    QListView *map_list_view();
    void SetActiveDatasetListRow(int row);
    bool DatasetMappable(int row);

signals:
    void GlobalGradientChanged(QCPColorGradient gradient);
    void GlobalDataRangeChanged(QCPRange data_range);


protected:
    void closeEvent(QCloseEvent *event);
public slots:
    void RangeDialogAccepted(double min, double max);
private slots:
    void on_actionExit_triggered();

    void on_actionImport_Dataset_from_File_triggered();

    void on_actionAbout_Vespucci_triggered();

    void on_actionCiting_Vespucci_triggered();

    void on_actionNew_Univariate_Map_triggered();

    void on_actionNew_Band_Ratio_Map_triggered();

    void on_actionPrincipal_Components_Analysis_triggered();

    void on_actionNormalize_Standardize_triggered();

    void on_actionSubtract_Background_triggered();

    void on_actionSpectra_triggered();

    void on_actionAverage_Spectra_triggered();

    void on_actionAverage_Spectra_with_Abscissa_triggered();

    void on_actionSpectral_Abscissa_triggered();

    void on_actionAll_Data_triggered();

    void on_actionFilter_Derivatize_triggered();

    void on_actionClose_Dataset_triggered();

    void on_actionDocumentation_triggered();

    void on_actionCrop_triggered();

    void on_actionCorrect_Baseline_triggered();

    void on_actionView_Dataset_Elements_triggered();

    void on_actionSet_Global_Color_Scale_triggered();

    void on_actionPartial_Least_Squares_triggered();

    void on_actionK_Means_Clustering_triggered();

    void on_actionVertex_Components_triggered();

    void on_actionUndo_triggered();

    void on_datasetsListView_clicked(const QModelIndex &index);

    void DatasetAdded(const QModelIndex &index);

    void on_mapsListView_doubleClicked(const QModelIndex &index);

    void on_actionDelete_Map_triggered();

    void on_actionMultivariate_Analysis_triggered();

    void on_actionNew_Composite_Dataset_triggered();

    void on_actionReject_Clipped_Spectra_triggered();

    void on_actionSpectra_as_Columns_triggered();

    void on_actionUnivariate_Analysis_triggered();

    void on_actionLarge_Matrices_triggered();

    void on_actionView_Edit_Spectra_triggered();

    void on_actionBooleanize_Clamp_triggered();

    void on_actionRemove_Vectors_of_Zeros_triggered();

    void on_actionRun_script_triggered();

    void on_actionDetect_Peaks_triggered();

    void on_actionCalculate_Peak_Populations_triggered();

    void on_actionImport_From_Multiple_Point_Spectra_triggered();

    void on_actionBatch_File_Conversion_triggered();

    void on_actionClassical_Least_Squares_triggered();

    void on_actionSettings_triggered();

    void on_actionTransform_Abscissa_triggered();

    void on_actionFourierTransform_triggered();

    void on_actionInterpolate_to_New_Abscissa_triggered();

    void on_actionSave_Log_File_triggered();

private:
    Ui::MainWindow *ui;

    ///
    /// \brief workspace
    /// The current workspace
    VespucciWorkspace *workspace;

    ///
    /// \brief map_list_view_
    /// The list view displaying the currently created for the currently (or most
    /// recently) selected map.
    QListView *map_list_view_;

    ///
    /// \brief dataset_list_widget_
    /// The list widget displaying the currently loaded datasets
    QListView *dataset_list_view_;

    ///
    /// \brief dataset_list_model_
    /// The abstract data model associated with the dataset list widget.
    DatasetListModel *dataset_list_model_;

    unsigned int global_map_count_;
};

#endif // MAINWINDOW_H



