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

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent, VespucciWorkspace *ws);
    ~MainWindow();
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

    void on_actionSpatial_Data_triggered();

    void on_actionSpectral_Abcissa_triggered();

    void on_actionAll_Data_triggered();

    void on_actionPrincipal_Component_Statistics_triggered();

    void on_actionFilter_Derivatize_triggered();

private:
    Ui::MainWindow *ui;
    VespucciWorkspace *workspace;
    QListWidget *map_list_widget_;
    QListWidget *dataset_list_widget_;

};

#endif // MAINWINDOW_H
