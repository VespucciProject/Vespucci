/*******************************************************************************
    Copyright (C) 2014 Wright State University - All Rights Reserved
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
#ifndef DATAEXTRACTORDIALOG_H
#define DATAEXTRACTORDIALOG_H

#include <QDialog>
#include "vespucciworkspace.h"
#include "mapdata.h"
class VespucciWorkspace;
class VespucciDataset;
class MainWindow;
class MapData;
namespace Ui {
class DataExtractorDialog;
}

///
/// \brief The DataExtractorDialog class
/// A dialog that allows the user to create a new dataset from a map, based on map values.
class DataExtractorDialog : public QDialog
{
    Q_OBJECT

public:
    DataExtractorDialog(QWidget *parent, MapData *map, QSharedPointer<VespucciDataset> dataset, MainWindow *main_window);
    DataExtractorDialog(QWidget *parent, vec data, QSharedPointer<VespucciDataset> dataset, MainWindow *main_window, QString name);
    ~DataExtractorDialog();

private slots:
    void on_buttonBox_accepted();
    
    void on_buttonBox_rejected();

    void on_methodComboBox_currentIndexChanged(const QString &arg1);

    void on_browsePushButton_clicked();

private:
    Ui::DataExtractorDialog *ui;

    vec condition_;

    ///
    /// \brief method_combo_box_
    /// User selects method (value-based or index-based) here.
    QComboBox *method_combo_box_;


    ///
    /// \brief name_line_edit_
    /// User sets the dataset name here
    QLineEdit *name_line_edit_;

    ///
    /// \brief name_line_edit_
    /// User sets the dataset name here
    QLineEdit *filename_line_edit_;


    ///
    /// \brief lower_box_
    /// User sets the lower bound here
    QDoubleSpinBox *lower_box_;

    ///
    /// \brief upper_box_
    /// User sets the upper bound here
    QDoubleSpinBox *upper_box_;

    ///
    /// \brief map_
    /// Pointer the map from which the dataset is formed
    MapData *map_;

    ///
    /// \brief dataset_
    /// The dataset from which the new dataset is formed
    QSharedPointer <VespucciDataset> dataset_;

    ///
    /// \brief data_range_
    /// The data range of the map (used to set limits)
    QCPRange data_range_;

    ///
    /// \brief workspace
    /// The workspace
    VespucciWorkspace *workspace;

    ///
    /// \brief main_window_
    /// The main window
    MainWindow *main_window_;
};

#endif // DATAEXTRACTORDIALOG_H
