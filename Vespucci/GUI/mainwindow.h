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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Global/vespucciworkspace.h"
#include "GUI/Processing/rangedialog.h"
#include "GUI/Display/plotviewer.h"
#include "GUI/Display/spectrumeditor.h"
#include "GUI/macrodialog.h"
#include "GUI/pythonshelldialog.h"
#include "GUI/Display/statsdialog.h"
class PythonShellDialog;
class SpectrumEditor;
class VespucciWorkspace;
class VespucciDataset;
class DatasetTreeModel;
class RangeDialog;
class SpectrumViewer;
class DataViewer;
class PlotViewer;
class DataModel;
class StatsDialog;
class MacroDialog;
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
    explicit MainWindow(QWidget *parent, QSharedPointer<VespucciWorkspace> ws);
    ~MainWindow();
    void RefreshTreeModel(const DataModel *data_model);
    QCPRange *global_data_range();
    QCPColorGradient *global_gradient();
    void RecalculateGlobalDataRange(QCPRange* new_data_range);
    void RefreshGlobalColorGradient(QCPColorGradient new_gradient);
    void SetGlobalDataRange(QCPRange* new_data_range);
    QSharedPointer<VespucciWorkspace> workspace_ptr(); //return the workspace
    void DisplayExceptionWarning(std::exception e);
    void DisplayExceptionWarning(std::string where, std::exception e);
    QListView *map_list_view();
    void SetActiveDatasetListRow(const QModelIndex &index);
    void SetActiveDatasetTreeIndex(const QModelIndex &index);
    void DisplayWarning(const QString &title, const QString &text);
    void DisplayInformation(const QString &title, const QString &text);
    void SetDatasetTreeModel(DatasetTreeModel *new_model);

    DatasetTreeModel *dataset_tree_model();
    PlotViewer *plot_viewer();
    DataViewer *data_viewer();

signals:
    void DatasetSelectionChanged(QString dataset_key);
    void MatrixSelectionChanged(QStringList matrix_keys);
    void DatasetToBeRemoved(QString name);
    void MatrixToBeRemoved(QStringList keys);

protected:
    void closeEvent(QCloseEvent *event);
public slots:
    void RangeDialogAccepted(double min, double max);
    void SetPlotViewerActionChecked(bool checked);
    void SetDataViewerActionChecked(bool checked);
    void SetStatsViewerActionChecked(bool checked);
    void SetSpectrumEditorActionChecked(bool checked);
    void SetMacroEditorActionChecked(bool checked);
    void SetPythonShellActionChecked(bool checked);
    void SpectrumRequested(QString dataset_key, QString map_name, size_t index);
    void HeldSpectrumRequested(QString dataset_key, QString map_name, size_t index);
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

    void on_actionPartial_Least_Squares_triggered();

    void on_actionK_Means_Clustering_triggered();

    void on_actionVertex_Components_triggered();

    void on_actionUndo_triggered();

    void on_actionDelete_Map_triggered();

    void on_actionNew_Composite_Dataset_triggered();

    void on_actionReject_Clipped_Spectra_triggered();

    void on_actionSpectra_as_Columns_triggered();

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

    void on_actionImport_Dataset_from_Multiple_Files_triggered();

    void on_actionCreate_Plot_triggered();

    void on_actionPlot_Viewer_toggled(bool arg1);

    void ChildDialogVisibleToggled(const QString &key, bool arg1);

    void on_actionData_Viewer_toggled(bool arg1);

    void on_actionStatistics_Viewer_toggled(bool arg1);

    void on_actionSpectrum_Editor_toggled(bool arg1);

    void on_actionPython_Shell_toggled(bool arg1);

    void on_actionMacro_Editor_toggled(bool arg1);

    void on_actionMapResult_triggered();

    void on_actionOnline_Documentation_triggered();

    void on_actionGlobal_Color_Scales_triggered();

    void on_datasetTreeView_clicked(const QModelIndex &index);

    void on_datasetTreeView_doubleClicked(const QModelIndex &index);

    void on_actionShow_in_Data_Viewer_triggered();

    void on_actionView_Statistics_triggered();

    void on_actionPlotResult_triggered();

    void on_actionSave_Dataset_triggered();

    void on_actionOpenDataset_triggered();

    void on_actionSave_Dataset_As_triggered();

    void on_actionExport_Matrix_triggered();

    void on_actionImport_Data_Into_Dataset_triggered();

    void on_actionCalculate_Representative_Spectrum_triggered();

    void on_actionTransform_triggered();

    void on_actionOn_Multiple_Datasets_triggered();

    void on_actionHierarchical_Clustering_triggered();

private:
    void CloseDataset(const QString &name);
    Ui::MainWindow *ui;

    PlotViewer *plot_viewer_;
    DataViewer *data_viewer_;
    StatsDialog *stats_viewer_;
    MacroDialog *macro_editor_;
    SpectrumEditor *spectrum_editor_;
    PythonShellDialog *python_shell_;


    ///
    /// \brief workspace
    /// The current workspace
    QSharedPointer<VespucciWorkspace> workspace_;


    ///
    /// \brief dataset_list_model_
    /// The abstract data model associated with the dataset list widget.
    DatasetTreeModel *dataset_tree_model_;

    unsigned int global_map_count_;

    QMap<QString, QDialog*> child_dialogs_;

};

#endif // MAINWINDOW_H



