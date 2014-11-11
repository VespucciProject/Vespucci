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
#ifndef DATAVIEWER_H
#define DATAVIEWER_H

#include <QDialog>
#include "vespucciworkspace.h"
#include "vespuccitablemodel.h"
#include "vespuccidataset.h"
class VespucciDataset;
class VespucciWorkspace;
namespace Ui {
class DataViewer;
class VespucciTableModel;
}

///
/// \brief The DataViewer class
/// Window that displays dataset elements in a QTableView widget
class DataViewer : public QDialog
{
    Q_OBJECT

public:
    explicit DataViewer(QWidget *parent = 0, VespucciWorkspace *ws = 0, int row = 0);
    ~DataViewer();

private slots:
    void on_comboBox_currentTextChanged(const QString &arg1);

    void on_plotPushButton_clicked();

    void on_exportPushButton_clicked();

private:
    Ui::DataViewer *ui;

    ///
    /// \brief current_text_
    /// Current selected text in the ComboBox
    QString current_text_;

    ///
    /// \brief directory_
    /// Current working directory (for saving things)
    QString directory_;

    ///
    /// \brief dataset_
    /// The current dataset from which data is pulled
    QSharedPointer<VespucciDataset> dataset_;

    ///
    /// \brief workspace
    /// The current workspace
    VespucciWorkspace *workspace;

    ///
    /// \brief table_
    /// The table view of the data
    QTableView *table_;

    ///
    /// \brief data_selector_
    /// The combobox where the user chooses which dataset element to display
    QComboBox *data_selector_;

    ///
    /// \brief current_data_
    /// The current contents of the tableview
    mat *current_data_;

    ///
    /// \brief export_button_
    /// The user clicks this to open a file dialog to save the current data
    QPushButton *export_button_;

    ///
    /// \brief plot_button_
    /// Opens a plot of the selected spectrum
    QPushButton *plot_button_;

    ///
    /// \brief spectrum_viewer_
    /// The plot window
    SpectrumViewer *spectrum_viewer_;

    ///
    /// \brief vca_endmembers_
    /// Number of plottable VCA endmembers
    int vca_endmembers_;

};

#endif // DATAVIEWER_H
