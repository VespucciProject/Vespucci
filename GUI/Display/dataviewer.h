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
#ifndef DATAVIEWER_H
#define DATAVIEWER_H

#include <QDialog>
#include "Global/vespucciworkspace.h"
#include "GUI/QAbstractItemModel/vespuccitablemodel.h"
#include "Data/Dataset/vespuccidataset.h"
class VespucciDataset;
class VespucciWorkspace;
class UnivariateData;
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

    void on_plotToolButton_clicked();

    void on_exportToolButton_clicked();

    void on_extractToolButton_clicked();

    void on_statsToolButton_clicked();

private:
    Ui::DataViewer *ui;

    QMap<QString, const mat*> data_objects_;
    QMap<QString, std::map<std::string, double> > metadata_objects_;

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
    const mat *current_data_;

    ///
    /// \brief export_button_
    /// The user clicks this to open a file dialog to save the current data
    QToolButton *export_button_;

    ///
    /// \brief plot_button_
    /// Opens a plot of the selected spectrum
    QToolButton *plot_button_;

    ///
    /// \brief extract_button_
    /// The user clicks this to create a new dataset from current data
    QToolButton *extract_button_;

    ///
    /// \brief stats_button_
    /// The user clicks this to view the statistics of the sample
    QToolButton *stats_button_;

    ///
    /// \brief spectrum_viewer_
    /// The plot window
    SpectrumViewer *spectrum_viewer_;

    ///
    /// \brief vca_endmembers_
    /// Number of plottable VCA endmembers
    int vca_endmembers_;

    bool principal_components_calculated_;
    bool vertex_components_calculated_;
    bool k_means_calculated_;
    bool partial_least_squares_calculated_;
    int univariate_count_;

    void RefreshComboBox();

};

#endif // DATAVIEWER_H





