/************************************************************************************
    Copyright (C) 2014 Daniel P. Foose - All Rights Reserved

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
***************************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "vespucciworkspace.h"
class VespucciWorkspace;
class VespucciDataset;

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

signals:
    void GlobalGradientChanged(QCPColorGradient gradient);
    void GlobalDataRangeChanged(QCPRange data_range);


protected:
    void closeEvent(QCloseEvent *event);
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

    void on_actionAverage_Spectra_with_Abcissa_triggered();

    void on_actionSpectral_Abcissa_triggered();

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

private:
    Ui::MainWindow *ui;

    ///
    /// \brief workspace
    /// The current workspace
    VespucciWorkspace *workspace;

    ///
    /// \brief map_list_widget_
    /// The list widget displaying the currently created maps
    QListWidget *map_list_widget_;

    ///
    /// \brief dataset_list_widget_
    /// The list widget displaying the currently loaded datasets
    QListWidget *dataset_list_widget_;

};

#endif // MAINWINDOW_H
