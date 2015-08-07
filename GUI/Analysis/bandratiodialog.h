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

#ifndef BANDRATIODIALOG_H
#define BANDRATIODIALOG_H

#include <QDialog>
#include "GUI/Analysis/univariatedialog.h"

namespace Ui {
class BandRatioDialog;
}

///
/// \brief The BandRatioDialog class
/// The dialog that allows the user to create a band-ratio map
class BandRatioDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BandRatioDialog(QWidget *parent, VespucciWorkspace *ws, int row);
    ~BandRatioDialog();

private slots:
    void on_buttonBox_accepted();

    void on_peakComboBox_currentTextChanged(const QString &arg1);

    void on_buttonBox_rejected();

    void on_firstMinLineEdit_textChanged(const QString &arg1);

    void on_firstMaxLineEdit_textChanged(const QString &arg1);

    void on_secondMinLineEdit_textChanged(const QString &arg1);

    void on_secondMaxLineEdit_textChanged(const QString &arg1);

    void on_mapCheckBox_stateChanged(int arg1);

private:
    Ui::BandRatioDialog *ui;

    ///
    /// \brief workspace
    /// Pointer to the workspace (which contains dataset list)
    VespucciWorkspace *workspace;

    ///
    /// \brief spectrum_plot_
    /// Pointer to the plot inside this window that displays a preview of the data
    QCustomPlot *spectrum_plot_;

    ///
    /// \brief first_min_box_
    /// User enteres first minimum here
    QLineEdit *first_min_box_;

    ///
    /// \brief first_max_box_
    /// User enters first maximum here
    QLineEdit *first_max_box_;

    ///
    /// \brief second_min_box_
    /// User enters second minimum here
    QLineEdit *second_min_box_;

    ///
    /// \brief second_max_box_
    /// User enteres second maximum here
    QLineEdit *second_max_box_;


    ///
    /// \brief integration_method_selector_
    /// User selects the integration method here
    QComboBox *integration_method_selector_;

    ///
    /// \brief name_box_
    /// User enters map name here
    QLineEdit *name_box_;

    ///
    /// \brief color_selector_
    /// User selects the color scheme here
    QComboBox *color_selector_;

    ///
    /// \brief value_method_selector_
    /// User selects the value determination method here
    QComboBox *value_method_selector_;


    ///
    /// \brief integration_method_label_
    /// A label displaying the integration method
    QLabel *integration_method_label_;

    ///
    /// \brief data_index_
    /// Index of the VespucciDataset object in the various lists
    int data_index_;

    ///
    /// \brief range_label_
    /// Displays the valid range
    QLabel *range_label_;

    ///
    /// \brief first_min_line_
    ///
    QCPItemStraightLine *first_min_line_;
    ///
    /// \brief first_max_line_
    ///
    QCPItemStraightLine *first_max_line_;
    ///
    /// \brief second_min_line_
    ///
    QCPItemStraightLine *second_min_line_;
    ///
    /// \brief second_max_line_
    ///
    QCPItemStraightLine *second_max_line_;

    ///
    /// \brief data_
    /// Points to the dataset we're working with.
    QSharedPointer<VespucciDataset> data_;

    QCheckBox *map_check_box_;
};

#endif // BANDRATIODIALOG_H
